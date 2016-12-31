/*
 * Author: Duality / Robert
 * Edited: 15-12-2013
 * 
 * 
 * */

//clock speed 8mhz
#define F_CPU 8000000

#include <avr/io.h>
#include <math.h>
#include <stdlib.h>
#include <util/delay.h>

#include <string.h>
#include <stdio.h>
#include "DmDisplay.h"

DmDisplay lcd;

//init analog
void init_analog(void)
{
	//select 5v analog reference.
	ADMUX = (1<<REFS0);
	//enable adc and use a prescaler of 128
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}
//adc returns a 10bit int so uint8_t isn't going to do it for us.
uint16_t adc_read(uint8_t ch)
{
	//only check for 3 first bits, or else we would select other options.
	ch &= 0x07;
	//clear first 3 bits before ORring, just incase.
	ADMUX = (ADMUX & 0xF8)|ch;
	//start conversion
	ADCSRA |= (1<<ADSC);
	//wait for it to complete.
	while(ADCSRA & (1<<ADSC));
	//return the value.
	return (ADC);
}
//writeFormated() for nicely printing things onto the screen.
void writeFormated(const char *aString)
{
	lcd.lcdChar(aString);
}
void writeFormated(int val, const char *aString)
{
	char str[21];
	sprintf(str, "%s%d", aString, val);
	lcd.lcdChar(str);
}
void writeFormated(int val, int val2, const char *aString)
{
	char str[21];
	sprintf(str, "%s%d, %d  ", aString, val, val2);
	lcd.lcdChar(str);
}

void writeLCDcontrast(int contrastVal)
{
	char str[21];
	sprintf(str, "value: %d", contrastVal);
	lcd.lcdChar(str);
}

void writeSomeTestText()//DmDisplay display)
{
	lcd.setRow(0);
	lcd.lcdChar("012345678901234567890");
	lcd.setRow(1);
	lcd.lcdChar("ABCDEFGHIJKLMNPQRSTUV");
	lcd.setRow(2);
	lcd.lcdChar("wxyzabcdefghijklmnopq");
	lcd.setRow(3);
	lcd.lcdChar("  !#$%&'( )@^-");
	lcd.setRow(4);
	lcd.lcdChar("  *+-/<>?;:[]");
	lcd.setRow(5);
	lcd.lcdChar("* AVR rules !! *");
}
//for fun with bubles.
//returns a random buble size.
uint8_t randSize(uint8_t max, uint8_t min)
{
	uint8_t size = rand()%max;
	while(size < min) size = rand()%max;
	return size;
}
//creates/draws a buble
void show_buble(uint8_t x, uint8_t y, uint8_t i)
{
	for(int wall = 0;wall<3;wall++)
	{
		lcd.drawCircle(x,y,i+wall);
	}
}

void create_bubbles(uint8_t bubles[][2], uint8_t number_of_bubles)
{
	for(uint8_t num = 0;num<number_of_bubles;num++)
	{
		bubles[num][0] = rand()%100;
		bubles[num][1] = rand()%48;
	}
}

//for delaying
void delay(int delay_val)
{
	while(delay_val--)
		_delay_ms(1);
}
//fun bubbles exploding or imploding.
void collapseExplosion(uint8_t type)
{
	//max size and min sizes for bubles.
	static const uint8_t max_size = 22;
	static const uint8_t min_size = 2;
	uint8_t size = randSize(max_size, min_size);
	//number of bubbles.
	static const uint8_t numBubles = 4;
	uint8_t bubles[numBubles][2] = {};
	create_bubbles(bubles, numBubles);
	//for delaying the refresh.
	uint8_t delay_val = 10;
	if(type)
	{
		for(int i = 0;i<size;i++)
		{
			for(int buble = 0;buble < numBubles;buble++)
				show_buble(bubles[buble][0], bubles[buble][1], i);
			delay(delay_val);
			lcd.clear();
		}
	}
	else
	{
		for(int i = size;i>0;i--)
		{
			for(int buble = 0;buble < numBubles;buble++)
				show_buble(bubles[buble][0], bubles[buble][1], i);
			delay(delay_val);
			lcd.clear();
		}
	}
}

int main(void)
{
	lcd.clear();
	lcd.invertDisplay(true);
	lcd.clearMarkers();
	lcd.setContrast(16);
	//init analog if needed
	init_analog();
	srand(adc_read(0));
	while(1)
	{
		//collapseExplosion(1);
		//collapseExplosion(0);
		lcd.drawRect(10,10,10,10);
		lcd.drawLine(0,0,100,48);
		lcd.drawCircle(20,20,5);
	}
	return 0;
}
