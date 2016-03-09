#include "KRSI.h"


conFbyte PinAnalog       = A6;//A1;
conFbyte PinStrobe       = A3;//A0;
conFbyte PinReset        = 2;
conFbyte PinBussy        = A0;
conFbyte PinTombol       = A1;

KRSI::KRSI() {}

void KRSI::ComunicationInit()
{
  Serial.begin(38400);
}

void KRSI::EarInit()
{
  pinMode(PinAnalog, INPUT );
  pinMode(PinStrobe, OUTPUT);
  pinMode(PinReset , OUTPUT);
  analogReference(DEFAULT);
  HoldBass  = 0;
  HoldMidle = 0;
  HoldTrible = 0;
}

void KRSI::Hear()
{
  digitalWrite(PinReset, HIGH);//
  digitalWrite(PinReset, LOW);
  for ( byte i = 0; i < 8; i++ )
  {
    digitalWrite(PinStrobe, LOW);
    delayMicroseconds(36);
    spectrumValue[i] = analogRead(PinAnalog);
    digitalWrite(PinStrobe, HIGH);
    delayMicroseconds(18);
  }

  digitalWrite(PinReset, LOW);
  digitalWrite(PinReset, HIGH);
}

void KRSI::Listening()
{
  Bass = Midd = Tribl = 0;

  for (looping = 0; looping < 100; looping++)
  {
    Hear(); // mendengarkan suara :3
    Bass   += (( spectrumValue[1] ));
    Midd   += (( spectrumValue[2] + spectrumValue[3] ) / 2);
    Tribl  += (( spectrumValue[4] + spectrumValue[5] ) / 2);
  }
  Bass = Bass   / looping;
  Midd = Midd   / looping;
  Tribl = Tribl / looping;

}

void KRSI::setSoundThresh(int bass, int midle, int trible)
{
  HoldBass   = bass ;
  HoldMidle  = midle;
  HoldTrible = trible;

//  bmtHold    = bass + midle + trible;
}

boolean KRSI::ListenSongOrNot()
{
  KRSI::Listening();
  if (/*bmt > bmtHold*/ (Bass > HoldBass) || (Midd > HoldMidle) || (Tribl > HoldTrible)) return true;
  else return false;
}



boolean KRSI::Tombol(char t)
{
  switch(t)
  {
    case 'A':
      if((analogRead(PinTombol) > 490) && (analogRead(PinTombol) < 600) && !(analogRead(PinTombol) > 950) )
        return true;
      else
        return false;
      break;
    case 'B':
      if((analogRead(PinTombol) > 780) && (analogRead(PinTombol) < 920) && !(analogRead(PinTombol) > 950))
        return true;
      else
        return false;
      break;
    case 'C':
      if((analogRead(PinTombol) > 390) && (analogRead(PinTombol) < 500) && !(analogRead(PinTombol) > 950))
        return true;
      else
        return false;
      break;
    default:
        return false;
      break;
  }

}

boolean KRSI::Bussy()
{
  return (analogRead(PinBussy) > 512) ? true : false;
}

/* void ControllerInit();
 * 
 * prosedur ini digunakan untuk konfigurasi awal untuk keperluan komunikasi menggunakan Robovie
 *
 */
void KRSI::ControllerInit(){

  SPI.begin();
  Serial.println("SPI.begin");
  SPI.attachInterrupt(JoystickTransferHandler);
  Serial.println("SPI.attachInterupt");
  SPI.setBitOrder(LSBFIRST);
  Serial.println("SPI.LSBFIRST");
  SPI.setDataMode(SPI_MODE3);
  Serial.println("SPI.SPI_MODE3");
  SPI.setSPDR(0xFF);
  Serial.println("init SPDR with 0xFF");
  sei();
  Serial.println("Activate Interupt");
  JoystickInit();
}

