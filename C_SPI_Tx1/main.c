#include <msp430.h> 

int i;

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	// Set up SPI A0
	UCA0CTLW0 |= UCSWRST;       // Put A0 into SW reset

	UCA0CTLW0 |= UCSSEL__SMCLK; // Choose SMCLK
	UCA0BRW = 10;               // Set prescalar to 10 to get SCLK = 100 kHz

	UCA0CTLW0 |= UCSYNC;        // Put A0 into SPI mode
	UCA0CTLW0 |= UCMST;         // Put into SPI master

	// Configure the ports
	P1SEL1 &= ~BIT5;             // P1.5 use SCLK (01)
	P1SEL0 |= BIT5;

    P1SEL1 &= ~BIT7;             // P1.7 use SIMO (01)
    P1SEL0 |= BIT7;

    P1SEL1 &= ~BIT6;             // P1.6 use SOMI (01)
    P1SEL0 |= BIT6;

    PM5CTL0 &= ~LOCKLPM5;       // Turn on GPIO

    UCA0CTLW0 &= ~UCSWRST;       // Take A0 out of SW reset

    while(1){
        UCA0TXBUF = 0x4D;       //Send 0x4D over SPI
        for(i=0; i<10000; i++){/*delay loop*/}
    }

}
