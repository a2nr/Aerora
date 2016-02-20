


#ifndef USING_AERORA || USING_AERORI
	#error "Define dulu dong USING_AERORI apa USING_AERORA di file Aerora.ino sebelum #include "Aerora.h" "
#endif


#if defined(TAHUN_2015)
	#if defined (USING_AERORA)
	    #include "ROBOT_1_2015.cpp"
	#elif  defined (USING_AERORI)
		#include "ROBOT_2_2015.cpp"
	#endif // pilih robot

#elif defined(TAHUN_2016)
	#if defined (USING_AERORA)
	    #include "ROBOT_1_2016.cpp"
	#elif  defined (USING_AERORI)
		#include "ROBOT_2_2016.cpp"
	#endif // pilih robot
#else

//Format penambahan Project
/*#if defined( TAHUN_<tahun> )
	#if defined (USING_AERORA)
	    #include "ROBOT_<nomer robot satu>_<tahun>.cpp"
	#elif  defined (USING_AERORI)
		#include "ROBOT_<nomer robot dua>_<tahun>.cpp"
	#endif // pilih robot
#else*/

	#error "Kamu lombanya kapan ? define TAHUN_<tahun> di file Aerora.ino sebelum #include "Aerora.h" "
	#error "Belum daftarin #ifdef nya ya di Aerora.h ya? formatnya ada di sana"
#endif //pilih tahun
