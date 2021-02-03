#include <TM1637Display.h>
const int CLK = 10;
const int DIO = 11;
TM1637Display display(CLK, DIO);  //set up the 4-Digit Display.


const int ledPin = 13; //sync with relay
const int rlyPin = 9;  //relay pin
const int second = 1000;
const int buttonPin1 = 2; //encoder A
const int buttonPin2 = 3; //encoder B


int ledState = LOW;  //sync with relay
int buttonState;
int aState;
int lastButtonState = LOW;
unsigned long previousMillis=0;

 

void setup() {
    pinMode(buttonPin1, INPUT);
    attachInterrupt(0, count, CHANGE);   // interrupt 0 digital pin 2 positive edge trigger (Arduino nano & micro)
    attachInterrupt(1, count, CHANGE);   // interrupt 1 digital pin 3 positive edge trigger
    pinMode(buttonPin2, INPUT);
  
    pinMode(ledPin, OUTPUT);
    pinMode(rlyPin, OUTPUT);
    digitalWrite(ledPin, ledState);
    digitalWrite(rlyPin, !ledState);

    display.setBrightness(0);  //@BRIGHT_DARKEST = 0,BRIGHTEST = 7; / @true=Turn on display, false=Turn off display;
}


volatile long int seconds = 0;
volatile byte aLastState;
void count() {
   aState = digitalRead(buttonPin1); // Reads the "current" state of the outputA

    // If the previous and the current state of the outputA are different, that means a Pulse has occured
    if (aState != aLastState){     
         
         // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
         if (digitalRead(buttonPin2) != aState) { 
           seconds += 10*60+1;
         } else {
           seconds -= 10*60;
         }
        
     
        if (seconds <= 0) {
            seconds = 3;
        } else if (seconds>43200) {  //max 12 hour
            seconds = 43200;
        }
    
    }

    aLastState = aState; // Updates the previous state of the outputA with the current state
}

void loop() {
    runTimer();
    showTime();


    digitalWrite(ledPin, ledState);
    digitalWrite(rlyPin, !ledState);
}

void runTimer(){
    unsigned long currentMillis = millis();
    if ((unsigned long)(currentMillis - previousMillis) >= second) {
        previousMillis = currentMillis;
        if (seconds>0){
          seconds--;
          ledState = true;        
          if (seconds == 0) 
            ledState = false;         
        }
    }

}

void showTime(){
    if (seconds=60){      
        long int t = seconds;
        //seconds = t % 60;
        t /= 60; // now it is minutes
        int minutes = t % 60;
        t /= 60; // now it is hours
        int hours = t % 24;    
        char fullTime[5];
        sprintf(fullTime, "%02d%02d", hours, minutes);
        int r=atoi( fullTime );
        display.showNumberDec(r, true, 4); //Display the Variable value;
    }
    else
    {
        display.showNumberDec(seconds);
    }
}
