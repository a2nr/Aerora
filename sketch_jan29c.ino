#include "KRSI2015.h"
#include <EEPROM.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include <string.h>

//#define SOUND_DEBUG
#define USING_EEPROM
//#define BLUETOOTH_DEBUG
#define RUNING_LCD

///////////////////////////////////////////////////////////////////
//////////////////////////STRATEGI PERLOMBAAN//////////////////////
///////////////////////////////////////////////////////////////////
#define USING_AERORA
//#define USING_AERORI
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
#define USING_TRIAL_SATU
//#define USING_TRIAL_DUA // HANYA UNTUK AERORA
//#define USING_TRIAL_TIGA

///////////////////////////////////////////////////////////////////
//////////////// NOTE PERLOMBAAN //////////////////////////////////
///////////////////////////////////////////////////////////////////
/*
 *JANGAN SALAH MROGRAM...
 */
 
 /*
  * Sensor suwara kacau :'(
  */

///////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////
///////////////////////////// LCD /////////////////////////////////
///////////////////////////////////////////////////////////////////
#ifdef RUNING_LCD
    LiquidCrystal_I2C lcd(0x27, 16, 2);  //lcd_i2c
    char buff[16];
#endif //RUNING_LCD
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
////////////// ALAMAT BLUETOOTH PADA AERORI ///////////////////////
////////////// AERORA MENJADI SLAVE ///////////////////////////////
////////////// bluetooth di sini hanyak untuk komunikasi //////////
////////////// antar robot ////////////////////////////////////////
///////////////////////////////////////////////////////////////////
////////////// nama bluetooth /////////////////////////////////////
////////////// AERORA = 65 45 52 4F 52 | 65 ///////////////////////
////////////// AERORI = 65 45 52 4F 52 | 73 ///////////////////////
////////////// KRSI15 = 48 52 53 73 49 53 /////////////////////////
///////////////////////////////////////////////////////////////////

SoftwareSerial BT(9, 8); // rx tx // CROSS
boolean isConnect = false;
#ifdef USING_AERORA
//////////////////////////////////////////////////////////////////
////////////// Ini adalah alamat blutooth ////////////////////////
////////////// baca datasheet lebih lengkapnya ///////////////////
//////////////////////////////////////////////////////////////////
    #define AERORI      "98d3,31,401839" 
#endif

#ifdef USING_AERORA
    #undef AERORA      "14,3,123882"
#endif // USING_AERORI
///////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
////////////// ALAMAT EEPROM BUAT PENYIMPANANTRESH HOLD ///////////
///////////////////////////////////////////////////////////////////
conFbyte BASS_ADD  =  0x00;
conFbyte MIDL_ADD  =  0x01;
conFbyte TRIB_ADD  =  0x02;
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//////////// BUAT TOMBOLL /////////////////////////////////////////
///////////////////////////////////////////////////////////////////
conFchar tom_A     = 'A';
conFchar tom_B     = 'B';
conFchar tom_C     = 'C';
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
KRSI Robot; /// PROTOTIPE ROBOT////////////////////////////////////
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
/// ini dipergunakan untuk ngetes komunikasi antara blutooth //////
/// dengan arduino //////////////////////////////////////////////// 
#ifdef BLUETOOTH_DEBUG
void settingHc05()
{
  while(BT.available())
    Serial.write(BT.read());
  while(Serial.available())
    BT.write(Serial.read());
//    delay(1000);
}
#endif // BLUETOOTH_DEBUG


char btBuff;
void writeBT(char x){

    if(isConnect) BT.write(x);

}

