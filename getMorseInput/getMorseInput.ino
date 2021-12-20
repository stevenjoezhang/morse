// Turns Morse key into USB keyboard

#include <Bounce2.h> // include de-bounce library
#include <Keyboard.h>

const int led = 13; // led is connected to pin 13
const int keyPin = 7;  // morse key is connected to pin 7
Bounce morseKey = Bounce();

const unsigned long dashThresh = 150; // time threshold in ms to differentiate dots from dashes
const unsigned long letterThresh = 500; // time threshold in ms to differentiate letter gaps
const unsigned long wordThresh = 3000; // time threshold in ms to differentiate word gaps

String inputString = ""; // initialise input string

unsigned long downTime = 0; // records the start time of state change
unsigned long upTime = 0; // records the end time of state change
unsigned long timeNow = 0; // records the current time 
unsigned long changeDuration = 0; // records the duration of state change
unsigned long pauseDuration = 0; // records the duration of the last pause

int pauseFlag = 0; // initilise the flag to indicate whether a pause has already been evaluated

void setup()
{
  pinMode(led, OUTPUT); // configure the pin connected to the led as an output
  pinMode(keyPin, INPUT_PULLUP); // configure the pin connected to the morse key as a pullup
  morseKey.attach(keyPin);
  morseKey.interval(10); // 10 ms debounce
} // end of setup

void loop()
{ 
  checkPause();
  // start of IF loop
  if (morseKey.update()){
    
    if (morseKey.risingEdge()) { // if input from key has gone to 1 and model is still 0, update model

      keyUp();
  
    } else if (morseKey.fallingEdge()) { // if input from key has gone to 0 and model is still 1, update model
  
      keyDown();
  
    }
  } // end of if update loop
  
} // end of loop

void keyDown()
{
    downTime = millis();
    digitalWrite(led, HIGH); // switch LED on
}

void keyUp()
{
    upTime = millis();
    changeDuration = upTime-downTime; 
    digitalWrite(led, LOW); // switch LED off

    if (changeDuration>0 and changeDuration<dashThresh){
      inputString = inputString + ".";
      Serial.println("DOT");

    } else if (changeDuration>=dashThresh) {
      inputString = inputString + "-";
      Serial.println("DASH");

    }

    pauseFlag = 1;
    
}

void checkPause()
{   
    timeNow = millis();
    pauseDuration = timeNow-upTime;

    if (pauseDuration>=letterThresh and pauseDuration<wordThresh and pauseFlag){ // if the preceding pause was long enough AND a pause hasn't just been evaluated, evaluate the previous inputs as a single letter

      evaluateLetter();
      pauseFlag = 0;
      
    } else if (pauseDuration >= wordThresh and pauseFlag) {

      evaluateLetter();
      newWord();
      pauseFlag = 0; 
      
    }
}

void newWord()
{
  Keyboard.press(32);
  Keyboard.release(32);
}

void evaluateLetter()
{

  if (inputString==".-") {
      Keyboard.press('A');
      Keyboard.release('A');
  } else if (inputString=="-..."){
      Keyboard.press('B');
      Keyboard.release('B');
  } else if (inputString == "-.-."){
      Keyboard.press('C');
      Keyboard.release('C');
  } else if (inputString=="-.."){
      Keyboard.press('D');
      Keyboard.release('D');
  } else if (inputString=="."){
      Keyboard.press('E');
      Keyboard.release('E');
  } else if (inputString=="..-."){
      Keyboard.press('F');
      Keyboard.release('F');
  } else if (inputString=="--."){
      Keyboard.press('G');
      Keyboard.release('G');
  } else if (inputString=="...."){
      Keyboard.press('H');
      Keyboard.release('H');
  } else if (inputString==".."){
      Keyboard.press('I');
      Keyboard.release('I');
  } else if (inputString==".---"){
      Keyboard.press('J');
      Keyboard.release('J');
  } else if (inputString=="-.-"){
      Keyboard.press('K');
      Keyboard.release('K');
  } else if (inputString==".-.."){
      Keyboard.press('L');
      Keyboard.release('L');
  } else if (inputString=="--"){
      Keyboard.press('M');
      Keyboard.release('M');
  } else if (inputString=="-."){
      Keyboard.press('N');
      Keyboard.release('N');
  } else if (inputString=="---"){
      Keyboard.press('O');
      Keyboard.release('O');
  } else if (inputString==".--."){
      Keyboard.press('P');
      Keyboard.release('P');
  } else if (inputString=="--.-"){
      Keyboard.press('Q');
      Keyboard.release('Q');
  } else if (inputString==".-."){
      Keyboard.press('R');
      Keyboard.release('R');
  } else if (inputString=="..."){
      Keyboard.press('S');
      Keyboard.release('S');
  } else if (inputString=="-"){
      Keyboard.press('T');
      Keyboard.release('T');
  } else if (inputString=="..-"){
      Keyboard.press('U');
      Keyboard.release('U');
  } else if (inputString=="...-"){
      Keyboard.press('V');
      Keyboard.release('V');
  } else if (inputString==".--"){
      Keyboard.press('W');
      Keyboard.release('W');
  } else if (inputString=="-..-"){
      Keyboard.press('X');
      Keyboard.release('X');
  } else if (inputString=="-.--"){
      Keyboard.press('Y');
      Keyboard.release('Y');
  } else if (inputString=="--.."){
      Keyboard.press('Z');
      Keyboard.release('Z');
  } else if (inputString==".----"){
      Keyboard.press('1');
      Keyboard.release('1');
  } else if (inputString=="..---"){
      Keyboard.press('2');
      Keyboard.release('2');
  } else if (inputString=="...--"){
      Keyboard.press('3');
      Keyboard.release('3');
  } else if (inputString=="....-"){
      Keyboard.press('4');
      Keyboard.release('4');
  } else if (inputString=="....."){
      Keyboard.press('5');
      Keyboard.release('5');
  } else if (inputString=="-...."){
      Keyboard.press('6');
      Keyboard.release('6');
  } else if (inputString=="--..."){
      Keyboard.press('7');
      Keyboard.release('7');
  } else if (inputString=="---.."){
      Keyboard.press('8');
      Keyboard.release('8');
  } else if (inputString=="----."){
      Keyboard.press('9');
      Keyboard.release('9');
  } else if (inputString=="-----"){
      Keyboard.press('0');
      Keyboard.release('0');
  } else { 
      Keyboard.press('-');
      Keyboard.release('-');
  }

  inputString = ""; // re-initialise inputString ready for new letter

}



