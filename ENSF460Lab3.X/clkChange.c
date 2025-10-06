/*
 * File:   clkChange.c
 * Authors: Shivanshi Visvanatha, Aditi Jain, Daniel Rey
 *
 * Created on September 29, 2025, 2:57 PM
 */

#include "clkChange.h"



void newClk(unsigned int clkval) {
    uint8_t COSCNOSC;
    switch(clkval) {
        case 8: // 8 MHz
            COSCNOSC = 0x00;
            break;
        case 500: // 500 kHz
            COSCNOSC = 0x66;
            break;
        case 32: // 32 kHz
            COSCNOSC = 0x55;
            break;
        default:
            COSCNOSC = 0x55;
    }
    SRbits.IPL = 7;
    CLKDIVbits.RCDIV = 0;
    __builtin_write_OSCCONH(COSCNOSC);
    __builtin_write_OSCCONL(0x01);
    while(OSCCONbits.OSWEN==1) {}
    SRbits.IPL = 0;
}
