//Define the LED Pin
#define PIN_OUT 0
//Define unit length in ms
#define UNIT_LENGTH 100
//string
#define MORSE
void setup()
{
  pinMode( PIN_OUT, OUTPUT );
  digitalWrite( PIN_OUT, LOW );
}

void loop()
{
  String morseWord = "-. . ...- . .-.|--. --- -. -. .- | --. .. ...- . | -.-- --- ..- | ..- .--. --..-- | -. . ...- . .-. | --. --- -. -. .- | .-.. . - | -.-- --- ..- | -.. --- .-- -.";
  for(int i=0; i<=morseWord.length(); i++) {
    switch( morseWord[i] )
    {
      case '.': //dit
        digitalWrite( PIN_OUT, HIGH );
        delay( UNIT_LENGTH );
        digitalWrite( PIN_OUT, LOW );
        delay( UNIT_LENGTH );
        break;
      case '-': //dah
        digitalWrite( PIN_OUT, HIGH );
        delay( UNIT_LENGTH*3 );
        digitalWrite( PIN_OUT, LOW );
        delay( UNIT_LENGTH );
        break;
      case '|': //word
        delay( UNIT_LENGTH*7 );
      case ' ': //gap
        delay( UNIT_LENGTH*2 );
    }
  }
// signal end of string
  digitalWrite ( PIN_OUT, LOW );
  delay( UNIT_LENGTH*20 );
}
