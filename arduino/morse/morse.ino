#define DELAY_MS 80

int led = 10; //attiny84 pin 2
//int led = 4; //attiny85 pin 3

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
}


// the loop routine runs over and over again forever:
void loop() {
  unsigned int morseWord[109] = {59530,47243,40961,56605,56436,29788,59,41610,47232,1885,51063,5568,696,48035,47803,32775,17493,50269,14,59630,58275,41696,373,4544,942,58299,35552,468,30577,30493,7637,56320,14882,44578,59392,30535,30493,7447,11,41656,59392,23645,7644,22301,7488,739,41896,117,4433,5959,14,48014,60971,35758,47104,29765,23621,53248,61070,60986,14894,29,50631,23616,942,58299,35552,471,17873,55153,56669,49155,41514,57902,32775,29815,29137,53616,168,47339,47104,30535,30493,56437,7505,55153,7637,56320,14882,44578,59392,30535,30493,7447,14,8938,11936,368,186,35360,369,53716,42,35554,59616,471,29149,50545,30167};
  byte i,j,k;
  for ( i=0; i<109; i++) {
    for ( j=0; j<=15; j++ ) {
      if ( (morseWord[i] & (1<<(15-j))) > 0 ) {
        digitalWrite(led, HIGH);
        _delay_ms(DELAY_MS);
      } else {
        digitalWrite(led, LOW);
        _delay_ms(DELAY_MS);
      }
    }
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    _delay_ms(DELAY_MS*14);               // wait for a while
  }
}
