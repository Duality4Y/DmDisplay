#include "DmDisplay.h"

//sets up a few variables and ports
//like setting data port to output.
//and setting the control lines to output.
//and call init to init the display.
DmDisplay::DmDisplay(void)
{
	FUNCTIONPORT_TO_OUTPUT;
	DATAPORT_TO_OUTPUT;
	_delay_ms(100);
	init();
	_delay_ms(100);
}
//initializes the display.
void DmDisplay::init(void)
{
	write(0xE2, INSTRUCT);
	_delay_ms(1000);
	write(0xA1, INSTRUCT);
	write(0xA2, INSTRUCT);
	write(0x2C, INSTRUCT);
	write(0x2E, INSTRUCT);
	write(0x2F, INSTRUCT);
	write(0xA6, INSTRUCT);
	write(0x8F, INSTRUCT);
	write(0xA4, INSTRUCT);
	write(0xAF, INSTRUCT);
	write(0x40, INSTRUCT);
	clear();
}

//type = wheter it's instruction/command or data.
//if DATA it's pixel data.
//if INSTRUCT it's instruction data.
void DmDisplay::write(uint8_t data)
{
	write(data, DATA);
}
void DmDisplay::write(uint8_t data, uint8_t type)
{
	DATA_PORT_CLEAR;
	if(type == DATA)
	{
		CLEAR_RW;
		SET_A0;
		
		DATA_PORT(data);
		
		SET_ENABLE;
		_delay_us(10);
		CLEAR_ENABLE;
		_delay_us(10);
		SET_ENABLE;
	}
	else if(type == INSTRUCT)
	{
		CLEAR_RW;
		CLEAR_A0;
		
		DATA_PORT(data);
		
		SET_ENABLE;
		_delay_us(10);
		CLEAR_ENABLE;
		_delay_us(10);
		SET_ENABLE;
	}
}
//sets contrast of screen, with a value between 0-31
void DmDisplay::setContrast(uint8_t contrast)
{
	write(0x80+contrast, INSTRUCT);
}
//allows to set the markers on the top of the screen.
void DmDisplay::setMarker(uint8_t marker, bool state)
{
	uint8_t heighNibble, lowNibble;
	uint8_t lcd_marker = 0;
	
	switch(marker)
	{
		case 1:
			lcd_marker = 20;
			break;
		case 2:
			lcd_marker = 31;
			break;
		case 3:
			lcd_marker = 32;
			break;
		case 4:
			lcd_marker = 57;
			break;
		case 5:
			lcd_marker = 69;
			break;
		case 6:
			lcd_marker = 78;
			break;
	}
	
	lowNibble = lcd_marker & 0xF;
	heighNibble = lcd_marker;
	heighNibble = (heighNibble >> 4);
	heighNibble |= 0x10;
	
	write(0xB6, INSTRUCT);
	write(heighNibble, INSTRUCT);
	write(lowNibble, INSTRUCT);
	
	write((state?1:0), DATA);
}
//inverts display turns pixels that are on off, and off to on.
void DmDisplay::invertDisplay(bool reverse)
{
	write(0xA6+(reverse), INSTRUCT);
}
//sets column address, thus which column of bytes to address. (bytes represents pixels.)
void DmDisplay::setCol(uint8_t col)
{
	int higher = col>>4;
	int lower = (col&0x0F);
	
	write((0x10|higher), INSTRUCT);
	write(lower, INSTRUCT);
}
//sets page address. thus which row of bytes to address. (bytes represents pixels.)
void DmDisplay::setRow(uint8_t row)
{
	int page = (0xB0|row);
	write(page, INSTRUCT);
}

