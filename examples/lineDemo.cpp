/*
 * Author: Duality / Robert
 * Edited: 24-12-2013
 * 
 * demo's the workings of the DmDisplay code for drawing a line.
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
		//draw a set of lines. from x=0,y=48 to x=i,y=i
		for(int i = 0;i<10;i++) lcd.drawLine(0,48,i*i,i);
	}
	return 0;
}

