/*
 * File:   IOFuncts.c
 * Author: Daniel Rey, Aditi Jain, Shivanshi Visvanatha
 *
 * Created on September 29, 2025, 2:19 PM
 */


#include <xc.h>
#include "IOFuncts.h"
#include "TimerFuncts.h"


void IOinit(){
    AD1PCFG = 0xFFFF; /* keep this line as it sets I/O pins that can also be analog to be digital */
    
    /* Let's set up some I/O */
    TRISBbits.TRISB9 = 0;
    LATBbits.LATB9 = 0;
    
    TRISAbits.TRISA4 = 1;
    CNPU1bits.CN0PUE = 1;
    CNEN1bits.CN0IE = 1;
    
    TRISBbits.TRISB4 = 1;
    CNPU1bits.CN1PUE = 1;
    CNEN1bits.CN1IE = 1;
    
    TRISBbits.TRISB7 = 1;
    CNPU2bits.CN23PUE = 1;
    CNEN2bits.CN23IE = 1;
    
    /* Let's clear some flags */
    IPC4bits.CNIP = 6;
    IFS1bits.CNIF = 0;
    IEC1bits.CNIE = 1;
    
    /* Disable nested interrupts*/
    INTCON1.NSTDIS = 1;
}

void IOcheck(){
    if (cur[0]!=past[0]){
    //PB1
        if (cur[0]){
            if (cur[1]&&cur[2]){
                blink = 0;
                LATBbits.LATB9 = 0;
            }
        } else {
            if (cur[1]){
                if (cur[2]){
                    Disp2String("PB1 is pressed\n\r");
                    delay_ms(500);
                } else { //PB3 is
                    Disp2String("PB3 and PB1 are pressed\n\r");
                    blink = 0;
                    LATBbits.LATB9 = 1;
                }
            } else //PB2 is
                if (cur[2]){
                    Disp2String("PB2 and PB1 are pressed\n\r");
                } else { //PB3 is
                    Disp2String("All PBs pressed\n\r");
                }
                blink = 0;
                LATBbits.LATB9 = 1;
        }
    } else if (cur[1]!=past[1]) {
    //PB2
        if (cur[1]){
            if (cur[0]&&cur[2]){
                blink = 0;
                LATBbits.LATB9 = 0;
            }
        } else {
            if (cur[0]){
                if (cur[2]){
                    Disp2String("PB2 is pressed\n\r");
                    delay_ms(500);
                } else { //PB3 is
                    Disp2String("PB3 and PB2 are pressed\n\r");
                    blink = 0;
                    LATBbits.LATB9 = 1;
                }
            } else //PB1 is
                if (cur[2]){
                    Disp2String("PB2 and PB1 are pressed\n\r");
                } else { //PB3 is
                    Disp2String("All PBs pressed\n\r");
                }
                blink = 0;
                LATBbits.LATB9 = 1;
        }
    } else if (cur[2]!=past[2]) {
    //PB3
        if (cur[2]){
            if (cur[0]&&cur[1]){
                blink = 0;
                LATBbits.LATB9 = 0;
            }
        } else {
            if (cur[0]){
                if (cur[1]){
                    Disp2String("PB3 is pressed\n\r");
                    delay_ms(500);
                } else { //PB2 is
                    Disp2String("PB3 and PB2 are pressed\n\r");
                    blink = 0;
                    LATBbits.LATB9 = 1;
                }
            } else //PB1 is
                if (cur[1]){
                    Disp2String("PB3 and PB1 are pressed\n\r");
                } else { //PB2 is
                    Disp2String("All PBs pressed\n\r");
                }
                blink = 0;
                LATBbits.LATB9 = 1;
        }
    }
}


void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void){
    //Don't forget to clear the CN interrupt flag!
    IFS1bits.CNIF = 0;
    past[0] = cur[0];
    past[1] = cur[1];
    past[2] = cur[2];
    cur[0] = PORTBbits.RB7;
    cur[1] = PORTBbits.RB4;
    cur[2] = PORTAbits.RA4;
}


