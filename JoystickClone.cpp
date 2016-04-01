
#include "JoystickClone.h"

// byte    0     1     2     3     4     5     6     7      8
// CMD  => 01h   42h   00h   00h   00h   00h   00h   00h    00h
// Data <= 00h   Type  5Ah   Dat_0 Dat_1 Dat_2 Dat_3 Dat_4  Dat_5

void JoystickTransferHandler(void){
  byte c = getDataReg();  // grab byte from SPI Data Register
  dataInCmd = c;

  switch(c){
    case 0x01:
      setDataReg(0x41); //0x73 kalo robovie
      dataFalg = 0x01;
      break;
    case 0x42:
      setDataReg(0x5A);
      countCmd = 0;
      break;
    case 0x00:
      setDataReg ( dataOutCmd[countCmd] );

      if (countCmd < 6) { 
        countCmd ++;
      } else {
        setDataReg(0x00);
        dataFalg = 0x00;
      }
      break;
    default:
      setDataReg(0x00);
      break;
  }
}


/*Controller disini digunakan untuk menggantikan joystick
 *dimana joystick untuk mengerakkan gerakan gerakan
 *yang sudah di program pada robovies
 */
void JoystickInit()
{

  SPCR |= bit (SPE);
  pinMode(MISO, OUTPUT);
  pinMode(SS,INPUT);
 
  SPI.setBitOrder(LSBFIRST);
  SPI.setDataMode(SPI_MODE3);
  SPI.attachInterrupt();

  // sei();
  dataInCmd = 0x00;
  countCmd = 0;
  JoystickReleaseAll();


  
}



/*KRSI::Press digunakan untuk menekan tombol
 *
 */
void JoystickPress(byte dataKe, byte _dataIn)
{

  dataOutCmd[dataKe] = (dataOutCmd[dataKe]) - (_dataIn);
  while (dataFalg == 0x01); //wait until data send

}

/*KRSI::Release digunakan untuk melepas tombol
 *
 */
void JoystickRelease(byte dataKe, byte _dataIn)
{

  dataOutCmd[dataKe] = (dataOutCmd[dataKe]) + (_dataIn);
  while (dataFalg == 0x01); //wait until data send

}

/*KRSI::Analog digunakan untuk memberikan data analog
 *
 */
void JoystickAnalog(byte dataKe, byte _dataIn)
{
  if (dataKe > 1)
    dataOutCmd[dataKe] = _dataIn;
  while (dataFalg == 0x01); //wait until data send

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
ISR (SPI_STC_vect) {
  JoystickTransferHandler();
}