void readBT(char x){
    if(isConnect){

        btBuff = x;
        while(!(BT.read() == x));
    }
  
}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
/////////////////// PROGRAM UTAMA /////////////////////////////////
///////////////////////////////////////////////////////////////////
void setup ()
{
  BT.begin(38400);

  Robot.ComunicationInit(); // init uart
  Robot.ControllerInit(); // init robovie kontroller
  Robot.ReleaseAll(); // reset state tombol
  Robot.EarInit(); // init msgeq
  


  kalibrasi();

#ifdef RUNING_LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
#endif //RUNING_LCD 

#ifdef USING_AERORA
 if(Robot.Tombol(tom_A))
    ConnectSlave();
#endif

  if(Robot.Tombol(tom_C))
    cek_sound();

 #ifdef USING_AERORI
   RoboStart();
//   lcd.setCursor(0,0);
//   lcd.print(F("Not Connect"));
//   while(!(BT.read() == 'S'));
 #endif

    lcd.setCursor(0,0);
    sprintf(buff,"Menarilah :)|%d",isConnect);
    lcd.print(buff);
    lcd.setCursor(0,1);
    sprintf(buff,"B:%3dM:%3dT:%3d",Robot.HoldBass
    ,Robot.HoldMidle
    ,Robot.HoldTrible);
    lcd.print(buff);

  #ifdef USING_AERORA
    while (!Robot.Tombol(tom_A)) {
  //      writeBT('S');
//        if(BT.read() == 'B') isConnect = true;
    }
       
  RoboStart();
  #endif

  delay(1000);

  #ifdef RUNING_LCD

  lcd.clear();
  lcd.setCursor(6,0);
  lcd.print(F("Bussy:"));
  lcd.setCursor(6,1);
  lcd.print(F("Suara:"));

  #endif
  

  #ifdef USING_AERORI
  
  for(int pose = 0; pose < 3; pose++)
  {

    pose_ke(pose);

  }
  
  #endif
  
  // lcd.clear();
  // lcd.print("Selesai >.<");

}

void loop()
{
#ifdef USING_AERORA
  for(int pose = 0; pose < 24; pose++)
  {
    pose_ke(pose);
  }
#endif

#ifdef BLUETOOTH_DEBUG
  settingHc05();
#endif
}



/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

void waitBTsignal(byte dataKe, byte _dataIn, char x){
  //Robot.Press(dataKe,_dataIn);
  //Robot.Press(L1);
    if(isConnect){

      btBuff = x;
      while(!(BT.read() == x))
      {
        Robot.Press(dataKe,_dataIn);
        Robot.Press(L1);
      }
    }

}

