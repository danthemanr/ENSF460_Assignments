/*
 * File:   IOFuncts.c
 * Author: Daniel Rey, Aditi Jain, Shivanshi Visvanatha
 *
 * Created on September 29, 2025, 2:19 PM
 */

#include <xc.h>
#include "IOFuncts.h"
#include "TimerFuncts.h"
#include "UART2.h"

static uint8_t past[3] = {1, 1, 1}, cur[3] = {1, 1, 1};
static uint8_t state_changed = 0;

// Getter and setter implementations
uint8_t get_past(uint8_t index) {
    return past[index];
}
uint8_t get_cur(uint8_t index) {
    return cur[index];
}
void set_past(uint8_t index, uint8_t value) {
    past[index] = value;
}
void set_cur(uint8_t index, uint8_t value) {
    cur[index] = value;
}

uint8_t get_state_changed(void) {
    return state_changed;
}
void set_state_changed(uint8_t value) {
    state_changed = value;
}

void IOinit(){
    AD1PCFG = 0xFFFF; /* keep this line as it sets I/O pins that can also be analog to be digital */
    
    /* Let's set up some I/O */
    TRISBbits.TRISB9 = 0; // LED on RB9 as output
    LATBbits.LATB9 = 0;   // LED initially off
    
    TRISAbits.TRISA4 = 1; // PB3 on RA4 as input
    CNPU1bits.CN0PUE = 1; // Enable pull-up for PB3
    CNEN1bits.CN0IE = 1;  // Enable CN interrupt for PB3
    
    TRISBbits.TRISB4 = 1; // PB2 on RB4 as input
    CNPU1bits.CN1PUE = 1; // Enable pull-up for PB2
    CNEN1bits.CN1IE = 1;  // Enable CN interrupt for PB2
    
    TRISBbits.TRISB7 = 1; // PB1 on RB7 as input
    CNPU2bits.CN23PUE = 1;// Enable pull-up for PB1
    CNEN2bits.CN23IE = 1; // Enable CN interrupt for PB1
    
    /* Let's clear some flags */
    IPC4bits.CNIP = 6;    // CN interrupt priority (highest)
    IFS1bits.CNIF = 0;    // Clear CN interrupt flag
    IEC1bits.CNIE = 1;    // Enable CN interrupts
    
    /* Disable nested interrupts */
    INTCON1bits.NSTDIS = 1;
}

void IOcheck(){
    if (state_changed) {
        state_changed = 0;
        
        // Count the number of pressed buttons (0 = pressed, 1 = not pressed due to pull-ups)
        uint8_t pressed_count = (!cur[0]) + (!cur[1]) + (!cur[2]);
        
        // Clear the terminal line before new message
        Disp2String("\r                            \r");
        
        if (pressed_count == 0) {
            // No buttons pressed - LED already off from T2
            Disp2String("Nothing pressed\n");
        } else if (pressed_count == 1) {
            // Single button pressed - blink already set in T2
            if (!cur[0]) { // PB1 pressed
                Disp2String("PB1 is pressed\n");
            } else if (!cur[1]) { // PB2 pressed
                Disp2String("PB2 is pressed\n");
            } else if (!cur[2]) { // PB3 pressed
                Disp2String("PB3 is pressed\n");
            }
        } else if (pressed_count == 2) {
            // Two buttons pressed - LED already on from T2
            if (!cur[0] && !cur[1]) {
                Disp2String("PB1 and PB2 are pressed\n");
            } else if (!cur[0] && !cur[2]) {
                Disp2String("PB1 and PB3 are pressed\n");
            } else if (!cur[1] && !cur[2]) {
                Disp2String("PB2 and PB3 are pressed\n");
            }
        } else if (pressed_count == 3) {
            // All three buttons pressed - LED already on from T2
            Disp2String("All PBs pressed\n");
        }
    }
}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void){
    // Clear the CN interrupt flag
    IFS1bits.CNIF = 0;
    
    // Read current button states
    uint8_t new_cur[3];
    new_cur[0] = PORTBbits.RB7; // PB1
    new_cur[1] = PORTBbits.RB4; // PB2
    new_cur[2] = PORTAbits.RA4; // PB3
    
    // Check if state has changed
    if (new_cur[0] != cur[0] || new_cur[1] != cur[1] || new_cur[2] != cur[2]) {
        // Update current and past states
        past[0] = cur[0];
        past[1] = cur[1];
        past[2] = cur[2];
        cur[0] = new_cur[0];
        cur[1] = new_cur[1];
        cur[2] = new_cur[2];
        
        // IMMEDIATE response for button release
        uint8_t new_pressed_count = (!new_cur[0]) + (!new_cur[1]) + (!new_cur[2]);
        if (new_pressed_count == 0) {
            // All buttons released - immediately turn off LED and stop blinking
            LATBbits.LATB9 = 0;
            set_blink(0);
            T2CONbits.TON = 0; // Stop debounce timer - no need to verify release
            set_state_changed(1); // Trigger display update
        } else {
            // Buttons pressed - start/restart debounce timer
            TMR2 = 0;
            T2CONbits.TON = 1;
        }
    }
}