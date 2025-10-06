#ifndef TIMEDELAY_H
#define TIMEDELAY_H

#include <xc.h>

// Function declarations
void T2init();
void T3init();
void delay_ms(uint16_t time_ms);
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void);

#endif // TIMEDELAY_H