void pose_ke(int pose)
{

#ifdef RUNING_LCD

  lcd.setCursor(0,0);
  sprintf(buff,"%d:%c",pose,btBuff);
  lcd.print(buff);

#endif

#ifdef USING_AERORI

  #ifdef USING_TRIAL_SATU

  switch(pose){
    case 0: 
    readBT('0'); //NUNGGU BAMBANG
    RoboRun(UP);
    break;
    case 1:
    RoboRun(RIGHT);
    break;
    case 2:
    RoboRun(DOWN);
    break;
    case 3:
    RoboRun(LEFT);
    break;
    case 4:
    RoboRun(CROSS);
    break;
    case 5:
    RoboRun(CIRCLE);
    break;
    case 6:
    RoboRun(TRIANGEL);
    break;
    case 7:
    RoboRun(UP);
    break;
    case 8:
    RoboRun(RIGHT);
    break;
    case 9:
    RoboRun(DOWN);
    break;
    case 10:
    RoboRun(LEFT); //LONELY AKHIR
    writeBT('X');
    break;

    case 11:
    readBT('1');
    RoboRun(CROSS); 
    writeBT('X');
    break;

    case 12:
    readBT('2');
    RoboRun(CIRCLE); 
    writeBT('X');
    break;

    case 13:
    readBT('3');
    RoboRun(TRIANGEL); 
    writeBT('X');
    break;

    case 14:
    readBT('4');
    RoboRun(SQUARE); 
    writeBT('X');
    break;

    case 15:
    readBT('5');
    RoboRun(UP); 
    waitBTsignal(UP,'X');
    break;

    case 16:
    readBT('6');
    RoboRun(RIGHT);
    waitBTsignal(RIGHT, 'X');
    break;

    case 17:
    readBT('7');
    RoboRun(DOWN);
    waitBTsignal(DOWN, 'X');
    break;

    case 18:
    readBT('8');
    RoboRun(LEFT );
    waitBTsignal(LEFT, 'X');
    break;

    case 19:
    readBT('9');
    RoboRun(CROSS);
    writeBT('X');
    break;

    case 20:
    readBT('Q');
    RoboRun(CIRCLE);
    writeBT('X');
    break;

    case 21:
    readBT('W');
    RoboRun(TRIANGEL);
    writeBT('X');
    break;

    case 22:
    readBT('A');
    RoboRun(UP);
    writeBT('X');
    break;

    case 23:
    readBT('E');
    RoboRun(DOWN);
    break;

    default:
    Robot.Press(L1);
    break;
  }

  #endif

  #ifdef USING_TRIAL_TIGA

  switch(pose){
    case 0: 
    readBT('0'); //NUNGGU BAMBANG
    RoboRun(R2);
    RoboRun(UP);
    break;
    case 1:
    RoboRun(RIGHT);
    break;
    case 2:
    RoboRun(DOWN);
    break;
    case 3:
    RoboRun(LEFT);
    RoboRun(L2);
    break;
    case 4:
    RoboRun(CROSS);
    break;
    case 5:
    RoboRun(CIRCLE);
    break;
    case 6:
    RoboRun(TRIANGEL);
    break;
    case 7:
    RoboRun(UP);
    break;
    case 8:
    RoboRun(RIGHT);
    break;
    case 9:
    RoboRun(DOWN);
    break;
    case 10:
    RoboRun(LEFT); //berakhirnya kesendirian
    break;

    case 11:
    readBT('1');
    RoboRun(CROSS); 
    writeBT('X');
    waitBTsignal(CROSS, 'X');

    break;

    case 12:
    readBT('2');
    RoboRun(JOY_R);
    writeBT('X');
    break;

    case 13:
    readBT('3');
    RoboRun(TRIANGEL); 
    writeBT('X');
    break;

    case 14:
    readBT('4');
    RoboRun(SQUARE); 
    writeBT('X');
    break;

    case 15:
    readBT('5');
    RoboRun(UP); 
    waitBTsignal(UP,'X');
    break;

    case 16:
    readBT('6');
    RoboRun(RIGHT);
    waitBTsignal(RIGHT, 'X');
    break;

    case 17:
    readBT('7');
    RoboRun(DOWN);
    waitBTsignal(DOWN, 'X');
    break;

    case 18:
    readBT('8');
    RoboRun(LEFT );
    waitBTsignal(LEFT, 'X');
    break;

    case 19:
    readBT('9');
    RoboRun(CROSS);
    writeBT('X');
    break;

    case 20:
    readBT('Q');
    RoboRun(CIRCLE);
    writeBT('X');
    break;

    case 21:
    readBT('W');
    RoboRun(TRIANGEL);
    writeBT('X');
    break;

    case 22:
    readBT('A');
    RoboRun(UP);
    writeBT('X');
    break;

    case 23:
    readBT('E');
    RoboRun(DOWN);
    break;

    default:
    Robot.Press(L1);
    break;
  }

  #endif
#endif

#ifdef USING_AERORA
  /////////////////////////////////////////////////////////////////////////////////////////
  #ifdef USING_TRIAL_SATU

  switch(pose){
    case 0:
    delay(3500);
    RoboRun(RIGHT);
    break;
    case 1:
    RoboRun(CIRCLE);
    break;
    case 2:
    RoboRun(R2);
    break;
    case 3:
    RoboRun(JOY_R);
    break;
    case 4:
    RoboRun(SQUARE);
    break;
    case 5:
    RoboRun(CROSS);
    break;
    case 6:
    RoboRun(R1);
    break;
    case 7:
    RoboRun(R1);
    break;
    case 8:
    RoboRun(R1);
    break;
    case 9:
    RoboRun(UP);
    break;
    case 10:
    RoboRun(LEFT);
    break;
    case 11:
    RoboRun(JOY_R);
    break;  
    case 12:
    RoboRun(RIGHT);
    break;
    case 13:
    RoboRun(SQUARE); 
    break; 
    case 14:
    RoboRun(CROSS);
    break; 
    case 15:
    RoboRun(CIRCLE);
    break;
    case 16:
    RoboRun(DOWN);
    break;    
    case 17:
    RoboRun(SQUARE);
    break; 
    case 18:
    RoboRun(R2);
    break; 
    case 19:
    RoboRun(CIRCLE);
    break; 
    case 20:
    RoboRun(RIGHT);
    break; 
    case 21:
    RoboRun(CROSS);
    break; 
    case 22:
    RoboRun(SQUARE);
    break; 
    case 23:
    RoboRun(R2);
    break; 
    case 24:
    RoboRun(JOY_R);
    break;
    default:
    Robot.Press(DOWN);
    Robot.Press(L1);
    break;}

  #endif

   /////////////////////////////////////////////////////////////////////////////////////////
  #ifdef USING_TRIAL_DUA

  switch(pose){
    case 0:
    RoboRun(RIGHT);
    break;
    case 1:
    RoboRun(TRIANGEL);
    break;
    case 2:
    RoboRun(CROSS);
    break;
    case 3:
    RoboRun(SQUARE);
    break;
    case 4:
    RoboRun(R2);
    break;
    case 5:
    RoboRun(TRIANGEL);
    break;
    case 6:
    RoboRun(CROSS);
    break;
    case 7:
    RoboRun(SQUARE);
    break;
    
    case 8:
    RoboRun(R1); //BERTAPA
    break;

    case 9:
    RoboRun(JOY_L);
    break;

    case 10:
    RoboRun(LEFT);
    break;
    
    case 11:

    RoboRun(JOY_L);
    break;  

    case 12:
    RoboRun(DOWN);
    break;

    case 13:
    RoboRun(LEFT); //CAKIL MAIN
                   //NUNGGU GERAKAN CAKIL SELESAI
    waitBTsignal(LEFT,'X');

    break; 
    case 14:
    writeBT('6');
    RoboRun(DOWN);
    break; 
    case 15:
    RoboRun(TRIANGEL);
    writeBT('X');
    break;
    case 16:
    writeBT('7');
    RoboRun(CIRCLE);
    break;    
    case 17:
    RoboRun(CROSS);
    writeBT('X');
    break; 
    case 18:
    writeBT('8');
    RoboRun(JOY_L);
    break; 
    case 19:
    RoboRun(R1);
    break; 
    case 20:
    RoboRun(R2);
    writeBT('X');
    break; 

    case 21:
    writeBT('9');
    RoboRun(R2);
    waitBTsignal(R2,'X');
    break; 

    case 22:
    writeBT('Q');
    RoboRun(UP);
    waitBTsignal(UP,'X');
    break; 

    case 23:
    writeBT('W');
    RoboRun(RIGHT);
    waitBTsignal(RIGHT,'X');
    break; 

    case 24:
    writeBT('A');
    RoboRun(UP);
    waitBTsignal(UP,'X');
    break;

    case 25:
    RoboRun(RIGHT);
    writeBT('E');
    break;

    case 26:
    RoboRun(UP);
    break;

    case 27:
    RoboRun(RIGHT);
    break;

    case 28:
    RoboRun(DOWN);
    break;

    default:
    Robot.Press(DOWN);
    Robot.Press(L1);
    break;
  }

  #endif

  /////////////////////////////////////////////////////////////////////////////////////
  #ifdef USING_TRIAL_TIGA
  switch(pose){
    case 0:
    RoboRun(RIGHT);
    break;
    case 1:
    RoboRun(R2);
    break;
    case 2:
    RoboRun(JOY_R);
    break;
    case 3:
    RoboRun(TRIANGEL);
    break;
    case 4:
    RoboRun(CROSS);
    break;
    case 5:
    RoboRun(CROSS);
    break;
    case 6:
    RoboRun(SQUARE);
    break;
    case 7:
    RoboRun(UP);
    break;
    
    case 8:
    RoboRun(R1); //BERTAPA
    writeBT('0');
    waitBTsignal(R1,'X'); // NUNGGU CAKIL BERHENTI NARI
    break;

    case 9:
    writeBT('1');
    RoboRun(JOY_L);
    waitBTsignal(JOY_L,'X');
    break;

    case 10:
    writeBT('2');
    RoboRun(LEFT);
    waitBTsignal(LEFT,'X');
    break;
    
    case 11:
    writeBT('3');
    RoboRun(JOY_L);
    waitBTsignal(JOY_L,'X');
    break;  

    case 12:
    writeBT('4');
    RoboRun(DOWN);
    waitBTsignal(DOWN,'X');
    break;

    case 13:
    writeBT('5');
    RoboRun(LEFT); //CAKIL MAIN
                   //NUNGGU GERAKAN CAKIL SELESAI
    waitBTsignal(LEFT,'X');

    break; 
    case 14:
    writeBT('6');
    RoboRun(DOWN);
    break; 
    case 15:
    RoboRun(TRIANGEL);
    writeBT('X');
    break;
    case 16:
    writeBT('7');
    RoboRun(CIRCLE);
    break;    
    case 17:
    RoboRun(CROSS);
    writeBT('X');
    break; 
    case 18:
    writeBT('8');
    RoboRun(JOY_L);
    break; 
    case 19:
    RoboRun(R1);
    break; 
    case 20:
    RoboRun(R2);
    writeBT('X');
    break; 

    case 21:
    writeBT('9');
    RoboRun(R2);
    waitBTsignal(R2,'X');
    break; 

    case 22:
    writeBT('Q');
    RoboRun(UP);
    waitBTsignal(UP,'X');
    break; 

    case 23:
    writeBT('W');
    RoboRun(RIGHT);
    waitBTsignal(RIGHT,'X');
    break; 

    case 24:
    RoboRun(DOWN);
    break;

    default:
    Robot.Press(DOWN);
    Robot.Press(L1);
    break;}

  #endif
#endif
   
}

