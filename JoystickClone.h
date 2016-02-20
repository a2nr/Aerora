#ifndef JOYSTICKCLONE_H
#define JOYSTICKCLONE_H

#include "SPI_slave.h"

#undef DEBUG_JOYSTICK

//byte4
#define SELECT      0,0x01
#define JOY_L       0,0x02
#define JOY_R       0,0x04
#define START       0,0x08
#define UP          0,0x10
#define RIGHT       0,0x20
#define DOWN        0,0x40
#define LEFT        0,0x80

//byte5
#define L2          1,0x01
#define R2          1,0x02
#define L1          1,0x04
#define R1          1,0x08
#define TRIANGEL    1,0x10
#define CIRCLE      1,0x20
#define CROSS       1,0x40
#define SQUARE      1,0x80

//analog
#define JOY_RX      2
#define JOY_RY      3
#define JOY_LX      4
#define JOY_LY      5

//===============================================
// for controler
static volatile byte countCmd;
static volatile byte dataInCmd;
static volatile byte dataOutCmd[6];

#ifdef DEBUG_JOYSTICK
	static volatile byte dataInview[6];
#endif

void JoystickInit(void);
void JoystickPress(byte dataKe, byte _dataIn);
void JoystickRelease(byte dataKe, byte _dataIn);
void JoystickAnalog(byte dataKe, byte _dataIn);
void JoystickReleaseAll(void);
void JoystickTransferHandler(void);



#endif