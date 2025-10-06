/* 
 * File:   TimerFuncts.h
 * Author: Daniel Rey, Aditi Jain, Shivanshi Visvanatha
 */

#ifndef IOFUNCTS_H
#define	IOFUNCTS_H

#include <xc.h>

uint8_t past[3], cur[3] = {1,1,1};

void IOinit();
void IOcheck();

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void);

#endif