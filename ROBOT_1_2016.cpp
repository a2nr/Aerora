#ifdef USING_AERORA && TAHUN_2016


#include "KRSI.h"

KRSI Seni16;
																									
void Robotsetup(){
	 Seni16.ComunicationInit();
	 Seni16.ControllerInit();
  // turn on SPI in slave mode
}

void Robotloop(){
	
	Seni16.Press(START);
	Seni16.Press(SELECT);
	delay(1000);
	Seni16.Release(START);
	delay(1000);
}

#endif //USING_AERORA && TAHUN_2016