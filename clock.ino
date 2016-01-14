#include "Wire.h"
#include <Deuligne.h>

// initialize the library with the numbers of the interface pins
Deuligne lcd;
const int pausePin = 10;
const int upPin = 11;

unsigned int seconds;
unsigned int minutes;
unsigned int hours;
unsigned long previous;
unsigned long current;
bool paused;
bool pausePressed;
bool upPressed;

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.init();
  reset();
  previous = 0;
  paused = false;
  pausePressed = false;

  pinMode(pausePin, INPUT);
  pinMode(upPin, INPUT);
}

void loop() {
  if(!paused) {
    current = millis();
    if(current >= previous + 1000) {
      previous = current;
      incrementSeconds();
    }
  }
  else {
    if(digitalRead(upPin) == HIGH) {
      if(!upPressed) {
        upPressed = true;  
        incrementMinutes();
      }
    } else if(upPressed) {
      upPressed = false;
    }
  }
  if(digitalRead(pausePin) == HIGH) {
    if(!pausePressed) {
      pausePressed = true;
      paused = !paused;
    }
  } else if(pausePressed) {
    pausePressed = false;
  }
  lcd.setCursor(0, 0);
  lcd.print(toString(hours) + ":" + toString(minutes) + ":" + toString(seconds));

}

void reset() {
  hours = 0;
  minutes = 0;
  seconds = 0;
}

void incrementSeconds() {
  seconds++;
  if(seconds == 60) {
    incrementMinutes();
    seconds = 0;  
  }
}

void incrementMinutes() {
  minutes++;
  if(minutes == 60) {
    minutes = 0;
    incrementHours();
  }
}

void incrementHours() {
  hours++;
  if(hours == 24) {
    hours = 0;
  }
}

String toString(int num) {
  switch(num) {
    case 0:
      return String("00");

    case 1:
      return String("01");

    case 2:
      return String("02");

    case 3:
      return String("03");

    case 4:
      return String("04");

    case 5:
      return String("05");

    case 6:
      return String("06");

    case 7:
      return String("07");

    case 8:
      return String("08");

    case 9:      
      return String("09");

    default:            
      return String(num);
  }
}

