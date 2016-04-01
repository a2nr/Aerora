#ifdef USING_AERORA && TAHUN_2016


#include "KRSI.h"

KRSI Seni16;
																									
void Robotsetup(){
	 Seni16.ComunicationInit();
	 Seni16.ControllerInit();
  // turn on SPI in slave mode
}

void Robotloop(){
	// #ifdef DEBUG_MODE
	// Seni16.ControlerDataView();
	// #else
	// 	#error oii
	// #endif
	Seni16.Press(UP);
	Seni16.Press(DOWN);
	
	delay(1000);
	Seni16.Release(UP);
	Seni16.Release(DOWN);
	

	delay(1000);
}

#endif //USING_AERORA && TAHUN_2016