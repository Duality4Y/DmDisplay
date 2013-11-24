//clock speed 8mhz
#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>

#include <stdlib.h>
#include <stdio.h>
#include "DmDisplay.h"

//for string representation of binary numbers;

char string[21];

DmDisplay lcd;

void writePixel(uint8_t x, uint8_t y)
{
}

void writeLCDcontrast(int contrastVal)
{
	char str[21];
	sprintf(str, " -> Contrast: %d    ", contrastVal);
	lcd.lcdChar(str);
}
void writeSomeTestText()//DmDisplay display)
{
	lcd.setRow(1);
	lcd.lcdChar("012345678901234567890");
	lcd.setRow(2);
	lcd.lcdChar("ABCDEFGHIJKLMNPQRSTUV");
	lcd.setRow(3);
	lcd.lcdChar("wxyzabcdefghijklmnopq");
	lcd.setRow(4);
	lcd.lcdChar("  !#$%&'( )@^-");
	lcd.setRow(5);
	lcd.lcdChar("  *+-/<>?;:[]");
	lcd.setRow(6);
	lcd.lcdChar("* AVR rules !! *");
}

char *numbToBin(uint8_t number)
{
	itoa(number, string, 2);
	return string;
}
//this is for debugging with sprintf which needs a buffer;
//char formatedText[21];

int main(void)
{
	lcd.clear();
	for(int i = 0;i<6;i++)
	{
		lcd.setWriteAddress(i, i);
		lcd.lcdChar("H");
	}
	// x 0-19 (20) and y 0-5 (6)
	lcd.setCursor(19,5);
	lcd.lcdChar("a");
	while(1)
	{
		
	}
	return 0;
}
