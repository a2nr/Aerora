
/*
 * KRSI2015
 * library ini dibuat untuk memudahkan pada penerus tim KRSI selanjutnya
 * jangan bingung dulu tentang program yang udah di tulis dibawah
 *
 *
*/

#ifndef KRSI_H
#define KRSI_H

#include "Arduino.h"
#include "JoystickClone.h"
#include <avr/pgmspace.h>
#include <stdio.h>


#define DEBUG_MODE

typedef const PROGMEM char  conFchar;
typedef const PROGMEM byte  conFbyte;
#define ENTER       "\r\n"

class KRSI
{
  public:
    KRSI(); // konstruktor
    //===============================================
    // Comunication Interface
    void ComunicationInit(); // inisialisasi komunikasi UART
    //===============================================



    //===============================================
    // for Electrical Ear
    /* void EarInit(byte _analog, byte _Strobe, byte _Reset);
     *
     * digunakan untuk inisialisasi sensor MSGEQ7
     * * parameter:
     * * * _analog : pin analog ke berapa untuk menerima sinyal
     * * * _Strobe : pin digital untuk keperluan SMGEQ07 mengeluaran sinyal
     * * * _Reset  : pin digital untuk keperluan SMGEQ07 mengeluaran sinyal
     * * Return: NULL
     * * datasheet: https://www.sparkfun.com/datasheets/Components/General/MSGEQ7.pdf
     */
    void EarInit();//byte _analog, byte _Strobe, byte _Reset);



    /* void Listening();
     *
     * Mengambil 100 data spektrum BASS, TRIBLE, dan MIDELL dan dilakukan rata - rata
     * Disimpan di dalam variable : int Bass, Midd, Tribl;
     * * parameter:NULL
     * * Return: NULL
     */
    void Listening();



    /* void setSoundThresh(int bass, int midle, int tible);
     *
     * prosedur ini dugunakan untuk memberikan nilai batas ambang
     * batas ambang tersebut digunakan untuk mendeteksi terdapat suara atau tidak
     *
     * * parameter:
     * * * bass  : nilai bass akan disimpan pada int HoldBass
     * * * midle : nilai midle akan disimpan pada int HoldMidd
     * * * tible : nilai tible akan disimpan pada int HoldTrible
     * * Return: NULL
     */
    void setSoundThresh(int bass, int midle, int tible);



    /* boolean ListenSongOrNot();
     *
     * Fungsi ini dugunakan untuk menentukan adanya suawa atau tidak
     * dengan membandingkan nilai sensor yang sekarang ketiga batas ambang
     *
     * * parameter: NULL
     * * Return:
     * * * True : (Bass > HoldBass) || (Midd > HoldMidle) || (Tribl > HoldTrible)
     * * * False: (Bass < HoldBass) || (Midd < HoldMidle) || (Tribl < HoldTrible)
     */
    boolean ListenSongOrNot();



    /* void viewSoundData();
     *
     * Menampilkan nilai dari suara ke terminal
     *
     * * parameter: NULL
     * * Return:
     */
    void viewSoundData();

    //===============================================
    // for controler
    //===============================================

    /* boolean Tombol(char t);
     *
     * metode tombol yang digunakan pada minimum system adalah menggunakan pembagi tegangan (coba di liat di schematic :p
     * buat mengetahui tegangannya). prinsip kerjanya adalah jika tombol A tertekan maka tegangan yang keluar sekian, tombol B 
     * tertekan maka tegangan yang keluar berbeda lagi.
     *
     * * parameter : tombol ke
     * * Return    : true jika memenuhi range tegangan
     * *             false jika tidak memenuhi range tegangan
     * 
     */
    boolean Tombol(char t);
    
    /* void ControllerInit();
     * 
     * prosedur ini digunakan untuk konfigurasi awal untuk keperluan komunikasi menggunakan Robovie
     *
     */
    void ControllerInit();

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
    void Press(byte dataKe, byte _dataIn);

    /* void Analog(byte dataKe, byte _dataIn);
     *
     * untuk mengirim data analog ke robovie
     *
     * * parameter : datake  : data ke berapa tombol tersebut berada
     * *             _dataIn : data yang dikirim bagaimana
     * * return    : NULL
     */
    void Analog(byte dataKe, byte _dataIn);

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
    void Release(byte dataKe, byte _dataIn);

    /* void ReleaseAll();
     *
     * sama seperti prosedur Release tapi ReleaseAll digunakan untuk melepas semua tombol yang sudah ditekan
     *
     */
    void ReleaseAll();

    /* boolean Bussy();
     * 
     * 
     * 
     */
    boolean Bussy();

    void BT_init();

    unsigned int Bass          , Midd          , Tribl;

    void eq_bug();
    byte HoldBass, HoldMidle, HoldTrible;
    byte bmt,bmtHold;


  private:

    unsigned int looping;
    //===============================================
    // for Electrical Ear
    void Hear();

    byte bass_bef, mid_bef, trib_bef;

    byte spectrumValue[7];

};


 static void Robotsetup();
 static void Robotloop(); 
#endif
