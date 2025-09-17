/*
 * File:   newmainXC16.c
 * Author: Daniel Rey
 *
 * Created on: USE THE INFORMATION FROM THE HEADER MPLAB X IDE GENERATES FOR YOU
 */

// FBS
#pragma config BWRP = OFF               // Table Write Protect Boot (Boot segment may be written)
#pragma config BSS = OFF                // Boot segment Protect (No boot program Flash segment)

// FGS
#pragma config GWRP = OFF               // General Segment Code Flash Write Protection bit (General segment may be written)
#pragma config GCP = OFF                // General Segment Code Flash Code Protection bit (No protection)

// FOSCSEL
#pragma config FNOSC = FRC              // Oscillator Select (Fast RC oscillator (FRC))
#pragma config IESO = OFF               // Internal External Switch Over bit (Internal External Switchover mode disabled (Two-Speed Start-up disabled))

// FOSC
#pragma config POSCMOD = NONE           // Primary Oscillator Configuration bits (Primary oscillator disabled)
#pragma config OSCIOFNC = ON            // CLKO Enable Configuration bit (CLKO output disabled; pin functions as port I/O)
#pragma config POSCFREQ = HS            // Primary Oscillator Frequency Range Configuration bits (Primary oscillator/external clock input frequency greater than 8 MHz)
#pragma config SOSCSEL = SOSCHP         // SOSC Power Selection Configuration bits (Secondary oscillator configured for high-power operation)
#pragma config FCKSM = CSECMD           // Clock Switching and Monitor Selection (Clock switching is enabled, Fail-Safe Clock Monitor is disabled)

// FWDT
#pragma config WDTPS = PS32768          // Watchdog Timer Postscale Select bits (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (WDT prescaler ratio of 1:128)
#pragma config WINDIS = OFF             // Windowed Watchdog Timer Disable bit (Standard WDT selected; windowed WDT disabled)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))

// FPOR
#pragma config BOREN = BOR3             // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware; SBOREN bit disabled)
#pragma config PWRTEN = ON              // Power-up Timer Enable bit (PWRT enabled)
#pragma config I2C1SEL = PRI            // Alternate I2C1 Pin Mapping bit (Default location for SCL1/SDA1 pins)
#pragma config BORV = V18               // Brown-out Reset Voltage bits (Brown-out Reset set to lowest voltage (1.8V))
#pragma config MCLRE = ON               // MCLR Pin Enable bit (MCLR pin enabled; RA5 input pin disabled)

// FICD
#pragma config ICS = PGx2               // ICD Pin Placement Select bits (PGC2/PGD2 are used for programming and debugging the device)

// FDS
#pragma config DSWDTPS = DSWDTPSF       // Deep Sleep Watchdog Timer Postscale Select bits (1:2,147,483,648 (25.7 Days))
#pragma config DSWDTOSC = LPRC          // DSWDT Reference Clock Select bit (DSWDT uses LPRC as reference clock)
#pragma config RTCOSC = SOSC            // RTCC Reference Clock Select bit (RTCC uses SOSC as reference clock)
#pragma config DSBOREN = ON             // Deep Sleep Zero-Power BOR Enable bit (Deep Sleep BOR enabled in Deep Sleep)
#pragma config DSWDTEN = ON             // Deep Sleep Watchdog Timer Enable bit (DSWDT enabled)

// #pragma config statements should precede project file includes.

#include <xc.h>
#include <p24F16KA101.h>

/**
 * You might find it useful to add your own #defines to improve readability here
 */

void wait(uint32_t ms){//function that stalls for the given number of milliseconds
  uint32_t t=ms*200;
  for(uint32_t i=0; i<t; i++){}
}
void blink(uint32_t ms){
  //function that blinks the LED with a period of ms*2
  LATBbits.LATB9 = 1;
  wait(ms)
  LATBbits.LATB9 = 0;
  wait(ms)
}


int main(void){
  
  AD1PCFG = 0xFFFF;      //keep this line as it sets I/O pins that can also be analog to be digital
  
  TRISBbits.TRISB9 = 0;  //set LED pin to output
  
  TRISBbits.TRISB7 = 1;  //set PB1 pin to input
  CNPU2bits.CN23PUE= 1;  //PB1 is 1 by default
  
  TRISBbits.TRISB4 = 1;  //set PB2 pin to input
  CNPU1bits.CN1PUE = 1;  //PB2 is 1 by default
  
  TRISAbits.TRISA4 = 1;  //set PB3 pin to input
  CNPU1bits.CN0PUE = 1;  //PB3 is 1 by default
  
  uint8_t s;
  while(1) {
    s = PORTBbits.PORTB7+PORTBbits.PORTB4+PORTAbits.PORTA4;
    //sum the number of unpressed buttons
    if(s<=1){
      LATBbits.LATB9 = 1;//if <=1 button are unpressed, then turn the LED on
    } else {
      LATBbits.LATB9 = 0;//make sure the LED is off
      if(s==2){          //if only one button is pressed
        //I use the ternary operator to select the 0 button's corresponding blink interval
        blink((PORTBbits.PORTB7) ? ((PORTBbits.PORTB4) ? 4000 : 1000) : 500);
      }
    }
  }
  return 0;
}