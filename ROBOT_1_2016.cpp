#ifdef USING_AERORA && TAHUN_2016

//#define DEBUG_MODE
#include "KRSI.h"

KRSI Seni16;

void RobotSetup(){
	Seni16.ComunicationInit();
	Seni16.ControllerInit();
}

void RobotLoop(){
	Seni16.Press(UP);
	Seni16.Release(DOWN);
	delay(1000);
	Seni16.Release(UP);
	Seni16.Press(DOWN);
	delay(1000);
}

#endif //USING_AERORA && TAHUN_2016