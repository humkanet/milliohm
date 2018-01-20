#ifndef _CLOCK_H
#define	_CLOCK_H

#include <stdint.h>

typedef void (*CLOCK_EVENT)();


inline void     clock_isr();

inline void     clock_init();
inline uint8_t  clock_sec();
uint16_t        clock_msec();
inline void     clock_event(CLOCK_EVENT event);

#endif
