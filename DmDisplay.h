#ifndef DMDISPLAY_H
#define DMDISPLAY_H

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "font5x7.h"
#include "pinDefs.h"

#undef abs 
#define abs(x, y) 		((x>y) ?  (x-y) : (y-x))
#define swap(a, b) 		do{a = a+b;b = a - b;a = a-b;}while(0);

class DmDisplay
{
	public:
		//constants.
		static const uint8_t WIDTH = 100;
		static const uint8_t HEIGTH = 48; //width and height in pixels.
		static const uint8_t MAXCONTRAST = 32; //maximum contrast value.
		static const uint8_t BLACK = 1;
		static const uint8_t WHITE = 0;
		//function prototypes.
		DmDisplay(void);
		void init(void);
		void blinkControlLines(void);
		void write(uint8_t, uint8_t);
		void write(uint8_t);
		void setContrast(uint8_t);
		void setMarker(uint8_t, bool);
		void invertDisplay(bool);
		void setRow(uint8_t);
		void putChar(char);
		void putStr(const char *);
		void resetColumnAdress(void);
		void resetRowAdress(void);
		void setCol(uint8_t col);
		void toggleDisplayOnOff(bool);
		void setWriteReadAddres(uint8_t columnAddr, uint8_t page);
		void setCursor(uint8_t, uint8_t);
		void clear(void);
		void home(void);
		void clearMarkers(void);
		void toggleEnable(void);
		uint8_t read();
		//graphical functions.
		void writePixel(uint8_t, uint8_t, uint8_t);
		void drawArc(float, float, int, float, float);
		void drawCircle(uint8_t, uint8_t, uint8_t);
		void drawCircle(uint8_t, uint8_t, uint8_t, uint8_t);
		void drawLine(uint8_t, uint8_t, uint8_t, uint8_t);
		void drawLine(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
		void drawRect(uint8_t, uint8_t, uint8_t, uint8_t);
		void drawRect(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
		
		
		enum
		{
			INSTRUCT,DATA
		};
};

#include "DmDisplay.cpp"

#endif
