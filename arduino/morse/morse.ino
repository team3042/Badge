#define DELAY_MS 120
#define ARRAY_SIZE 4
//int led = 10; //attiny84 pin 2
int led = 4; //attiny45 pin 3

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
}

// the loop routine runs over and over again forever:
void loop() {
  uint16_t morseWord[ARRAY_SIZE] = {60323,48014,43192,47758};
  byte i,j=0;
  long lastTime, currentTime;
  for ( i = 0; i<ARRAY_SIZE; i++ ) {
    if ( (morseWord[i] & (1<<(15-j))) > 0 ) {
      digitalWrite(led, HIGH);
    } else {
      digitalWrite(led, LOW);
    }
    currentTime = millis();
    if ( currentTime >= ( lastTime + DELAY_MS ) ) {
      lastTime = millis();
      if ( i == ( ARRAY_SIZE -1 ) && j == 15 ) { //we're done
        digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
        delay(10000);
      }
      if ( j == 15 ) {
        j = 0;
      } else {
        j++;
      }
    }
  }
}
