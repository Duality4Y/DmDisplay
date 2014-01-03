/*
 * author Duality / Robert
 * 
 * edited: 24-12-2013
 * 
 * Game of life inplemented on SED1531 Display.
 * with DmDisplay code.
 * */

#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>

//cell size.
#define CELLSIZE 10 //in pixels
//for life.cpp fieldWidth fieldHeight fieldSize
#define fieldWidth	 	100 					//in pixels
#define fieldHeight 	48 						//in pixels
#define fieldSize		fieldWidth*fieldHeight 	//in pixels
#include "life.cpp"
//for controlling the display
#include "DmDisplay.h"
//lcd instance of display.
DmDisplay lcd;

//show field for position.
void showField(DmDisplay lcd, uint8_t *field, int position)
{
	//calculate x and y location from position.
	static uint8_t x = position%fieldWidth;
	static uint8_t y = 0;
	if(!(x))
	{
		y++;
		if(y>(fieldHeight-1))
		{
			y = 0;
		}
	}
	//set this location in field.
	lcd.setWriteReadAddres(x,y);
	//draw a cel if there is a 1 at position in field.
	//else draw no cell.
	if(field[position])
	{
		lcd.drawCircle(x,y,CELLSIZE);
	}
	else
	{
		lcd.drawCircle(x,y,0);
	}
}

int main(void)
{
	return 0;
}
