#include <xc.h>
#include "fosc.h"


inline void fosc_init()
{
	OSCCON = 0x58;
}
