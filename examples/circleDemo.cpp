/*
 * Author: Duality / Robert
 * Edited: 24-12-2013
 * 
 * demo's the workings of the DmDisplay code for drawing a circle.
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
		//draw a circle with diameter 10, at position x=30, y=30
		lcd.drawCircle(30,30, 10);
	}
	return 0;
}
