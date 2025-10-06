/*
 * File:   TimeDelay.c
 * Author: Daniel Rey
 *
 * Created on September 22, 2025, 4:45 PM
 */


#include "TimeDelay.h"

void T2init(){
    T2CONbits.T32 = 0;   // operate timer 2 and 3 as 16 bit timer
    T2CONbits.TCKPS = 1; // set prescaler to 8
    T2CONbits.TCS = 0;   // use internal clock
    // T2CONbits.TGATE = 0;
    T2CONbits.TSIDL = 0; //operate in idle mode when activated
    
    IPC1bits.T2IP = 2;   // set priority
    IFS0bits.T2IF = 0;   // clear flag
    IEC0bits.T2IE = 1;   // enable timer interrupt
    PR2 = 15625;         // set the count value
    TMR2 = 0;            // start timer at 0
    T2CONbits.TON = 1;   // activate
}
void T3init(){
    T3CONbits.TCKPS = 2; // set prescaler to 64
    T3CONbits.TCS = 0;   // use internal clock
    // T3CONbits.TGATE = 0;
    T3CONbits.TSIDL = 0; //operate in idle mode when activated
    
    IPC2bits.T3IP = 2;   // set priority
    IFS0bits.T3IF = 0;   // clear flag
    IEC0bits.T3IE = 1;   // enable timer interrupt
    PR3 = 1953;          // set the count value
    TMR3 = 0;            // start timer at 0
    T3CONbits.TON = 1;   // activate
}

void delay_ms(uint16_t time_ms){ //if time_ms > 16776 then overflow happens
    //this function only works properly for F_OSC == 500 kHz
    float t1 = time_ms*3.90625;
    uint16_t t2 = (uint16_t)t1;
    if (t1-t2>=0.5) t2 += 1;
    if (PR3!=t2){
        TMR3 = 0;
        PR3 = t2;
    }
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){
    LATAbits.LATA6 = (LATAbits.LATA6) ? 0 : 1;
    IFS0bits.T2IF=0; //Clear timer 2 interrupt flag
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void){
    LATBbits.LATB9 = (LATBbits.LATB9) ? 0 : 1;
    IFS0bits.T3IF=0; //Clear timer 3 interrupt flag
}
