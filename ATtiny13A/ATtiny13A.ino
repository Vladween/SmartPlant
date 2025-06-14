#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

inline void update_outputs() {
  uint8_t input = PINB;
  PORTB = (PORTB & ~((1 << PB4) | (1 << PB3)))
    | ((input & (1 << PB1)) << (PB4 - PB1))
    | ((input & (1 << PB2)) << (PB3 - PB2));
}

int main()
{
  _delay_us(200);

  CLKPR = (1 << CLKPCE);
  CLKPR = (1 << CLKPS3);

  DDRB  = (1 << DDB4) | (1 << DDB3);
  PORTB = 0;

  ADCSRA = 0;
  TIMSK0 = 0;
  TCCR0B = 0;

  PCMSK = (1 << PCINT1) | (1 << PCINT2);
  GIMSK |= (1 << PCIE);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sei();
  update_outputs();

  while (1)
  {
    sleep_enable();
    sleep_cpu();
    sleep_disable();
  }
}

ISR(PCINT0_vect)
{
  GIFR |= (1 << PCIF);
  update_outputs();
}