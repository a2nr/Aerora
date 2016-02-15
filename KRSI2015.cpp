#include "KRSI2015.h"

conFbyte PinAnalog       = A6;//A1;
conFbyte PinStrobe       = A3;//A0;
conFbyte PinReset        = 2;
conFbyte PniBussy        = A0;
conFbyte PinTombol       = A1;

#define JOYS_DEBUG

KRSI::KRSI() {}

void KRSI::ComunicationInit()
{
  Serial.begin(9600);
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

  //  bmt = Bass + Midd + Tribl;

  //bmt= (Bass + Midd + Tribl)/3;
/*  for(looping = 0; looping < 100; looping++)
  {
    Hear();
    BassArray[looping]  = ((spectrumValue[1] + spectrumValue[2] + spectrumValue[3])/3);
    MidArray [looping]  = ((spectrumValue[3] + spectrumValue[4])/2);
    TribArray[looping]  =(spectrumValue[5] + spectrumValue[6]) /2;
    
    if(looping < 1)
     {
        Bass = BassArray[looping];
        Midd = MidArray [looping];
        Tribl= TribArray[looping];
     }
     else
     {
        Bass = Bass + BassArray[looping];
        Midd = Midd + MidArray [looping];
        Tribl= Tribl+ TribArray[looping];
     }
  }
  Bass = Bass / looping;
  Midd = Midd / looping;
  Tribl= Tribl/ looping; */ 
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


/*Controller disini digunakan untuk menggantikan joystick
 *dimana joystick untuk mengerakkan gerakan gerakan
 *yang sudah di program pada robovie
 */
void KRSI::ControllerInit()
{
  SPI.setBitOrder(LSBFIRST);
  SPI.setDataMode(SPI_MODE3);
  dataTemp[0] = 0xFF; //byte 0
  dataTemp[1] = 0xFF; //byte 1
  dataTemp[2] = 0x80;
  dataTemp[3] = 0x7F;
  dataTemp[4] = 0x80;
  dataTemp[5] = 0x7F;
  pinMode(PinBussy, INPUT);

}

/*KRSI::Press digunakan untuk menekan tombol
 *
 */
void KRSI::Press(byte dataKe, byte _dataIn)
{
/*
#ifdef JOYS_DEBUG
  Serial.println(dataTemp[dataKe]); 
#endif */ 

  dataTemp[dataKe] = (dataTemp[dataKe]) & ~(_dataIn);;

#ifdef JOYS_DEBUG
  Serial.println(dataTemp[dataKe]); 
#endif  
   

  if (dataKe == 0 || dataKe == 1)
  {

    SPI.begin();
    //header joystick
    /*Byte[0] = */SPI.transfer(0xFF);
    /*Byte[1] = */SPI.transfer(0x73);
    SPI.transfer(0x5A);
    //end header

    //start digital
    /*Byte[2] = */SPI.transfer(dataTemp[0]);
    /*Byte[3] = */SPI.transfer(dataTemp[1]);
    //end digital

    //start analog
    /*Byte[4] = */SPI.transfer(dataTemp[2]);
    /*Byte[5] = */SPI.transfer(dataTemp[3]);
    /*Byte[6] = */SPI.transfer(dataTemp[4]);
    /*Byte[7] = */SPI.transfer(dataTemp[5]);
    //end analog
    SPI.end();

  }
}

/*KRSI::Release digunakan untuk melepas tombol
 *
 */
void KRSI::Release(byte dataKe, byte _dataIn)
{
  dataTemp[dataKe] |= _dataIn;

  byte temp = (dataTemp[dataKe]) | (_dataIn);

  dataTemp[dataKe] = temp;

  if (dataKe == 0 || dataKe == 1)
  {
    SPI.begin();
    //header joystick
    /*Byte[0] = */SPI.transfer(0xFF);
    /*Byte[1] = */SPI.transfer(0x73);
    SPI.transfer(0x5A);
    //end header
    //start digital
    /*Byte[2] = */SPI.transfer(dataTemp[0]);
    /*Byte[3] = */SPI.transfer(dataTemp[1]);
    //end digital
    //start analog
    /*Byte[4] = */SPI.transfer(dataTemp[2]);
    /*Byte[5] = */SPI.transfer(dataTemp[3]);
    /*Byte[6] = */SPI.transfer(dataTemp[4]);
    /*Byte[7] = */SPI.transfer(dataTemp[5]);
    //end analog
    SPI.end();
  }
}

/*KRSI::Analog digunakan untuk memberikan data analog
 *
 */
void KRSI::Analog(byte dataKe, byte _dataIn)
{

  dataTemp[dataKe] = _dataIn;
  if (dataKe == 2 || dataKe == 3 || dataKe == 4 || dataKe == 5)
  {
    //header joystick
    SPI.begin();
    //header joystick
    /*Byte[0] = */SPI.transfer(0xFF);
    /*Byte[1] = */SPI.transfer(0x73);
    SPI.transfer(0x5A);
    //end header
    //start digital
    /*Byte[2] = */SPI.transfer(dataTemp[0]);
    /*Byte[3] = */SPI.transfer(dataTemp[1]);
    //end digital
    //start analog
    /*Byte[4] = */SPI.transfer(dataTemp[2]);
    /*Byte[5] = */SPI.transfer(dataTemp[3]);
    /*Byte[6] = */SPI.transfer(dataTemp[4]);
    /*Byte[7] = */SPI.transfer(dataTemp[5]);
    //end analog
    SPI.end();
  }
}

/*KRSI::ReleaseAll digunakan untuk melepas Semua tombol
 *
 */
void KRSI::ReleaseAll()
{
  dataTemp[0] = 0xFF;
  dataTemp[1] = 0xFF;
  dataTemp[2] = 0x80;
  dataTemp[3] = 0x7F;
  dataTemp[4] = 0x80;
  dataTemp[5] = 0x7F;

  //header joystick
  SPI.begin();
  //header joystick
  /*Byte[0] = */SPI.transfer(0xFF);
  /*Byte[1] = */SPI.transfer(0x73);
  SPI.transfer(0x5A);
  //end header
  //start digital
  /*Byte[2] = */SPI.transfer(dataTemp[0]);
  /*Byte[3] = */SPI.transfer(dataTemp[1]);
  //end digital
  //start analog
  /*Byte[4] = */SPI.transfer(dataTemp[2]);
  /*Byte[5] = */SPI.transfer(dataTemp[3]);
  /*Byte[6] = */SPI.transfer(dataTemp[4]);
  /*Byte[7] = */SPI.transfer(dataTemp[5]);
  //end analog
  SPI.end();
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
  if (analogRead(PinBussy) > 512) return true;
  else return false;
}

/*void KRSI::ControlerDataView()
{
  Serial.println(F("===================================="));
  Serial.println(F("Joystick Debug"));
  for (byte _i = 0; _i <= 8 ; _i++)
  {
    if (_i != 8) {
      Serial.print(Byte[_i], HEX);
      Serial.print(F(" | "));
    }
    else {
      Serial.println(Byte[_i], HEX);
    }
  }
  Serial.println(F("===================================="));
}*/