#ifdef USING_AERORA

void ConnectSlave(){


  int count;
  memset(buff,0,sizeof(buff));
  lcd.setCursor(0,0);
  
  #ifdef BLUETOOTH_DEBUG
  Serial.print(F("Connect Slave"));
  #endif

  #ifdef RUNING_LCD
  lcd.print(F("Connect Slave"));
  
  delay(500);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("AT+INIT ")); //inisialisasi 
  #endif

  memset(buff,0,sizeof(buff));
  
  BT.write("AT+RMAAD");
  BT.write(ENTER);
  delay(500);

  BT.write("AT+INIT");
  BT.write(ENTER);
  count = 0;
 delay(500);
  while(BT.available()) {

    buff[count]=BT.read();

    #ifdef BLUETOOTH_DEBUG
    Serial.write(buff[count]);
    #endif

    #ifdef RUNING_LCD
    lcd.setCursor(0,1);
    lcd.print(buff[count]);
    #endif

    count++;      

  }

  delay(500);
  
  #ifdef  RUNING_LCD

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("AT+FSAD "));
  
  #endif

  memset(buff,0,sizeof(buff));

  BT.write("AT+FSAD="); // cari alamat pair
  BT.write(AERORI);
  BT.write(ENTER);
  count = 0; 
  delay(500); 
  lcd.setCursor(0,1);
  while(BT.available()) {

    buff[count]=BT.read();

    #ifdef BLUETOOTH_DEBUG
    Serial.write(buff[count]);
    #endif

    #ifdef  RUNING_LCD
    
    lcd.print(buff[count]);
    #endif
    count++;      

  }
  delay(500);

  if( buff[0] != 'O' ){ // jika tidak ada

    memset(buff,0,sizeof(buff));
    
    #ifdef  RUNING_LCD

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(F("BIND "));
    
    #endif

      //do{
    Serial.println(F("pairing"));
    BT.write("AT+BIND="); // pairing
    BT.write(AERORI);
    BT.write(ENTER);
    delay(500);

    memset(buff,0,sizeof(buff));

    count = 0; 
    lcd.setCursor(0,1); 
    while(BT.available()) {

      buff[count]=BT.read();

      #ifdef BLUETOOTH_DEBUG
      Serial.write(buff[count]);
      #endif

      #ifdef  RUNING_LCD
      
      lcd.print(buff[count]);
      #endif
      
      count++;      
    }

    delay(500);

    //bmtHoldRobot

    memset(buff,0,sizeof(buff));
    
    #ifdef  RUNING_LCD

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(F("PAIRING "));
    
    #endif

      //do{
    Serial.println(F("pairing"));
    BT.write("AT+PAIR="); // pairing
    BT.write(AERORI);
    BT.write(",1");
    BT.write(ENTER);
    
    delay(500);

    memset(buff,0,sizeof(buff));

    count = 0; 
    lcd.setCursor(0,1); 
    while(!BT.available());

    while(BT.available()) {

      buff[count]=BT.read();

      #ifdef BLUETOOTH_DEBUG
      Serial.write(buff[count]);
      #endif

      #ifdef  RUNING_LCD
      
      lcd.print(buff[count]);
      #endif
      
      count++;      
    }

    delay(500);
  }

  #ifdef  RUNING_LCD

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("LINKING "));
  
  #endif

  memset(buff,0,sizeof(buff));

  BT.write("AT+LINK="); // link
  BT.write(AERORI);
  BT.write(ENTER);
  count = 0;  
  lcd.setCursor(0,1);

  while(BT.available()) {

    #ifdef BLUETOOTH_DEBUG
    Serial.write(buff[count]);
    #endif

    #ifdef  RUNING_LCD
    lcd.print(buff[count]);
    #endif

    count++;      
  }

  delay(500);    
}

