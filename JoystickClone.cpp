
#include "JoystickClone.h"

// byte    0     1     2     3     4     5     6     7      8
// CMD  => 01h   42h   00h   00h   00h   00h   00h   00h    00h
// Data <= 00h   Type  5Ah   Dat_0 Dat_1 Dat_2 Dat_3 Dat_4  Dat_5

void JoystickTransferHandler(void) {

  switch(dataInCmd ){
    case 0x00: // perlu di validasi benar 0x00 atau 0xFF
      
      if(countCmd == 0)
        break;//SPI.setSPDR(0xFF);

      if(countCmd > 1)
        SPI.setSPDR(dataOutCmd[ countCmd - 2 ]);
      
      break;
    case 0x42:
      SPI.setSPDR(0x5A);

      break;
    case 0x01:
      SPI.setSPDR(0x73);
      
      break;
  }
    
  dataInCmd = SPI.getSPDR();

  #ifdef DEBUG_JOYSTICK
    dataInview[countCmd] = dataInCmd;
  #endif
    
  if(countCmd < 8)
    countCmd ++;
  else
    countCmd = 0;
}


/*Controller disini digunakan untuk menggantikan joystick
 *dimana joystick untuk mengerakkan gerakan gerakan
 *yang sudah di program pada robovies
 */
void JoystickInit()
{

  SPI.begin();
  SPI.attachInterrupt(JoystickTransferHandler);
  SPI.setBitOrder(LSBFIRST);
  SPI.setDataMode(SPI_MODE3);
  SPI.setSPDR(0xFF);
  dataInCmd = 0x00;
  countCmd = 0;
  JoystickReleaseAll();
  sei();
}



/*KRSI::Press digunakan untuk menekan tombol
 *
 */
void JoystickPress(byte dataKe, byte _dataIn)
{

  dataOutCmd[dataKe] = (dataOutCmd[dataKe]) - (_dataIn);;

}

/*KRSI::Release digunakan untuk melepas tombol
 *
 */
void JoystickRelease(byte dataKe, byte _dataIn)
{

  dataOutCmd[dataKe] = (dataOutCmd[dataKe]) + (_dataIn);

}

/*KRSI::Analog digunakan untuk memberikan data analog
 *
 */
void JoystickAnalog(byte dataKe, byte _dataIn)
{
  if (dataKe > 1)
    dataOutCmd[dataKe] = _dataIn;

}

/*KRSI::ReleaseAll digunakan untuk melepas Semua tombol
 *
 */
void JoystickReleaseAll()
{
  dataOutCmd[0] = 0xFF; //dat 0
  dataOutCmd[1] = 0xFF;
  dataOutCmd[2] = 0x80;
  dataOutCmd[3] = 0x7F;
  dataOutCmd[4] = 0x80;
  dataOutCmd[5] = 0x7F;

}
