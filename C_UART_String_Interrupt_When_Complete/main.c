#include <msp430.h> 

char message[] = "Hello World!";
unsigned int i;
unsigned int j;
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

	P4SEL1 &= ~BIT3;            // P4SEL1.3 : P4SEL0.3 = 01
	P4SEL0 |= BIT3;             // Puts UART A1 Tx on P4.3

	// Setup Switch
	P4DIR &= ~BIT1;
	P4REN |= BIT1;
	P4OUT |= BIT1;
	P4IES |= BIT1;


	PM5CTL0 &= ~ LOCKLPM5;      // Turn on GPIO
	UCA1CTLW0 &= ~UCSWRST;      // Turn off Software reset for UART A1

	// Enable interrupts
	P4IE |= BIT1;               // Enable SW1 IRQ
	P4IFG &= ~BIT1;             // Clear flag
	__enable_interrupt();       // Global enable


	// Main loop
	while(1){

	}
}

#pragma vector = PORT4_VECTOR
__interrupt void ISR_Port4_S1(void){
    i = 0;
    UCA1IE |= UCTXCPTIE;        // Turns on Tx complete IRQ
    UCA1IFG &= ~UCTXCPTIFG;       // Clears Tx complete flag
    UCA1TXBUF = message[i];     // Put first char from message
    P4IFG &= ~BIT1;             // clear flag
}

#pragma vector = EUSCI_A1_VECTOR
__interrupt void ISR_EUSCI_A1(void){
    if(i == sizeof(message)){
        UCA1IE &= ~UCTXCPTIE;    // Turn off Tx complete IRQ
    }
    else{
        i++;
        UCA1TXBUF = message[i];     // Put next char into Tx buffer
    }

    UCA1IFG &= ~UCTXCPTIFG;    // Turn off Tx complete flag
}
