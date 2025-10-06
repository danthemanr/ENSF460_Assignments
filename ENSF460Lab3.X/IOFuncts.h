/* 
 * File:   IOFuncts.h
 * Author: Daniel Rey, Aditi Jain, Shivanshi Visvanatha
 */

/* 
 * File:   IOFuncts.h
 * Author: Daniel Rey, Aditi Jain, Shivanshi Visvanatha
 */

#ifndef IOFUNCTS_H
#define	IOFUNCTS_H

#include <xc.h>

void IOinit();
void IOcheck();

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void);

// Getter and setter functions for past and cur arrays
uint8_t get_past(uint8_t index);
uint8_t get_cur(uint8_t index);
void set_past(uint8_t index, uint8_t value);
void set_cur(uint8_t index, uint8_t value);

uint8_t get_state_changed(void);
void set_state_changed(uint8_t value);

#endif