//toggles display on or off, depending on state.
void DmDisplay::toggleDisplayOnOff(bool state)
{
	int new_state = (0xAE + state);
	write(new_state, INSTRUCT);
}
//a function for putting a single character on display.
void DmDisplay::putChar(char lcd_char)
{
	for(uint8_t byte = 0;byte<5;byte++)
	{
		write( pgm_read_byte( &(Font[lcd_char][byte])), DATA);
	}
};
//prints a char to screen, increments column addres by char width.
void DmDisplay::putStr(const char *str)
{
	int charCount = strlen(str);
	//if there are more chars then 
	//there is room on a line. just make it line size.
	if(charCount > 20)
	{
		charCount = 20;
	}
	
	//start read-modify-write
	write(0xE0, INSTRUCT);
	//a loop that write the characters to the screen.
	for(int i = 0;i<charCount;i++)
	{
		uint8_t currentChar = str[i];
		uint8_t lcd_char = currentChar-32;
		putChar(lcd_char);
	}
	//end of read-modify-write
	write(0xEE, INSTRUCT);
	//reset column address
	resetColumnAdress();
}
//fill screen with spaces.
//so it seems cleared. well is cleared.
void DmDisplay::clear(void)
{
	home();
	for(int i = 0;i<6;i++)
	{
		setRow(i);
		putStr("                    ");
	}
}
//set write location for example writing pixel data.
//single pixel distances.
void DmDisplay::setWriteReadAddres(uint8_t columnAddr, uint8_t page)
{
	//bound check
	if(page >= 6)
			page = 6;
	if(columnAddr >= 100)
			columnAddr = 100;
	if(page < 0)
		page = 0;
	if(columnAddr < 0)
		columnAddr = 0;
	//set columnAddr.
	setCol(columnAddr);
	//set page adress.
	setRow(page);
}
//set cursor to location x,y with in bounds of the screen size.
//uses character distances.
void DmDisplay::setCursor(uint8_t x, uint8_t y)
{
	// this function wil do nothing if you specify numbers
	// outside of the screen
	if(x < 20)
	{
		if(y < 6)
		{
			//font width is 5
			setWriteReadAddres(x*5, y);
		}
	}
}
//sets print location to 0.0
//uses character distances.
void DmDisplay::home(void)
{
	setCursor(0,0);
}
//resets column addres aka sets to position 0,x
//single pixel distances
void DmDisplay::resetColumnAdress(void)
{
	setCol(0);
}
//resets row addres. aka sets to position x,0
//single pixel distances.
void DmDisplay::resetRowAdress(void)
{
	setRow(0);
}
//for testing purposes
void DmDisplay::blinkControlLines(void)
{
	SET_A0;
	SET_ENABLE;
	SET_RW;
	_delay_ms(250);
	CLEAR_A0;
	CLEAR_ENABLE;
	CLEAR_RW;
	_delay_ms(250);
}


//draws a line.
//based on this line drawing function:
//https://code.google.com/p/glcd-arduino/downloads/detail?name=glcd-v3-20111205.zip&can=2&q=
//which source says its actually based on:
//http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void DmDisplay::drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
	int deltax, deltay, x,y, steep;
	int error, ystep;
	
	steep = abs(y1, y2) > abs(x1 ,x2);  

	if (steep)
	{
		swap(x1, y1);
		swap(x2, y2);
	}

	if (x1 > x2)
	{
		swap(x1, x2);
		swap(y1, y2);
	}

	deltax = x2 - x1;
	deltay = abs(y2, y1);  
	error = deltax / 2;
	y = y1;
	if(y1 < y2) ystep = 1;  else ystep = -1;

	for(x = x1; x <= x2; x++)
	{
		if (steep) writePixel(y,x,1); else writePixel(x,y,1);
   		error = error - deltay;
		if (error < 0)
		{
			y = y + ystep;
			error = error + deltax;
    	}
	}
}

