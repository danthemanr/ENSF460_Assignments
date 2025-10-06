/* 
 * File:   TimerFuncts.h
 * Author: Daniel Rey, Aditi Jain, Shivanshi Visvanatha
 */

#ifndef TIMERFUNCTS_H
#define	TIMERFUNCTS_H

#include <xc.h>

uint8_t blink = 0;

void T3init();
void delay_ms(uint16_t time_ms);

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void);

#endif