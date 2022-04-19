/*
 * File:       main.c
 * Author:     Jan Abramowicz
 * Last edit:  2022/04/19 (YYYY/MM/DD)
 */

#include <xc.h>
#include "configurationBits.h"
#include "stdint.h"

#define FCY 4000000

uint8_t counter;

void __attribute__((interrupt(auto_psv))) _T1Interrupt(void);

void __attribute__((interrupt(auto_psv))) _T1Interrupt(void) {    
    counter++;
    IFS0bits.T1IF = 0;
}

int main(void) {
    T1CON = 0x0;
    T1CONbits.TCKPS = 0x2; //1:64 prescale
    T1CONbits.TCS = 0; //internal oscilator - uses primary oscilator (OSC1 and OSC2)
    T1CONbits.TGATE = 0; //Gated time accumulation disabled
    
    TMR1 = 0x0; //reset timer register
    PR1 = 0x3d09; //value to be counted by timer1 module
    
    IFS0bits.T1IF = 0; //timer1 interrupt flag reset
    IPC0bits.T1IP = 0x7; //timer1 interrupt set as the highest priority interrupt - level does not matter for this program
    IEC0bits.T1IE = 1; //timer1 interrupt enable
    
    TRISBbits.TRISB6 = 0; //RB6 as output
    
    T1CONbits.TON = 1; //timer1 enable (start counting)
    
    while (1) {
        if ((counter % 2) == 0) {
            PORTBbits.RB6 = 1;
        }
        else {
            PORTBbits.RB6 = 0;
        }
    }
    
    return 0;
}