//implementation of line drawing with color.
void DmDisplay::drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color)
{
	int deltax, deltay, x,y, steep;
	int error, ystep;
	
	steep = abs(y1, y2) > abs(x1 ,x2);  

	if (steep)
	{
		swap(x1, y1);
		swap(x2, y2);
	}

	if (x1 > x2)
	{
		swap(x1, x2);
		swap(y1, y2);
	}

	deltax = x2 - x1;
	deltay = abs(y2, y1);  
	error = deltax / 2;
	y = y1;
	if(y1 < y2) ystep = 1;  else ystep = -1;

	for(x = x1; x <= x2; x++)
	{
		if (steep) writePixel(y,x,color); else writePixel(x,y,color);
   		error = error - deltay;
		if (error < 0)
		{
			y = y + ystep;
			error = error + deltax;
    	}
	}
}
//drawing a rectangle
void DmDisplay::drawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
	//upper x line.
	drawLine(x,y, x+width, y);
	//lower x line.
	drawLine(x, y+height, x+width, y+height);
	//left y line.
	drawLine(x,y, x, y+height);
	//right y line.
	drawLine(x+width,y, x+width, y+height);
}
//draw a rectangle with color
void DmDisplay::drawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color)
{
	//upper x line.
	drawLine(x,y, x+width, y, color);
	//lower x line.
	drawLine(x, y+height, x+width, y+height, color);
	//left y line.
	drawLine(x,y, x, y+height, color);
	//right y line.
	drawLine(x+width,y, x+width, y+height, color);
}
//implementation of bresenham's circle
void DmDisplay::drawCircle(uint8_t x0, uint8_t y0, uint8_t radius)
{
	int error = 1 - radius;
	int errorY = 1;
	int errorX = -2 * radius;
	int x = radius, y = 0;
	
	writePixel(x0, y0 + radius, 1);
	writePixel(x0, y0 - radius, 1);
	writePixel(x0 + radius, y0, 1);
	writePixel(x0 - radius, y0, 1);
	
	while(y< x)
	{
		if(error > 0)
		{
			x--;
			errorX += 2;
			error += errorX;
		}
		y++;
		errorY += 2;
		error += errorY;
		
		writePixel(x0 + x, y0 + y, 1);
		writePixel(x0 - x, y0 + y, 1);
		writePixel(x0 + x, y0 - y, 1);
		writePixel(x0 - x, y0 - y, 1);
		writePixel(x0 + y, y0 + x, 1);
		writePixel(x0 - y, y0 + x, 1);
		writePixel(x0 + y, y0 - x, 1);
		writePixel(x0 - y, y0 - x, 1);
	}
}
//this function implements a circle with a color. draw visible or invisible circles .. 
void DmDisplay::drawCircle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t color)
{
	int error = 1 - radius;
	int errorY = 1;
	int errorX = -2 * radius;
	int x = radius, y = 0;
	
	writePixel(x0, y0 + radius, color);
	writePixel(x0, y0 - radius, color);
	writePixel(x0 + radius, y0, color);
	writePixel(x0 - radius, y0, color);
	
	while(y< x)
	{
		if(error > 0)
		{
			x--;
			errorX += 2;
			error += errorX;
		}
		y++;
		errorY += 2;
		error += errorY;
		
		writePixel(x0 + x, y0 + y, color);
		writePixel(x0 - x, y0 + y, color);
		writePixel(x0 + x, y0 - y, color);
		writePixel(x0 - x, y0 - y, color);
		writePixel(x0 + y, y0 + x, color);
		writePixel(x0 - y, y0 + x, color);
		writePixel(x0 + y, y0 - x, color);
		writePixel(x0 - y, y0 - x, color);
	}
}


//this thing fails big time. floating point computation on a 8 bit device ... so ugly.
void DmDisplay::drawArc(float x, float y, int r, float start_angle, float end_angle)
{
	// I know the question is tagged c++, but the idea comes clear in javascript
	
	float step = 0.1;
	for(float i = start_angle; i < end_angle; i = i + step)
	{
		float x0 = x+cos(i)*r; 
		float y0 = y+sin(i)*r;
		float x1 = x+cos(i+step)*r;
		float y1 = y+sin(i+step)*r;
		//writePixel(30 + cos(i) * r, 20 + sin(i) * r); // center point is (50,100)
		drawLine(x0, y0, x1, y1);
	}
}
//clears al the markers.
void DmDisplay::clearMarkers(void)
{
	for(int i = 0;i<7;i++)
		setMarker(i, 0);
}

//reads pixel data, on location set by setWriteReadAdress.
uint8_t DmDisplay::read()
{
	uint8_t data = 0;
	
	//enter read modify write.
	write(0xE0, INSTRUCT);
	
	//set port to input.
	DATAPORT_TO_INPUT;
	//clear input port to be sure.
	DATA_INPUT = 0;
	//set apropriate controlle lines. 
	SET_RW;
	SET_A0;
	//toggle enable twice. dummy read+actuall read.
	toggleEnable();
	_delay_us(10);
	toggleEnable();
	_delay_us(10);
	//read in the data.
	data = DATA_INPUT;
	//set the port back to output.
	DATAPORT_TO_OUTPUT;
	//write back data we read (datasheet dummy-read -> read -> write. (read modify write)).
	write(data, DATA);
	//leave read modify write.
	write(0xEE, INSTRUCT);
	return data;
}

//write a pixel.
void DmDisplay::writePixel(uint8_t x, uint8_t y, uint8_t color)
{
	//for holding existing pixel data.
	uint8_t pixelData = 0;
	
	if((x >= 100)||(y >= 48))
		return;
	if((x < 0) || (y < 0))
		return;
	
	//set read/write location.
	//also read pixel data in so we can OR it with the already existing data.
	setWriteReadAddres(x, y/8);
	pixelData = read();
	
	//determine what pixel in the pixel column to set.
	uint8_t pixelByteCol = (1<<(y%8));
	//write data and or it with that was already there.
	if(color)
	{
		write(pixelByteCol|pixelData, DATA);
	}
	else
	{
		write(~pixelByteCol&(pixelData), DATA);
	}
}

//toggles Enable
void DmDisplay::toggleEnable(void)
{
	SET_ENABLE;
	_delay_us(10);
	CLEAR_ENABLE;
	_delay_us(10);
	SET_ENABLE;
}

