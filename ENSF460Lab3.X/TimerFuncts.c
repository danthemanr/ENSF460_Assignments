/*
 * File:   TimerFuncts.c
 * Author: Daniel Rey, Aditi Jain, Shivanshi Visvanatha
 *
 * Created on September 29, 2025, 2:19 PM
 */
#include <xc.h>
#include "TimerFuncts.h"
#include "IOFuncts.h"

static uint8_t blink = 0;

// Getter and setter for blink
uint8_t get_blink(void) {
    return blink;
}

void set_blink(uint8_t value) {
    blink = value;
}

void T3init(){
    T2CONbits.T32 = 0; // Operate timer 2 and 3 as 16 bit timers
    T3CONbits.TCKPS = 2; // Set prescaler to 1:64 (corrected for 500kHz clock)
    T3CONbits.TCS = 0;  // Use internal clock
    T3CONbits.TSIDL = 0; // Operate in idle mode
    IPC2bits.T3IP = 2;   // Priority level 2
    IFS0bits.T3IF = 0;   // Clear interrupt flag
    IEC0bits.T3IE = 1;   // Enable timer interrupt
    PR3 = 1953;          // Default to 0.5s period (500ms) at 500kHz, prescaler 1:64
    TMR3 = 0;
    T3CONbits.TON = 1;   // Start timer
}

void T2init(){
    T2CONbits.T32 = 0; // Operate timer 2 as 16 bit timer
    T2CONbits.TCKPS = 2; // Set prescaler to 1:64
    T2CONbits.TCS = 0;  // Use internal clock
    T2CONbits.TSIDL = 0; // Operate in idle mode
    IPC1bits.T2IP = 3;   // Priority level 3
    IFS0bits.T2IF = 0;   // Clear interrupt flag
    IEC0bits.T2IE = 1;   // Enable timer interrupt
    PR2 = 390;           // 50ms period at 500kHz, prescaler 1:64
    TMR2 = 0;
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){
    // Clear timer 2 interrupt flag
    IFS0bits.T2IF = 0;
    T2CONbits.TON = 0; // Stop Timer 2 after debounce period
    
    // Re-verify state after debounce period (state should be stable now)
    uint8_t verify_cur[3];
    verify_cur[0] = PORTBbits.RB7; // PB1
    verify_cur[1] = PORTBbits.RB4; // PB2
    verify_cur[2] = PORTAbits.RA4; // PB3
    
    // Only process if state is still the same (stable)
    if (verify_cur[0] == get_cur(0) && 
        verify_cur[1] == get_cur(1) && 
        verify_cur[2] == get_cur(2)) {
        
        uint8_t pressed_count = (!get_cur(0)) + (!get_cur(1)) + (!get_cur(2));
        
        if (pressed_count == 1) {
            // Single button - set up blinking with appropriate period
            set_blink(1);
            TMR3 = 0; // Reset T3 for consistent blinking start
            
            if (!get_cur(0)) { // PB1
                PR3 = 1953; // 0.5s full cycle
            } else if (!get_cur(1)) { // PB2
                PR3 = 3906; // 1s full cycle
            } else if (!get_cur(2)) { // PB3
                PR3 = 15625; // 4s full cycle
            }
        } else if (pressed_count >= 2) {
            // Two or more buttons - LED on solid, no blinking
            set_blink(0);
            LATBbits.LATB9 = 1;
        }
        // Note: pressed_count == 0 case handled immediately in CN interrupt
        
        set_state_changed(1); // Signal main loop to update display
    }
    // If state doesn't match, it's still bouncing - CN interrupt will restart T2
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void){
    IFS0bits.T3IF = 0;
    
    if (blink) {
        LATBbits.LATB9 = !LATBbits.LATB9; // Toggle LED
    }
}