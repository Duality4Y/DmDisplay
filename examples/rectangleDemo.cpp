/*
 * Author: Duality / Robert
 * Edited: 24-12-2013
 * 
 * demo's the workings of the DmDisplay code for drawing a rectangle.
 * 
 * */
 
#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include "DmDisplay.h"

//new instance of DmDisplay called lcd.
DmDisplay lcd;

//main function
int main(void)
{
	//clear incase it has random stuff in display ram.
	lcd.clear();
	//clear markers incase it has randomly set markers in ram.
	lcd.clearMarkers();
	//set the display contrast.
	lcd.setContrast(15);
	while(1)
	{
		//draw a rect from x=30,y=30,width= 20,height=12
		lcd.drawRect(30,30, 20,12);
	}
	return 0;
}

