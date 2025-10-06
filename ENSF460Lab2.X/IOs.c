/*
 * File:   IOs.c
 * Author: HPLaptop
 *
 * Created on September 22, 2025, 4:50 PM
 */


#include "IOs.h"
#include "TimeDelay.h"

void IOinit() {
    AD1PCFG = 0xFFFF;      //keep this line as it sets I/O pins that can also be analog to be digital
    
    TRISBbits.TRISB9 = 0;  //set LED1 pin to output
    
    TRISAbits.TRISA6 = 0;  //set LED2 pin to output

    TRISBbits.TRISB7 = 1;  //set PB1 pin to input
    CNPU2bits.CN23PUE= 1;  //PB1 is 1 by default

    TRISBbits.TRISB4 = 1;  //set PB2 pin to input
    CNPU1bits.CN1PUE = 1;  //PB2 is 1 by default

    TRISAbits.TRISA4 = 1;  //set PB3 pin to input
    CNPU1bits.CN0PUE = 1;  //PB3 is 1 by default
}

void IOcheck() {
    if (PORTBbits.RB7&&PORTBbits.RB4&&PORTAbits.RA4) {
        LATBbits.LATB9 = 0;
        TMR3 = 0;
    } else {
        delay_ms((PORTBbits.RB7||PORTBbits.RB4||PORTAbits.RA4) ? ((PORTBbits.RB7) ? (PORTBbits.RB4 ? 4000 : 1000) : 500) : 1);
    }
}
