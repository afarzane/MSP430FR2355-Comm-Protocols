#include <msp430.h> 

char packet[] = {0xF0, 0xF0, 0xF0, 0x40};
unsigned int pos;
/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	// Set up A0 for SPI
	UCA0CTLW0 |= UCSWRST;       // Put A0 into SW reset

	UCA0CTLW0 |= UCSSEL__SMCLK; // Choose SMCLK
	UCA0BRW = 10;               // Set prescalar to 10 to get SCLK = 100 kHz

	UCA0CTLW0 |= UCSYNC;        // Put A0 into SPI mode
	UCA0CTLW0 |= UCMST;         // Put into SPI Master

	// Configure ports
	P4DIR &= ~BIT1;             // Set SW1 as input
	P4REN |= BIT1;              // Enable resistor
	P4OUT |= BIT1;              // Set resistor as pull up
	P4IES |= BIT1;              // Sensitive to H-to-L

	P1SEL1 &= ~BIT5;            // P1.5 = SCLK
	P1SEL0 |= BIT5;

	P1SEL1 &= ~BIT7;            // P1.7 = SIMO
	P1SEL0 |= BIT7;

	P1SEL1 &= ~BIT6;            // P1.6 = MISO
	P1SEL0 |= BIT6;

	PM5CTL0 &= ~LOCKLPM5;       // Turn on GPIO

	UCA0CTLW0 &= ~UCSWRST;      // Take A0 out of SW reset

	// Enable IRQs
	P4IE |= BIT1;               // Enable P4.1 IRQ
	P4IFG &= ~BIT1;             // Clear flag

	UCA0IE |= UCTXIE;           // Enable A0 Tx IRQ
	UCA0IFG &= ~UCTXIFG;        // Clear flag

	__enable_interrupt();       // Global enable

	// Main
	while(1){}

}

// Interrupt Service Routines
#pragma vector = PORT4_VECTOR
__interrupt void ISR_PORT4_S1(void){
    pos = 0;
    UCA0TXBUF = packet[pos];    // Send first byte

    P4IFG &= ~BIT1;
}

#pragma vector = EUSCI_A0_VECTOR
__interrupt void ISR_EUSCI_A0(void){
    pos++;

    if(pos < sizeof(packet)){
        UCA0TXBUF = packet[pos];    // send next byte
    }else{
        UCA0IFG &= ~UCTXIFG;          // Clear interrupt flag
    }
}

