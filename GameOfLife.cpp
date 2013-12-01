/*
 * author: Robert van der Tuuk
 * Edited on: 27 Nov 2013
 * */

//clock speed 8mhz
#define F_CPU 8000000

//needed libraries
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

//the code for working with the display. 
#include "DmDisplay.h"

//display write location.
int x,y = 0;
//create a display instance
DmDisplay lcd;
//number it takes to survive and to reproduce/become alife.
#define surviveAbility 2
#define reproductiveNumber 3
//parameters that define how big the field is. and how big a buffer to use.
#define fieldWidth 20
#define fieldHeight 5
#define fieldSize (fieldWidth*fieldHeight)
uint8_t field[fieldSize+1];
//and include with functions that make the game of life go.
#include "life.cpp"
//variables for determining wether we are is a steady state or still evolving.
uint8_t currentState = 0;
uint8_t previousState = 0;
uint8_t changeCount = 0;
uint8_t holdingNumber = 10;//how many iterations it takes before desiding that there is no evolution happening anymore.
//for holding how many iterations it took.
int iterations = 0;
//-------------------

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
	sprintf(str, "%s:%d  ", aString, val);
	lcd.lcdChar(str);
}
void writeFormated(int val, int val2, const char *aString)
{
	char str[21];
	sprintf(str, "%s:%d, %d  ", aString, val, val2);
	lcd.lcdChar(str);
}
//write pixel data not instructions.
void writePixelData(uint8_t data)
{
	lcd.write(data, lcd.DATA);
}
//randomly create cells at position in field.
void fillField(uint8_t *field, int position)
{
	uint16_t randomVal = rand()%2;
	field[position] = (bool)randomVal;
}
//make a random playing field.
void createRandomField(uint8_t *field)
{
	for(int i = 0;i<fieldSize;i++)
		fillField(field, i);
}
//used to draw a lifing cell with a box
void writeBlackBox(void)
{
	writePixelData(0xFF);
	writePixelData(0x81);
	writePixelData(0x81);
	writePixelData(0x81);
	writePixelData(0xFF);
}
//this actually is the same as lcd.lcdChar(" ");
//cause it just writes 5 empty bytes.
void writeBlock(void)
{
	for(int i = 0;i<5;i++)
	{
		lcd.write(0x00, lcd.DATA);
	}
}
//display the field with position.
void showField(uint8_t *field, int position)
{
	//calculate x and y location from position.
	x = position%fieldWidth;
	if(!(position%fieldWidth))
	{
		y++;
		if(y>(fieldHeight-1))
		{
			y=0;
		}
	}
	//set this location.
	lcd.setCursor(x,y);
	//draw a cell if there is a 1 at this position.
	//else draw a empty block.
	if(field[position])
	{
		writeBlackBox();
	}
	else
	{
		writeBlock();
	}
}
//my implementation of delay, it's blocking.
//_delay_ms(); requires a constant for an argument.
void delay(int n)
{
	while(n--)
	{
		_delay_ms(1);
	}
}
/*
void printDebug()
{
	lcd.setCursor(0,0);
	writeLCDvalue(position%fieldWidth, ">>Width");
	lcd.setCursor(0,1);
	writeLCDvalue(y, ">>Height");
	lcd.setCursor(0,2);
	writeLCDvalue(position,">>Position");
}*/
uint8_t checkField(uint8_t *field)
{
	uint8_t countCels = 0;
	for(int i = 0;i<fieldSize;i++)
	{
		if(field[i])
			countCels++;
	}
	return countCels;
}
int main(void)
{
	for(int i = 0;i<6;i++)
		lcd.setMarker(i,0);
	//init analog if needed
	init_analog();
	srand(adc_read(0));
	
	//pin 0 on portb output
	DDRB |= _BV(PB0);
	//pin 2 on portb input
	DDRB &= ~(_BV(PB2));
	//turn on interupt on int2
	GICR |= (1<<INT2);
	SREG |= (1<<7);
	//trigger on rising edge
	MCUCSR |= (1<<ISC2);
	sei();
	//randomly generate a playing field.
	for(int i = 0;i<fieldSize;i++)
		fillField(field, i);
	//set contrast.
	lcd.setContrast(16);
	//make sure to start at location 0,0
	lcd.setCursor(0,0);
	//position is field size, I refresh screen Backwards.
	int position = fieldSize;
	while(1)
	{
		//display field with current position
		showField(field, position);
		//here the rules of the game of life are checked.
		//if a position has a cell (1),
		//then look how many around,
		//if 2 or 3 around it lives, else it dies.
		if(field[position])
		{
			if(totalAround(field, position)==surviveAbility)
			{
				field[position]=1;
			}
			else if(totalAround(field, position)==surviveAbility+1)
			{
				field[position]=1;
			}
			else
			{
				field[position]=0;
			}
		}
		//but if a position in the field is empty
		//and it has 3 around, that position becomes alife.
		else
		{
			if(totalAround(field, position)==reproductiveNumber)
			{
				field[position]=1;
			}
		}
		//position keeps position in the field array,
		//and at the same time drawing location on
		//screen.
		position--;
		if(position == 0)
		{
			position = fieldSize;			
			//set delay with a potmeter aka frame rate :)
			//could be done with a adc Interrupt ?
			//and set value that way?
			delay(adc_read(1)/2);	
			//check wether we are in a steady state or just still evolving.
			currentState = checkField(field);
			if(changeCount == holdingNumber)
			{
				//reset changeCount
				changeCount = 0;
				//reset iteration count.
				iterations = 0;
				//create a random playing field.
				createRandomField(field);
				//is blocking button won't work because it blocks interrupts,
				//and it will then also not be able to listen to button.
				delay(2000);
			}
			else if(currentState == previousState)
			{
				PORTB |= (1<<PB0);
				changeCount++;
			}
			else
			{
				PORTB &= ~(1<<PB0);
				previousState = currentState;
				//changeCount shouldn't change if the inbetween states happen te be the same.
				//that is why setting it to zero.
				changeCount = 0;
				iterations++;
				//set location and nicely print something.
				lcd.setCursor(0,5);
				writeFormated(iterations-changeCount,changeCount,"Game of Life");
			}
		}
		//also turn on led to see if the button read works.
		if(PINB & (1<<PB2))
		{
			PORTB |= (1<<PB0);
		}
		else if(currentState != previousState)
		{
			PORTB &= ~(1<<PB0);
		}
	}
	return 0;
}

//our pin change interrupt.
//we have it set to be triggered on rising edge.
//thus if the button is pressed, not when it is 
//released.
ISR(INT2_vect)
{
	createRandomField(field);
	//reset variables depending on cel count.
	iterations = 0;
	changeCount = 0;
	//dit is blocking
	//while(PINB & (1<<PB2));
	//PORTB &= ~(1<<PB0);
}