#endif


/*void autokalibrasi(byte dataKe, byte _dataIn){

    int Min_bass, Min_midd, Min_trib;
    int Max_bass, Max_midd, Max_trib;

    Robot.Listening();

    Min_bass = Robot.Bass;
    Min_midd = Robot.Midd;
    Min_trib = Robot.Tribl;

    while (!Robot.Tombol(tom_B));

    delay(3000);
    

    Robot.Press(dataKe,_dataIn);

    while(!Robot.ListenSongOrNot());

    while(Robot.Bussy()){

        Robot.Listening();

        Max_bass = Robot.Bass;
        Max_midd = Robot.Midd;
        Max_trib = Robot.Tribl;
    }

    Robot.setSoundThresh((Max_bass + Min_bass)/2,
                         (Max_midd + Min_midd)/2,
                         (Max_trib + Min_trib)/2);
}
*/
void kalibrasi() 
{
  lcd.clear();

 //int bmt_min, bmt_max;
 int Min_bass, Min_midd, Min_trib;
 int Max_bass, Max_midd, Max_trib;

    //menurut di lcd
    Robot.setSoundThresh(60,65,92); // bass , mid, trible

    if (Robot.Tombol(tom_B)) {

      lcd.setCursor(0, 0);
      lcd.print(F("Kalibrasi :P"));
      lcd.setCursor(0, 1);
      lcd.print(F("A utk next"));
      delay(500);

      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print(F(" ......O.O....."));
      lcd.setCursor(0,1);
      lcd.print(F("T:"));

      while (!Robot.Tombol(tom_A)) {



        Robot.Listening();
        
        Min_bass = Robot.Bass;
        Min_midd = Robot.Midd;
        Min_trib = Robot.Tribl;

        lcd.setCursor(2,1);
        lcd.print(F("   "));
        lcd.setCursor(7,1);
        lcd.print(F("   "));
        lcd.setCursor(12,1);
        lcd.print(F("   "));

        lcd.setCursor(2,1);
        lcd.print(Min_bass);
        lcd.setCursor(7,1);
        lcd.print(Min_midd);
        lcd.setCursor(12,1);
        lcd.print(Min_trib);
        delay(300);

     }//////////////// no sound end
     delay(500);
     lcd.clear();

     lcd.setCursor(0, 0);
     lcd.print(F(" (((((>.<))))) "));
     lcd.setCursor(0,1);
     lcd.print(F("T:"));

     while (!Robot.Tombol(tom_A)) {


      Robot.Listening();

      //bmt_max = Robot.bmt;
       Max_bass = Robot.Bass;
       Max_midd = Robot.Midd;
       Max_trib = Robot.Tribl;

      lcd.setCursor(2,1);
      lcd.print(F("   "));
      lcd.setCursor(7,1);
      lcd.print(F("   "));
      lcd.setCursor(12,1);
      lcd.print(F("   "));

      lcd.setCursor(2,1);
      lcd.print(Max_bass);
      lcd.setCursor(7,1);
      lcd.print(Max_midd);
      lcd.setCursor(12,1);
      lcd.print(Max_trib);

      delay(300);
    }/////////////// sound detect end
    delay(500);
    
    #ifdef USING_EEPROM

    EEPROM.write(BASS_ADD, (Max_bass+ Min_bass) / 2);
    EEPROM.write(MIDL_ADD, (Max_midd + Min_midd) / 2 );
    EEPROM.write(TRIB_ADD, (Max_trib + Min_trib) / 2 );


    Robot.setSoundThresh(EEPROM.read(BASS_ADD),
      EEPROM.read(MIDL_ADD),
      EEPROM.read(TRIB_ADD));

    #endif
}
}

