/*
 * Control power on environmental monitor using most power efficient method
 * possible
 */

#define F_CPU 1000000UL

#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/delay.h>

/*
 * Need to set fuse to use 128MHz watchdog clock for system
 *
 * avrdude -p attiny2313 -c usbtiny -U lfuse:w:0x66:m -U hfuse:w:0xdf:m \
 * -U efuse:w:0xff:m
 */

#define TCNT1VAL 51474

ISR(TIMER1_OVF_vect)
{
}

int main(int argc, char * argv[])
{
	/* Enable output on port D, pin 4 */
	DDRD = _BV(PD4);
	PORTD |= _BV(PD4);

	/* Enable system clock prescaler to 256 */
	CLKPR = 0x08;
	CLKPR |= _BV(CLKPCE);

	/* Set timer1 prescaler to 1024 */
	TCCR1B |= (_BV(CS10) | _BV(CS12));

	while(1) {
		PORTD |= _BV(PD4);

		while(! (PIND & _BV(PD5))) {}

		PORTD &= ~_BV(PD4);

		/* initialize counter */
		TCNT1 = TCNT1VAL;

		/* enable overflow interrupt */
		TIMSK |= _BV(TOIE1);

		/* Enable Global Interrupts */
		sei();

		/* Set idle sleep mode and sleep */
		set_sleep_mode(SLEEP_MODE_IDLE);
		sleep_mode();

		/* disable overflow interrupt */
		TIMSK &= ~_BV(TOIE1);
	}

	return(0);
}
