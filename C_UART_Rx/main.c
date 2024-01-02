#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	// Setup UART
	    UCA1CTLW0 |= UCSWRST;       // Put UART A1 into SW reset
	    UCA1CTLW0 |= UCSSEL__SMCLK; // Choose SMCLK for UART A1
	    UCA1BRW = 8;                // Set prescalar to 8
	    UCA1MCTLW = 0XD600;         // Configure modulation

	    P4SEL1 &= ~BIT2;            // P4SEL1.3 : P4SEL0.3 = 01
	    P4SEL0 |= BIT2;             // Puts UART A1 Rx on P4.3

	    // Setup LED1
	    P1DIR |= BIT0;
	    P1OUT |= BIT0;


	    PM5CTL0 &= ~ LOCKLPM5;      // Turn on GPIO
	    UCA1CTLW0 &= ~UCSWRST;      // Turn off Software reset for UART A1

	    // Enable interrupts
	    UCA1IE |= UCRXIE;           // Local enable for A1 RXIFG
	    __enable_interrupt();       // Global enable

	    // Main
	    while(1){

	    }
}

#pragma vector = EUSCI_A1_VECTOR
__interrupt void EUSCI_A1_RX_ISR(void){
    if(UCA1RXBUF == 't'){
        P1OUT ^= BIT0;
    }
}
