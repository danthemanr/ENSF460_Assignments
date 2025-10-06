/*
 * File:   TimerFuncts.c
 * Author: Daniel Rey, Aditi Jain, Shivanshi Visvanatha
 *
 * Created on September 29, 2025, 2:19 PM
 */


#include <xc.h>
#include "TimerFuncts.h"


void T3init(){
    T2CONbits.T32 = 0; // operate timer 2 and 3 as 16 bit timers
    T3CONbits.TCKPS = 2; // set prescaler to 1:8
    T3CONbits.TCS = 0; // use internal clock
    T3CONbits.TSIDL = 0; //operate in idle mode
    IPC2bits.T3IP = 2; //7 is highest and 1 is lowest pri.
    IFS0bits.T3IF = 0;
    IEC0bits.T3IE = 1; //enable timer interrupt
    PR3 = 1953; // set the count value for 0.5 s (or 500 ms)
    TMR3 = 0;
    T3CONbits.TON = 1;
}

void delay_ms(uint16_t time_ms){ //if time_ms > 16776 then overflow happens
    //this function only works properly for F_OSC == 500 kHz
    blink = 1;
    float t1 = time_ms*3.90625;
    uint16_t t2 = (uint16_t)t1;
    if (t1-t2>=0.5) t2 += 1;
    if (PR3!=t2){
        TMR3 = 0;
        PR3 = t2;
    }
}


void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){
    //Don't forget to clear the timer 2 interrupt flag!
    IFS0bits.T2IF = 0;
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void){
    //Don't forget to clear the timer 2 interrupt flag!
    IFS0bits.T3IF = 0;
    if (blink) LATBbits.LATB9 = (LATBbits.LATB9) ? 0 : 1;
}
