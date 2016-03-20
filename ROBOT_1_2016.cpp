#ifdef USING_AERORA && TAHUN_2016


#include "KRSI.h"
#include "ROBOT_1_2016.h"

KRSI Seni16;

// FUNGSI ROBO RUN


#define LENGTH_COMMAND	6 
#define POLING			2000 //listening 2 second for listening song

CmdTbl RoboCmdTbl [LENGTH_COMMAND] = {
		UP			,
		RIGHT		,
		LEFT		,
		LEFT		,
		DOWN		,
		CROSS		,
};

void RobotSetup(){
	Seni16.ComunicationInit();
	Seni16.ControllerInit();
	Seni16.EarInit();

	while( !Seni16.Tombol('A') ); //nunggu di tekan

	for (byte i = 0; i < LENGTH_COMMAND; i++)
	{
		RoboRun(RoboCmdTbl[ i ]);
	}
}

void RobotLoop(){

	#ifdef DEBUG_MODE
	Seni16.ControlerDataView();
	#endif

	

	//RoboRun( COMMAND[2] );

}


// fungsi robo run, mungkin aku khilaf kalo algoritmanya gak pas. boleh dikembangkan sendiri.
/********************************************************************************************************
/* KETENTUAN KETIKA MEMBUAT Pose 																		*
/********************************************************************************************************
 * Hi Robovie programer, 																				*
 * Ketahui lah bahwa komunikasi antar robovie dan arduino menggunakan FeedBack (.Bussy()). FeedBack tsb	*
 * digunakan untuk memberitahukan kepada Arduino bahwa Robovie sedang melakukan kegiatan menari. ini 	*
 * berguna untuk memberikan interupsi pada arduino bahwa robovie mendapati event mengerjakan gerakan	*
 * maka arduino harus menghentikan memberi perintah selanjutnya. Pengaturan FeedBack dilakukan sama 	*
 * seperti menggeser Servo.																				*
 * Gerakan Tari robot di bagi dengan gerakan gerakan pada setiap tombol.								*
 * Buat lah pose sebagai berikut.																		*
 * 1. Pose START STOP.																					*
 *    Pose START STOP digunakan untuk menghidupkan servorobot untuk memulai suatu gerakan.				*
 * 2. Pose Nari.																						*
 *    Pose Nari terdiri dari beberapa gerakan pada setiap tombol, contoh tombol CROSS menjalankan Pose 	*
 *    tertentu. Pose Nari harus memberikan FeedBack HIGH untuk mengidikasikan sedang mengerjakan Tarian	*
 *	  lalu di akhiri dengan FeedBack LOW. 																*
 * 3. Pose Idle Nari.																					*
 *    Pose Idle Nari digunakan untuk menggerakkan pose yang aman ketika setelah menggerakkan Pose Nari,	*
 *    Pose Idle Nari menggunakan Command Combinasi contohnya. Pose Nari dengan tombol CROSS maka 		*
 *    pose Idle Nari menggunakan Combinasi L1 + CROSS. Jadi New Gen Robovie Programer, bikin satu pose 	*
 *	  yang terdisi dari satu gerakan saja yang isinya gerakan terakhir dari Pose Nari yang sesuai		*
 *	  tombol nya. pastikan Gerakan terakhirnya "safe" artinya aman untuk robot berhenti dan tidak jatuh *
 *	  Jangan lupa untuk memberikan FeedBack LOW untuk mengindikasi kan bahwa Robovie tdk sibuk			*
 *	  																									*
 ********************************************************************************************************/
void RoboStrat(void){
	Seni16.Press(SELECT);
	delay(500);
	Seni16.Press(START);
	delay(500);
	Seni16.Release(START);
	delay(500);
	Seni16.Release(SELECT);
	delay(500);	
}

void RoboRun(TSJoyCmd _cmdToRobot) {
	byte count = POLING;
	
	while ( count > 0 ){ 

		while ( Seni16.ListenSongOrNot() == false );
		delay(1);
		count --;
	}

	Seni16.ReleaseAll();

	do {
		Seni16.Press(_cmdToRobot);
	}while ( Seni16.Bussy() == false ); //loop while robot not bussy to make sure robot is runing

	while ( Seni16.Bussy() == true ); // loop until robot not bussy before to next stage 

	Seni16.Press(L1);
}
#endif //USING_AERORA && TAHUN_2016