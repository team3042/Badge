#include <TinyDebugSerial.h>
#include <CapacitiveSensor.h>
#include <avr/io.h>  //device specific io definitions
#include <avr/interrupt.h> /* for sei() */

#define BRIGHTNESS 64
#define DELAY_MS 250

TinyDebugSerial mySerial = TinyDebugSerial();

CapacitiveSensor cs_1_2 = CapacitiveSensor(1,2); //10 Megaohm R between Arduino 1 & 2 ( chip 6 & 7, respectively) sensor pad on chip pin 7
unsigned long csSum; //to accumulate cap sense touches

volatile unsigned long milliseconds; //stores milliseconds from clock

const byte morseLength = 9; // the number of integers in the morsePhrase array
const unsigned int morsePhrase[morseLength] = {43554,59950,41915,32773,56439,29044,23879,20951,24000}; //Hello World!
unsigned long previousMillis = 0; // store the last time we changed
byte i=0; //integer for looping through integer array
byte j=0; //integer for looping the the bits in one 16-bit integer

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
  pinMode (0, OUTPUT);  // arduino pin 0, chip pin 5, OC0A
  
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
    _BV(WGM00) | _BV(WGM01) | //fast PWM 
    _BV(COM0A1); //  clear OC0A (pin 5) on compare
  TCCR0B = 
    _BV(WGM02) | // compare with OCR0A
    _BV(CS00); // start timer, no prescaling for PWM
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
    _BV(CTC1) | //CTC Mode 
    _BV(CS12) | _BV(CS11) | _BV(CS10); // CS = prescalar 64 
  OCR1C = 0.001 * F_CPU/4 - 1; //1ms, F_CPU @1Mhz, div64
  TIMSK |= _BV(OCIE1A); // Output Compare Interrupt Enable 1A

  sei();

  mySerial.begin(9600); //for debugging on pin 2

}  // end of setup

void loop() {
  // Check to see if the capacitive sensor has been triggered
 int trigger = CSread();
 if ( trigger > 0 ) {
    // time to switch messages
 }
 
 // check to see if it's time to change the led status
 unsigned long currentMillis = millisLocal();
 mySerial.println(currentMillis);
  if ( currentMillis - previousMillis >= DELAY_MS) { //time for the next bit
    previousMillis = currentMillis;
    if ( j == 15 && (i - 1) == morseLength ) { //done with the phrase - reset to the beginning
      j = 0;
      j = 0;
      while ( (currentMillis - previousMillis) < (DELAY_MS * 10 ) ) {
        currentMillis = millisLocal();
      }
    } else if ( j == 15 ) { //done with this int
      j = 0;
      i++;
    } else { //next bit
      j++;
    }
  }

  //set the LED on or off
  if ( morsePhrase[i] & (1<<(15-j)) > 0 ) {
    OCR0A = BRIGHTNESS;
  } else {
    OCR0A = 0; //might have to invert PWM and set to 255 to turn the LED completely off
  }
}
/* Capacitive Sensor read */
int CSread() {
  long cs = cs_1_2.capacitiveSensor(80); //sensor resolution is set to 80
  if ( cs > 100 ) { //arbitrary
     csSum += cs;
     if ( csSum >= 3800 ) { // threshold
        //trigger the change
        return 1;
     } else {
      csSum = 0; // timeout caused by bad readings
     }
     return 0;
  }
}
