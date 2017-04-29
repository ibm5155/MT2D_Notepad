#include <stdio.h>
#include <stdlib.h>
#include "..\..\MT2D\Window_core.h"
#include "..\..\MT2D\Keyboard.h"
#include "Notepad.h"

void  main(){
	int i=0;
	//
//	printf("%c%c%c%d",NULL,NULL,NULL,Keyboard_keytouched());
//	system("pause");
	//
	clear_display();
    Start_Notepad_Window_Background();
	print_display(DISPLAY_WINDOW1);
	goto_topscreen();
    Read_Write_Screen_keyboard();
}