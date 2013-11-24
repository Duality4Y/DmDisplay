#ifndef DMDISPLAY_H
#define DMDISPLAY_H

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "font5x7.h"
#include "pinDefs.h"

class DmDisplay
{
	public:
		DmDisplay();
		void init();
		void blinkControlLines();
		void write(uint8_t, uint8_t);
		void write(uint8_t);
		void setContrast(uint8_t);
		void setMarker(uint8_t, bool);
		void invertDisplay(bool);
		void setRow(uint8_t);
		void lcdChar(const char *);
		void resetColumnAdress();
		void setCol(uint8_t col);
		void toggleDisplayOnOff(bool);
		void setWriteAddress(uint8_t columnAddr, uint8_t page);
		void setCursor(uint8_t, uint8_t);
		void clear();
		enum
		{
			INSTRUCT,DATA
		};
};

#include "DmDisplay.cpp"

#endif