void blank(boolean y){
  lcd.setCursor(13, y);
  lcd.print(F("   "));
}

void yes(boolean y){
  lcd.setCursor(13, y);
  lcd.print(F("   "));
  lcd.setCursor(13, y);
  lcd.print(F("yes"));
}

void no(boolean y){
  lcd.setCursor(13, y);
  lcd.print(F("   "));
  lcd.setCursor(13, y);
  lcd.print(F("no"));
}


void RoboStart(){
  Robot.Press(SELECT);
  delay(500);
  Robot.Press(START);
  delay(500);
  Robot.Release(START);
  delay(500);
  Robot.Release(SELECT);
  delay(500);
}

void cek_sound(){

  int _bass, _midd, _trib;
  lcd.setCursor(0, 0);
  lcd.print(F("CHECK SOUND"));
  lcd.setCursor(0,1);
//  lcd.print(F("T:"));
  lcd.print(F("B:   M:   T:   "));

  while (!Robot.Tombol(tom_A)) {


    Robot.Listening();
    Robot.eq_bug();

    _bass = Robot.Bass;
    _midd = Robot.Midd;
    _trib = Robot.Tribl;

    lcd.setCursor(2,1);
    lcd.print(F("    "));
    lcd.setCursor(7,1);
    lcd.print(F("   "));
    lcd.setCursor(12,1);
    lcd.print(F("    "));

    lcd.setCursor(2,1);
    lcd.print(_bass);
    lcd.setCursor(7,1);
    lcd.print(_midd);
    lcd.setCursor(12,1);
    lcd.print(_trib);

    if ( Robot.ListenSongOrNot() ) {

      yes(0);

    }
    else {

      no(0);
    }

  }  
}


  void RoboRun(byte dataKe, byte _dataIn){

  #ifdef RUNING_LCD
    sprintf(buff,"%d:%c",dataKe,btBuff);
    lcd.setCursor(0,1);
    lcd.print(buff);
  #endif

    while(Robot.Bussy()){
    #ifdef RUNING_LCD
      yes(0);
    #endif
    }

    while(!Robot.Bussy())
    {

      while(!Robot.ListenSongOrNot()){
        Robot.Press(L1);
      #ifdef RUNING_LCD
        no(1);
      #endif   
      }

    #ifdef RUNING_LCD
      yes(1);
    #endif 

      Robot.ReleaseAll();
      delay(500);

      Robot.Press(dataKe,_dataIn);

    #ifdef RUNING_LCD
      no(0);
    #endif

      while(!Robot.ListenSongOrNot()){
        Robot.Press(L1);
      #ifdef RUNING_LCD
        no(1);
      #endif   
      }

      Robot.Release(L1);
    #ifdef RUNING_LCD
      yes(1);
    #endif 


      delay(500);
    #ifdef RUNING_LCD
      sprintf(buff,"%d:%c",dataKe,btBuff);
      lcd.setCursor(0,1);
      lcd.print(buff);
    #endif
    }

    delay(500);
    while(Robot.Bussy()){
    #ifdef RUNING_LCD
      yes(0);
    #endif
    }

  }

