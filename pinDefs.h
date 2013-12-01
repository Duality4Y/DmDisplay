#ifndef PINDEFS_H
#define PINDEFS_H

/*
 * using a atmega16
 * with a glcd, this display: http://tkkrlab.nl/wiki/Glcd_48x100
 * 
 * A0, Read/Write and Enable are 
 * connected to port D
 * 
 * A0 is connected to: PD6
 * R/W is connected to: PD5
 * Enable is connected to: PD4
 *
 * Start of defines for easy line controle
 * This wil spare me a lot of typing :)
 * Also this wil make it more portable between AVR's.
 * although if you want data to be on different ports,
 * it will be a little coding with defines,
 * but I think you should be able to do it.
 * 
 * keeping it abstract like this will make it easier to port?
 * 
 * other ideas about this or suggestions ?
 * send them to:
 * 201292@live.nl
 * 
 * or on irc:
 * channel: #tkkrlab
 * nick:    Duality
 * 
 * */
 
#define A0 						_BV(PD6)
#define RW 						_BV(PD5)
#define ENABLE 					_BV(PD4)
#define LCD_POWER 				_BV(PD3)

#define FUNCTIONPORT 			PORTD
#define FUNCTIONDIR 			(DDRD)
#define FUNCTIONPORT_TO_OUTPUT	(FUNCTIONDIR |=  (A0|RW|ENABLE|LCD_POWER))
#define FUNCTIONPORT_TO_INPUT 	(FUNCTIONDIR &= ~(A0|RW|ENABLE|LCD_POWER))

#define SET_A0 					(FUNCTIONPORT |= A0)
#define SET_RW 					(FUNCTIONPORT |= RW)
#define SET_ENABLE 				(FUNCTIONPORT |= ENABLE) 
#define SET_POWER				(FUNCTIONPORT |= LCD_POWER)

#define CLEAR_A0				(FUNCTIONPORT &= ~(A0))
#define CLEAR_RW				(FUNCTIONPORT &= ~(RW))
#define CLEAR_ENABLE			(FUNCTIONPORT &= ~(ENABLE))
#define CLEAR_POWER				(FUNCTIONPORT &= ~(LCD_POWER))

#define DATA_PORT 				PORTC
#define DATA_DIR 				(DDRC)
#define DATAPORT_TO_OUTPUT 		(DATA_DIR = 0xFF)
#define DATAPORT_TO_INPUT		(DATA_DIR = 0x00)

#endif
