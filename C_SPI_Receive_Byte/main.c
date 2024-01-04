#include <msp430.h> 

int Rx_Data;

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
	P1DIR |= BIT0;
	P1OUT &= ~BIT0;
	P6DIR |= BIT6;
	P6OUT &= ~BIT6;             // Set up both LED 1 and 2 as outputs

	P4DIR &= ~BIT1;
	P4REN |= BIT1;
	P4OUT |= BIT1;
	P4IES |= BIT1;              // Set up switch 1 as input with pull up resistor and interrupt on H-to-L

    P2DIR &= ~BIT3;
    P2REN |= BIT3;
    P2OUT |= BIT3;
    P2IES |= BIT3;              // Set up switch 2 as input with pull up resistor and interrupt on H-to-L

    // SPI port setup
    P1SEL1 &= ~BIT5;            // P1.5 = SCLK
    P1SEL0 |= BIT5;

    P1SEL1 &= ~BIT7;            // P1.7 = SIMO
    P1SEL0 |= BIT7;

    P1SEL1 &= ~BIT6;            // P1.6 = MISO
    P1SEL0 |= BIT6;

    PM5CTL0 &= ~LOCKLPM5;       // Turn on GPIO
    UCA0CTLW0 &= ~UCSWRST;      // Take A0 off of SW reset

    // Set up IRQs
    P4IE |= BIT1;               // Enable SW1 interrupt
    P4IFG &= ~BIT1;

    P2IE |= BIT3;               // Enable SW2 interrupt
    P2IFG &= ~BIT3;

    UCA0IE |= UCRXIE;           // Set up SPI Rx IRQ
    UCA0IFG &= ~UCRXIFG;

    __enable_interrupt();       // Global enable

    while(1){} // Do nothing

}

#pragma vector = PORT4_VECTOR
__interrupt void ISR_PORT4_SW1(void){
    UCA0TXBUF = 0x10;           // Send a 0x10
    P4IFG &= ~BIT1;
}

#pragma vector = PORT2_VECTOR
__interrupt void ISR_PORT2_SW2(void){
    UCA0TXBUF = 0x66;           // Send a 0x66
    P2IFG &= ~BIT3;
}

#pragma vector = EUSCI_A0_VECTOR
__interrupt void ISR_EUSCI_A0_RX(void){
    Rx_Data = UCA0RXBUF;        // Read Rx buffer

    if(Rx_Data == 0x10){
        P1OUT ^= BIT0;          // Toggle LED1
    }else if(Rx_Data == 0x66){
        P6OUT ^= BIT6;          // Toggle LED2
    }

}
