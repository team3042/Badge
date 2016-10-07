#define DELAY_MS 100
#define DUTY_CYCLE 10
#define BRIGHTNESS 8

int led = 0; //attiny84 pin 13
//int led = 4; //attiny85 pin 3

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
}


// the loop routine runs over and over again forever:
void loop() {
  unsigned int morseWord[108] = {59530,47243,40961,56605,56436,29788,59,41610,47232,1885,51063,5568,696,48035,47803,32775,17493,50269,14,59630,58275,41696,373,4544,942,58299,35552,468,30577,30493,7637,56320,14882,44578,59392,30535,30493,7447,11,41656,59392,23645,7644,22301,7488,739,41896,117,4433,5959,14,48014,60971,35758,47104,29765,23621,53248,61070,60986,14894,29,50629,55056,235,47342,58040,117,53620,30172,30551,28672,59530,47243,40961,56605,56436,29788,42,11834,60928,7633,56775,30493,18260,30172,18293,30464,3720,43912,47616,7633,56775,18245,49155,35002,35752,92,46,41608,92,29813,10,41656,47672,117,56439,29020};
  byte i,j,k;
  for ( i=0; i<108; i++) {
    for ( j=0; j<=15; j++ ) {
      if ( (morseWord[i] & (1<<(15-j))) > 0 ) {
        digitalWrite(led, HIGH);
        delay(DELAY_MS);
      } else {
        digitalWrite(led, LOW);
        delay(DELAY_MS);
      }
    }
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(DELAY_MS*14);               // wait for a second
  }
}
