
#include "JoystickClone.h"

// byte    0     1     2     3     4     5     6     7      8
// CMD  => 01h   42h   00h   00h   00h   00h   00h   00h    00h
// Data <= 00h   Type  5Ah   Dat_0 Dat_1 Dat_2 Dat_3 Dat_4  Dat_5

void JoystickTransferHandler(void){
  byte c = getDataReg();  // grab byte from SPI Data Register
  
  switch(c){
    case 0x01:
      setDataReg(0x73); //0x73 kalo robovie //0x41 joystick biasa
      dataFalg = 0x00;
      countCmd = 0;
      break;
    case 0x42:
      setDataReg(0x5A);
      countCmd = 0;
      break;
    case 0x00: // lek gak bisaganti 0xff
      if (countCmd < 6) { 
        setDataReg ( dataOutCmd[countCmd] );
        countCmd ++;
      } else {
        setDataReg(0xFF);
        dataFalg = 0x01;
      }
      break;
    default:
      setDataReg(0xFF);
      break;
  }

  #ifdef DEBUG_JOYSTICK
  switch(c){
    case 0x01:
        dataInview[0] = c;
      break;
    case 0x42:
        dataInview[1] = c;
      break;
    case 0x00: // lek gak bisaganti 0xff
        dataInview[countCmd + 1] = c;
      break;
  }
  #endif
  
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
  countCmd = 0;
  JoystickReleaseAll();
  #ifndef DEBUG_JOYSTICK
    Serial.println(F("\n#Define DEBUG_JOYSTICK in JoystickClone.h before"));
  #endif

  
}



/*KRSI::Press digunakan untuk menekan tombol
 *
 */
void JoystickPress(byte dataKe, byte _dataIn)
{

  dataOutCmd[dataKe] = (dataOutCmd[dataKe]) & (~_dataIn);
  while (dataFalg == 0x01); //wait until data send
  #ifdef DEBUG_JOYSTICK
    Serial.println(F("===================================="));
    for (byte _i = 0; _i < 8 ; _i++)
    {
      if (_i != 7) {
        Serial.print(dataInview[_i], HEX);
        Serial.print(F(" | "));
      }
      else {
        Serial.println(dataInview[_i], HEX);
      }
    }

  #endif
}

/*KRSI::Release digunakan untuk melepas tombol
 *
 */
void JoystickRelease(byte dataKe, byte _dataIn)
{

  dataOutCmd[dataKe] = (dataOutCmd[dataKe]) | (_dataIn);
  while (dataFalg == 0x01); //wait until data send
  #ifdef DEBUG_JOYSTICK
    Serial.println(F("===================================="));
    for (byte _i = 0; _i < 8 ; _i++)
    {
      if (_i != 7) {
        Serial.print(dataInview[_i], HEX);
        Serial.print(F(" | "));
      }
      else {
        Serial.println(dataInview[_i], HEX);
      }
    }

  #endif
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