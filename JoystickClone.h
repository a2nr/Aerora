#ifndef JOYSTICKCLONE_H
#define JOYSTICKCLONE_H

#include "SPI_slave.h"

#define DEBUG_JOYSTICK

//===============================================
// for controler
typedef struct joyCmd_ts 
{
	byte dataKe ;
	byte _cmd ;
} PROGMEM const TSJoyCmd;


//byte4
TSJoyCmd  SELECT	= {0, 0x01};
TSJoyCmd  JOY_L 	= {0, 0x02};
TSJoyCmd  JOY_R 	= {0, 0x04};
TSJoyCmd  START 	= {0, 0x08};
TSJoyCmd  UP    	= {0, 0x10};
TSJoyCmd  RIGHT 	= {0, 0x20};
TSJoyCmd  DOWN  	= {0, 0x40};
TSJoyCmd  LEFT  	= {0, 0x80};
//byte5
TSJoyCmd  L2      	= {1, 0x01};
TSJoyCmd  R2      	= {1, 0x02};
TSJoyCmd  L1      	= {1, 0x04};
TSJoyCmd  R1      	= {1, 0x08};
TSJoyCmd  TRIANGEL	= {1, 0x10};
TSJoyCmd  CIRCLE  	= {1, 0x20};
TSJoyCmd  CROSS   	= {1, 0x40};
TSJoyCmd  SQUARE  	= {1, 0x80};
//analog
#define JOY_RX      2
#define JOY_RY      3
#define JOY_LX      4
#define JOY_LY      5

static volatile byte countCmd;
static volatile byte dataInCmd;
static volatile byte dataOutCmd[6];

#ifdef DEBUG_JOYSTICK
	static volatile byte dataInview[6];
#endif

void JoystickInit(void);
void JoystickPress(TSJoyCmd Command);
void JoystickRelease(TSJoyCmd Command);
void JoystickAnalog(byte dataKe, byte _dataIn);
void JoystickReleaseAll(void);
void JoystickTransferHandler(void);
boolean JoystickWaitToSend(void);



#endif