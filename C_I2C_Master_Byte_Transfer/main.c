#include <msp430.h> 


unsigned int i;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	// Set up B0 for I2C
	UCB0CTLW0 |= UCSWRST;       // Put in SW reset

	UCB0CTLW0 |= UCSSEL_3;      // Choose SMCLK
	UCB0BRW = 10;               // Set prescalar to 10

	UCB0CTLW0 |= UCMODE_3;      // Put into I2C mode
	UCB0CTLW0 |= UCMST;         // Set as master
	UCB0CTLW0 |= UCTR;          // Put into Tx mode (Write)
	UCB0I2CSA = 0x68;           // Set slave address RTC = 0x68

	UCB0CTLW1 |= UCASTP_2;      // Auto stop mode
	UCB0TBCNT |= 1;             // Byte count = 1

	// Set up ports
	P1SEL1 &= ~BIT3;            // P1.3 = SCL
	P1SEL0 |= BIT3;

    P1SEL1 &= ~BIT2;            // P1.2 = SDA
    P1SEL0 |= BIT2;

    PM5CTL0 &= ~LOCKLPM5;       // Turn on GPIO

    UCB0CTLW0 &= ~UCSWRST;      // Take out of SW reset

    // Enable B0 TX0 IRQ
    UCB0IE |= UCTXIE0;          // Local enable for Tx0
    __enable_interrupt();       // Global enable

	// Main
	while(1){
	    UCB0CTLW0 |= UCTXSTT;   // Manually start message (START)
	    for(i=0; i<100; i++){/*delay*/}
	}
}

// Interrupt Service Routines
#pragma vector = EUSCI_B0_VECTOR
__interrupt void EUSCI_B0_I2C_ISR(void){
    UCB0TXBUF = 0xBB;
}