/* void Press(byte dataKe, byte _dataIn);
 * 
 * prosedur ini digunakna untuk memberikan data tombol ke robovie. agar inputan parameter simple untuk di eksekusi
 * digunakan #define yang udah digunakan di atas. ada 2 byte untuk digunakan untuk memberikan data jadi ada 'byte dataKe'
 * cek pada protokol komunikasi joystick.
 * 
 * * parameter : datake  : data ke berapa tombol tersebut berada
 * *             _dataIn : data yang dikirim bagaimana
 * * return    : NULL
 *
 */
void KRSI::Press(byte dataKe, byte _dataIn){
  JoystickPress( dataKe,  _dataIn);
}

/* void Analog(byte dataKe, byte _dataIn);
 *
 * untuk mengirim data analog ke robovie
 *
 * * parameter : datake  : data ke berapa tombol tersebut berada
 * *             _dataIn : data yang dikirim bagaimana
 * * return    : NULL
 */
void KRSI::Analog(byte dataKe, byte _dataIn){
  JoystickAnalog( dataKe,  _dataIn);
}

/* void Release(byte dataKe, byte _dataIn);
 * 
 * seperti kita menggunakan joystick untuk menekan tombol dan tidak lupa untuk melepasnya kan ??
 * fungsi ini digunakan untuk melepas tombol yang sudah ditekan sebelumnya. parameternya harus sama
 * dengan pada fungsi press tadi yaa.
 * 
 * * parameter : datake  : data ke berapa tombol tersebut berada
 * *             _dataIn : data yang dikirim bagaimana
 * * return    : NULL 
 */
void KRSI::Release(byte dataKe, byte _dataIn){
  JoystickRelease(dataKe, _dataIn);
}

/* void ReleaseAll();
 *
 * sama seperti prosedur Release tapi ReleaseAll digunakan untuk melepas semua tombol yang sudah ditekan
 *
 */
void KRSI::ReleaseAll(){
  JoystickReleaseAll();
}


#ifdef DEBUG_MODE
void KRSI::ControlerDataView()
{
  #ifdef DEBUG_JOYSTICK
    Serial.println(F("===================================="));
    Serial.print(F("Joystick Debug Count :"));
    Serial.println(dataFalg);
    Serial.println(F("Data Diterima"));
    for (byte _i = 0; _i <= 8 ; _i++)
    {
      if (_i != 8) {
        Serial.print(dataInview[_i], HEX);
        Serial.print(F(" | "));
      }
      else {
        Serial.println(dataInview[_i], HEX);
      }
    }
    Serial.println(F("Data Dikirim"));
    Serial.print(0x01, HEX);
    Serial.print(F(" | "));
    Serial.print(0x42, HEX);
    Serial.print(F(" | "));
    Serial.print(0x00, HEX);
    Serial.print(F(" | "));
    for (byte _i = 3; _i <= 8 ; _i++)
    {
      if (_i != 8) {
        Serial.print(dataOutCmd[_i], HEX);
        Serial.print(F(" | "));
      }
      else {
        Serial.println(dataOutCmd[_i], HEX);
      }
    }
    Serial.println(F("===================================="));
  #else
    Serial.printLn(F("\n#Define DEBUG_JOYSTICK in JoystickClone.h before"))
  #endif
}

void KRSI::viewSoundData()
{
  Serial.print(F("B: "));
  Serial.print(Bass);
  Serial.print(F(" M: "));
  Serial.print(Midd);
  Serial.print(F(" T: "));
  Serial.println(Tribl);

}

void KRSI::eq_bug()
{
  Serial.print(F("Current values - "));
  
  Serial.print(F(" : "));
  Serial.print(spectrumValue[0]);
  
  Serial.print(F(" : "));
  Serial.print(spectrumValue[1]);

  Serial.print(F(" : "));
  Serial.print(spectrumValue[2]);

  Serial.print(F(" : "));
  Serial.print(spectrumValue[3]);

  Serial.print(F(" : "));
  Serial.print(spectrumValue[4]);

  Serial.print(F(" : "));
  Serial.print(spectrumValue[5]);

  Serial.print(F(" : "));
  Serial.print(spectrumValue[6]);

  Serial.println();   
}
#endif

