#include "Wire.h"
#include <Deuligne.h>

// initialize the library with the numbers of the interface pins
Deuligne lcd;
const int pausePin = 1;
const int hoursDownPin = 2;
const int hoursUpPin = 3;
const int minutesDownPin = 4;
const int minutesUpPin = 5;

int seconds;
int minutes;
int hours;unsigned long previous;
unsigned long current;
bool pins[15];
bool paused;

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.init();
  reset();
  previous = 0;
  paused = false;

  for(int i = 0; i < sizeof(pins)/sizeof(*pins); i++) {
    pins[i] = false;
  }

  pinMode(pausePin, INPUT);
  pinMode(hoursUpPin, INPUT);
}

void loop() {
  if(!paused) {
    current = millis();
    if(current >= previous + 1000) {
      previous = current;
      incrementSeconds();
    }
  }
  if(getPinState(hoursDownPin) == HIGH) decrementHours();
  if(getPinState(hoursUpPin) == HIGH) incrementHours();
  if(getPinState(minutesDownPin) == HIGH) decrementMinutes();
  if(getPinState(minutesUpPin) == HIGH) incrementMinutes();

  if(getPinState(pausePin) == HIGH) {
      paused = !paused;
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

void decrementSeconds() {
  seconds++;
  if(seconds < 0) {
    seconds = 59;
  }
}

void incrementMinutes() {
  minutes++;
  if(minutes == 60) {
    minutes = 0;
    incrementHours();
  }
}

void decrementMinutes() {
  minutes--;
  if(minutes < 0) {
    minutes = 59;
  }
}

void incrementHours() {
  hours++;
  if(hours == 24) {
    hours = 0;
  }
}

void decrementHours() {
  hours--;
  if(hours < 0) {
    hours = 23;
  }
}

int getPinState(int num) {
  bool pressed = pins[num];
  if(digitalRead(num) == HIGH) {
    if(!pressed) {
      pins[num] = true;
      return HIGH;
    }
  } else {
    if(pressed) {
      pins[num] = false;
    }
  }
  return LOW;
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

