/* Name: main.c
 * ATTiny84 Pinout                   +-----+
 *                             VCC - |1  14| - GND
 *                      Pin 10 PB0 - |2  13| - PA0 Pin 0 (Analog 0, AREF)
 *                       Pin 9 PB1 - |3  12| - PA1 Pin 1 (Analog 1 )
 *                       Reset PB3 - |4  11| - PA2 Pin 2 (Analog 2)
 *                 (PWM) Pin 8 PB2 - |5  10| - PA3 Pin 3 (Analog 3)
 *       (PWM, Analog 7) Pin 7 PA7 - |6   9| - PA4 Pin 4 (Analog 4, SCK)
 * (MOSI, PWM, Analog 6) Pin 6 PA6 - |7   8| - PA5 Pin 5 (Analog 5, PWM, MISO)
 *                                   +-----+
 * ATTiny85 Pinout
 *                                                                +-----+
 *                           (Digital 5, Reset, Analog 0, dW) PB5 |1   8| VCC
 * (PWM, Digital 3, XTAL1, CLKI, OC1B, Analog 3) Pin A3 Pin 3 PB3 |2   7| PB2 Pin A1 Pin 2(SCK, SCL, Analog 1, Digital 2)
 * (PWM, Digital 4, XTAL2, CLKO, OC1B, Analog 2) Pin A2 Pin 4 PB4 |3   6| PB1 Pin 1(PWM, MISO, Digital 1)
 *                                                            GND |4   5| PB0 Pin 0 (PWM, MOSI, SDA, Analog REF, Digital 0)
 *                                                                +-----+
 */
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h> /* for sei() */
#include <util/delay.h> /* for _delay_ms() */

#define BRIGHTNESS 64
#define DELAY_MS 250

volatile unsigned long milliseconds; //stores milliseconds from clock
unsigned long previousMillis = 0; // store the last time we changed

ISR(TIMER1_COMPA_vect)
{
  milliseconds++;
}

unsigned long millisLocal()
{
  return milliseconds;
}

void setup() 
{
  DDRB = 1<<DDB0;

  /* ************************************************************************
   * PWM setup
   * Timer 0
   * TTCR0A = Timer/Counter 0 Control Register A ( 1 of 2)
   *    7: COM0A1  6: COM0A0  5: COM0B1 4: COM0B0 3: - 2: - 1: WGM01  0: WGM00
   * WGM = Waveform Generator Mode
   * COM = Compare Output Mode
   * TTCR0B = 2nd Timer/Counter 0 Control Register B
   *    7: FOC0A  6: FOC0B  5: -  4: -  3: WGM02  2: CS02  1: CS01  0: CS00
   * FOC = Force Output Compare
   * CS = Clock Select
   * 
   * OCR = Output Compare Register
   */
  TCCR0A = 
    1<<WGM00 | 1<<WGM01 | //fast PWM 
    1<<COM0A1; //  clear OC0A (pin 5) on compare
  TCCR0B = 
    1<<WGM02 | // compare with OCR0A
    1<<CS00; // start timer, no prescaling for PWM
  OCR0A = BRIGHTNESS; //8-bit value represent pwm duty cycle

  /* ***********************************************************************
   * millis() setup
   * Timer 1
   * TCCR1 = Timer/Counter 1 Control Register
   *    7: CTC1  6: PWM1A  5: COM1A1  4: COM1A0  3: CS13  2: CS12  1: CS11  0: CS10
   * CTC = Clear Timer/Counter Compare Match
   * PWM1A = PWM A Enable
   * COM =  Comparator A Output Mode, Bits 1 and 0
   * CS = Clock Select, Bits 3,2,1, and 0
   * 
   * TIMSK = Timer/Counter Interrupt Mask Register
   */
  TCCR1 =  
    1<<CTC1 | //CTC Mode 
    1<<CS12 | 1<<CS11 | 1<<CS10; // CS = prescalar 64 
  OCR1C = 0.001 * F_CPU/4 - 1; //1ms, F_CPU @1Mhz, div64
  TIMSK |= 1<<OCIE1A; // Output Compare Interrupt Enable 1A

  sei();

}  // end of setup

int main (void) {
  uint8_t i=0; //integer for looping through integer array
  uint8_t j=0; //integer for looping the the bits in one 16-bit integer
  uint8_t morseLength = 9; // the number of integers in the morsePhrase array
  uint16_t morsePhrase[9] = {43554,59950,41915,32773,56439,29044,23879,20951,24000}; //Hello World!
  unsigned long currentMillis = millisLocal();
  setup();
  for(;;) { //main loop
    currentMillis = millisLocal();
    if ( currentMillis - previousMillis >= DELAY_MS) {
      previousMillis = currentMillis;
      if ( j == 15 && (i - 1) == morseLength ) { //done with the phrase
        j = 0;
        j = 0;
        _delay_ms(20 * DELAY_MS);
      } else if ( j == 15 ) { //done with this int
        j = 0;
        i++;
      } else { //next bit
        j++;
      }
    }
    if ( morsePhrase[i] & (1<<(15-j)) > 0 ) {
      OCR0A = BRIGHTNESS;
    } else {
      OCR0A = 0; //might have to invert PWM and set to 255 to turn the LED completely off
    }
  }
}
