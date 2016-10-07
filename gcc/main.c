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

// Define pins -- gcc uses different pin numbers than arduino studio
#define PIN_LED1  PB0
#define PORT  PORTB

// Define the delay time in ms
#define DELAY_MS  150

// Define helper macros
// write digital "high" to pin <pn> on port <prt>
#define DIGIWRITE_H(prt,pn) prt |= (1<<pn)
// write digital "low" to pin <pn> on port <prt>
#define DIGIWRITE_L(prt,pn) prt &= ~(1<<pn)

// program entry point
int main(void) {
  //Our morse phrase converted to unsigned 16-bit integers
  uint16_t  morseWord[109] = {59530,47243,40961,56605,56436,29788,59,41610,47232,1885,51063,5568,696,48035,47803,32775,17493,50269,14,59630,58275,41696,373,4544,942,58299,35552,468,30577,30493,7637,56320,14882,44578,59392,30535,30493,7447,11,41656,59392,23645,7644,22301,7488,739,41896,117,4433,5959,14,48014,60971,35758,47104,29765,23621,53248,61070,60986,14894,29,50631,23616,942,58299,35552,471,17873,55153,56669,49155,41514,57902,32775,29815,29137,53616,168,47339,47104,30535,30493,56437,7505,55153,7637,56320,14882,44578,59392,30535,30493,7447,14,8938,11936,368,186,35360,369,53716,42,35554,59616,471,29149,50545,30167};
  uint8_t morseLength = 109;

  uint8_t i, j, k = 0;

  // DDRB is the "data direction register" for port B
  // Here we OR it with the existing register settings
  DDRB |= (1 << PIN_LED1); //OR PIN_LED1 with DDRB and store answer in DDRB

  // initially set the pin to "low"
  DIGIWRITE_L(PORT,PIN_LED1);

  // main loop
  while(1){
    //loop through the integer array
    for( i=0; i<morseLength; i++ ) { 
      //loop through the bits of this integer
      for ( j=0; j<=15; j++ ) {
        //is the current bit set
        if ( (morseWord[i] & (1<<(15-j))) > 0 ) { //bitwise AND
          DIGIWRITE_H(PORT,PIN_LED1);
          _delay_ms(DELAY_MS);
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
