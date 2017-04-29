#include <stdio.h>
#include <stdlib.h>
#include "..\..\MT2D\Window_core.h"
#include "..\..\MT2D\Keyboard.h"
#include "..\..\MT2D\Io_FILE.h"
#include "..\..\MT2D\building_functions\display_popup.h"
#include "..\..\MT2D\building_functions\display_popup_with_result.h"
#include "..\..\MT2D\building_functions\generic_line.h"
#include "..\..\MT2D\building_functions\generic_string_reader.h"
extern char WINDOW1[MAX_VER+1][MAX_HOR];



void Start_Notepad_Window_Background(){
	create_window_layout(0,0,MAX_HOR-1,MAX_VER,false,true,false,' ',DISPLAY_WINDOW1);
	insert_string_on_display("ANSI_WORD_ALPHA 1.0",0,5,DISPLAY_WINDOW1);
	insert_string_on_display("(ESC) Save/Quit",1,3,DISPLAY_WINDOW1);
	insert_string_on_display("(Insert) OverWrite text",1,20,DISPLAY_WINDOW1);
	insert_string_on_display("CREATED_BY  LUCAS_ZIMERMAN",24,5,DISPLAY_WINDOW1);
}


char *Read_Write_Screen_keyboard(){
	char *string=0;
	char *addres;
	FILE *file;
	int string_size=0;
	char buff;//Buffer to get char
	int pos_hor=1,pos_ver=3;//
	int win_hor=0,win_ver=0;// Position where the window will start to be
	int j=0,i=0,k=0;
	int l=0,m=0,n=0,o=0;
	int string_pos=0;
	int first_text_position=0;//Antepenultima Posição
	bool out_loop=false;;
	string=(char*)malloc(sizeof(char));
	string[0]='\0';
	i=win_hor+1;
	j=win_ver+3;
	WINDOW1[j][i]='>';
	while(!out_loop){
		print_display(DISPLAY_WINDOW1);
		goto_topscreen();
		change_cursor_position(j,i);
		while(!Keyboard_touched());
		buff=Keyboard_keytouched();
//==========================TEXT VECTOR FILTER=================================================
// START
		if(buff==_key){//If it's up down left right key
			buff=Keyboard_keytouched();
			buff=0;
		}else if(buff==back_space){//Delete Key
			if(string_size>1){//There is a char to be deleted
				string_size--;
				string=(char*)realloc(string,(string_size+1)*sizeof(char));
				string[string_size]='\0';
				string_size++;
			}else{//There is no char to be deleted
				string_size=0;
				string[0]=' ';
				WINDOW1[j][i]=' ';
			}
		}else if(buff==27){//Get out from the reader
			buff=print_popup_wiht_result("ESC PRESSED","would you like to save this text?","Yes","No");
			if(buff==esc){
				addres=get_string_popup("Type the name of your file");
				buff=check_if_file_exist(addres);
				if(buff){
					buff=print_popup_wiht_result("Warning","There is already a file with the same name. Would you like to continue?","Yes","No");
					if(buff==esc){
						//SAVE DATA TO FOLDER
						file=open_file(addres,"w");
						for(k=0;k<string_size;k++){
							if(string[k]==13) fputc('\n',file);
							else fputc(string[k],file);
						}
						fclose(file);
						print_popup("File saved                                Press any key to exit");
					}
				}
				else{
					//SAVE DATA TO FOLDER
					file=open_file(addres,"wb");
					for(k=0;k<string_size;k++){
						if(string[k]==13) fputc('\n',file);
						else fputc(string[k],file);
					}
					if(file)fclose(file);
					print_popup("File saved                                Press any key to exit");
				}
			}
			out_loop=true;
		}else{//It's a normal char, so it'll load the char to the string...
			string_size++;
			string=(char*)realloc(string,(string_size+1)*sizeof(char));
			string[string_size]='\0';
			if(buff==tab){
				buff=' ';
				string[string_size-1]=tab;
			}else string[string_size-1]=buff;
		}
//==========================SCREEN FILTER=================================================
// START
		if(string_size==0){k=0;
			i=win_hor+1;
			j=win_ver+3;
		}else k=string_size-1;

		if(string[k]==enter){//====ENTER LINE
			WINDOW1[j][i]=' ';
			j++;
			i=win_hor+1;
			if(j<23)WINDOW1[j][i]='>';
		}else if(i==MAX_HOR-1){//End of line
			if(buff==back_space){
				WINDOW1[j+1][1]=' ';
				i--;
				WINDOW1[j][i]='>';
				string_size--;
			}else{
				j++;
				i=win_hor+1;
				if(j<23){
					WINDOW1[j][i]=string[k];
					WINDOW1[j][i+1]='>';
				}
			}
		}else if(buff==back_space){//Erase a Char
 			if(i>1){
				WINDOW1[j][i]=' ';
				WINDOW1[j][--i]='>';
			}else if(string_size==0){
				string_size++;
				WINDOW1[j][i]='>';
			}else{//				Complex search to find on window where is the last position before the last line stoped
				if(j>win_ver+3){// Avoid to erase the place where couldn't put text
					WINDOW1[j][i]=' ';
					j--;
					i=78;
					n=string[k-1];
					if(n==enter){// Go back one line on first colun if the last line was a enter
						i=1;
						WINDOW1[j][i]='>';
					}//else if(WINDOW1[j][i]==n){
					//	WINDOW1[j][i]='>';
					//else if(n==tab)n=' ';
					else if(n==' '){//If the last line before enter is a space
						o=k-1;
						while(string[o]!=enter && o>=0){
							o--;//o=69
						}if(o==0){//NO
							i=2;//last line, just count the spaces to the spot to be wrote
						}else{
							i=k-o;//i=53
							if(i>78){
								while(i>79){
									i-=78;
								}
								i--;
							}
						}	
						WINDOW1[j][i]='>';
					}
					else{//If the last char before enter is a number or a letter do
						o=WINDOW1[j][i];
						while(o!=n){
							if(i==0){
								i=78;
								j--;
							}else i--;
							o=WINDOW1[j][i];
						}i++;
						if(i<78)WINDOW1[j][i]='>';
					}
					
				}
			}

			string_size--;
		}else{
			if(string[k]==tab) WINDOW1[j][i]=' ';
			else WINDOW1[j][i]=string[k];
			if(i<78)WINDOW1[j][i+1]='>';
			else WINDOW1[j+1][1]='>';
			i++;
		}if(j==24){//24 lines exeded, erasing last one ...
			for(l=first_text_position+1;l<first_text_position+78 && string[l]!=13;l++);
			first_text_position=l;//Tenho a ultima posição que o texto foi apagado	
			j--;
			i=win_hor+1;
			for(l=win_ver+4,m=win_hor+1;l<MAX_VER;l++){
				while(m<MAX_HOR-1){
					WINDOW1[l-1][m]=WINDOW1[l][m];
					m++;
				}
				m=win_hor+1;
			}
			while(m<MAX_HOR-1){
				WINDOW1[l-1][m]=' ';
				m++;
			}
			m=win_hor+1;
			if(string[k]!=enter){
				WINDOW1[j][i]=string[k];
				WINDOW1[j][i+1]='>';
			}else WINDOW1[j][i]='>';
		}else if(j==22 && first_text_position>0){//Draw back the old line=============================================================

			//TRANSFER WINDOW1 BUFFER TO DOWN (START)
			i=win_hor+1;
			for(l=MAX_VER-2,m=win_hor+1;l>win_ver+3;l--){
				while(m<MAX_HOR-1){
					WINDOW1[l][m]=WINDOW1[l-1][m];
					m++;
				}
				m=win_hor+1;
			}
		//TRANSFER WINDOW1 BUFFER TO DOWN (END)
		//SEARCH OF THE PREVIOUS TEXT START (START)
			m=first_text_position-78;//Create a probable text position
			
			for(l=first_text_position-1;l>=m && string[l]!=13 && l>=0;l--);//IT
			o=l;
		//SEARCH OF THE PREVIOUS TEXT START (END)

			if(l==first_text_position-1){//Line before was just 'enter', so clean line
				for(i=win_ver+1,m=win_hor+3;i<MAX_HOR-1;i++){
					WINDOW1[m][i]=' ';
				}//OK
				if(string[l]!=13)WINDOW1[win_ver+3][win_hor+1]=string[l];
			}else{
				j=win_ver+3;
				i=win_hor+1;
				l++;
				while(l<=first_text_position-1){
					WINDOW1[j][i]=string[l];
					l++;
					i++;
				}if(i<78)WINDOW1[j][i]=' ';
			}	//AJUSTA PONTEIRO POSIÇÃO
			j=23;
			i=MAX_HOR-2;
			n=string[k-1];
			if(n==enter){//If the last one position on string is a enter too, just go back one line
				i=1;
				WINDOW1[j][i]='>';
			}else if(WINDOW1[j][i]==n)WINDOW1[j][i]='>';
			else{
				o=WINDOW1[j][i];
				while(o!=n){
					if(i==0){
						i=78;
						j--;
					}else i--;
					o=WINDOW1[j][i];
				}i++;
				if(i<78)WINDOW1[j][i]='>';
			}
			j=23;
			first_text_position=o;
					
					//---------------
		}
//==========================SCREEN FILTER=================================================
// END
	}
	return string;
}
