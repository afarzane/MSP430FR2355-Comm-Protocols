#include <msp430.h> 

int i;
int j;
char message[] = "Hello World!";
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

	        PM5CTL0 &= ~ LOCKLPM5;       // Turn on GPIO
	        UCA1CTLW0   &= ~UCSWRST;    // Turn off Software reset for UART A1

	        // Main loop
	        while(1){
	            for(i=0; i<sizeof(message); i++){
	                UCA1TXBUF = message[i];       // Send 0x55 out over UART A1
	                for(j=0; j<10000; j++){
	                                    // Delay between frames
	                }
	            }
	        }
	}
