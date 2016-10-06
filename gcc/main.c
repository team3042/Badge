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
 */
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

// Define pins
#define PIN_LED1  PB0
#define PORT  PORTB

// Define the delay time in ms
#define DELAY_MS  100

// TOTAL PWM CYCLE - Lower number means less flicker
#define DUTY_CYCLE 10
// TOTAL LED ON - Must be less than  or equal to duty cycle
#define BRIGHTNESS 8

// Define helper macros

// write digital "high" to pin <pn> on port <prt>
#define DIGIWRITE_H(prt,pn) prt |= (1<<pn)

// write digital "low" to pin <pn> on port <prt>
#define DIGIWRITE_L(prt,pn) prt &= ~(1<<pn)


// program entry point
int main(void) {
  //Our morse phrase converted to unsigned 16-bit integers
  uint16_t  morseWord[103] = {59530,47243,40961,56605,56436,29788,59,41610,47232,1885,51063,5568,696,48035,47803,32775,17493,50269,14,59630,58275,41696,373,4544,942,58299,35552,468,30577,30493,7441,22289,29696,15267,48014,36491,32773,53596,29696,11822,36590,11150,36512,369,53716,58,34984,35747,32775,24007,30485,51012,21956,23808,3816,61155,41890,57345,56412,23921,14,48014,60971,32775,23831,18269,51061,30464,3720,43912,47616,7633,56775,18245,49154,41699,44768,477,7644,30577,54389,18269,50292,17756,17872,238,36590,14906,11776,7237,54365,16386,57345,29972,16386,58275,43008,21781,50641,49155,44771,48010,57344};

  uint8_t i, j, k = 0;

  // DDRB is the "data direction register" for port B
  // Here we OR it with the existing register settings
  DDRB |= (1 << PIN_LED1);

  // initially set the pin to "low"
  DIGIWRITE_L(PORT,PIN_LED1);

  // main loop
  while(1){
    //loop through the integer array
    for( i=0; i<(sizeof(morseWord)/sizeof(morseWord[0])); i++ ) { 
      //loop through the bits of this integer
      for ( j=0; j<=15; j++ ) {
        //is the current bit set
        if ( (morseWord[i] & (1<<(15-j))) > 0 ) {
          int8_t timeLeft = DELAY_MS;
          for ( k = 0; k >= DELAY_MS; k += DUTY_CYCLE ) {
            if ( timeLeft < DUTY_CYCLE ) {
              DIGIWRITE_H(PORT,PIN_LED1);
              _delay_ms(timeLeft);
            } else {
              DIGIWRITE_H(PORT,PIN_LED1);
              _delay_ms(DUTY_CYCLE - ( DUTY_CYCLE - BRIGHTNESS));
              DIGIWRITE_L(PORT,PIN_LED1);
              _delay_ms(DUTY_CYCLE - BRIGHTNESS);
            }
            timeLeft = timeLeft - DUTY_CYCLE;
          }
        } else {
          DIGIWRITE_L(PORT,PIN_LED1);
          _delay_ms(DELAY_MS);
        }
      }
    }
    DIGIWRITE_L(PORT,PIN_LED1);
    _delay_ms(DELAY_MS*14);//long off at the end
  }

  return 0;   /* never reached */
}
