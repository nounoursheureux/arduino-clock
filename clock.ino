#include "Wire.h"
#include <Deuligne.h>

struct Time {
  int seconds;
  int minutes;
  int hours;

  Time(int h=0, int m=0, int s=0): seconds(s), minutes(m), hours(h) {
  }

  void incrementSeconds() {
    seconds++;
    if(seconds >= 60) {
      seconds = 0;
      incrementMinutes();
    }
  }

  void incrementMinutes() {
    minutes++;
    if(minutes >= 60) {
      minutes = 0;
      incrementHours();
    }
  }

  void incrementHours() {
    hours++;
    if(hours >= 24) {
      hours = 0;
    }
  }

  void decrementSeconds() {
    seconds--;
    if(seconds < 0) {
      seconds = 59;
    }
  }

  void decrementMinutes() {
    minutes--;
    if(minutes < 0) {
      minutes = 59;
    }
  }

  void decrementHours() {
    hours--;
    if(hours < 0) {
      hours = 23;
    }
  }

  void reset(int h=0, int m=0, int s=0) {
    hours = h;
    minutes = m;
    seconds = s;
  }
};


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


struct TimePrinter {
  Time t;
  Deuligne* lcd;
  int posX, posY;
  int offset;

  TimePrinter(Deuligne* l, int x, int y, int hours=0, int minutes=0, int seconds=0): lcd(l), posX(x), posY(y), t(hours, minutes, seconds) {
    offset = 0;
  }

  void init() {
    lcd->setCursor(posX, posY);
    lcd->print("00:00:00");
  }

  void updateCursor() {
    lcd->setCursor(posX + offset * 3, posY);  
  }
  
  void incrementHours() {
    t.incrementHours();
    printHours();
  }

  void decrementHours() {
    t.decrementHours();
    printHours();
  }

  void printHours() {
    offset = 0;
    updateCursor();
    lcd->print(toString(t.hours));
    updateCursor();
  }

  void incrementMinutes() {
    t.incrementMinutes();
    printMinutes();
  }

  void decrementMinutes() {
    t.decrementMinutes();
    printMinutes();
  }

  void printMinutes() {
    offset = 1;
    updateCursor();
    lcd->print(toString(t.minutes));
    updateCursor();
  }

  void incrementSeconds() {
    t.incrementSeconds();
    printSeconds();
  }

  void decrementSeconds() {
    t.decrementSeconds();
    printSeconds();
  }

  void printSeconds() {
    offset = 2;
    updateCursor();
    lcd->print(toString(t.seconds));
    updateCursor();
  }

  void moveLeft() {
    if(offset > 0) {
      offset--;
      updateCursor();
    }
  }

  void moveRight() {
    if(offset < 2) {
      offset++;
      updateCursor();
    }
  }

  void increment() {
    switch(offset) {
      case 0:
        incrementHours();
        break;

      case 1:
        incrementMinutes();
        break;

      case 2:
        incrementSeconds();
        break;

      default:
        break;
    }
  }

  void decrement() {
    switch(offset) {
      case 0:
        decrementHours();
        break;

      case 1:
        decrementMinutes();
        break;

      case 2:
        decrementSeconds();
        break;

      default:
        break;
    }
  }
};

// initialize the library with the numbers of the interface pins
Deuligne lcd;
TimePrinter current(&lcd, 0, 0);
const int pausePin = 1;
const int leftPin = 2;
const int rightPin = 3;
const int decrementPin = 4;
const int incrementPin = 5;

unsigned long previous;
bool pins[15];
bool paused;

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.init();
  current.init();
  previous = 0;
  paused = false;

  for(int i = 0; i < sizeof(pins)/sizeof(*pins); i++) {
    pins[i] = false;
  }

  pinMode(pausePin, INPUT);
  pinMode(rightPin, INPUT);
}

void loop() {
  if(!paused) {
    if(millis() >= previous + 1000) {
      previous += 1000;
      current.incrementSeconds();
    }
  } else {
    previous = millis();
    if(getPinState(leftPin) == HIGH) current.moveLeft();
    if(getPinState(rightPin) == HIGH) current.moveRight();
    if(getPinState(decrementPin) == HIGH) current.decrement();
    if(getPinState(incrementPin) == HIGH) current.increment();
  }
  if(getPinState(pausePin) == HIGH) {
    paused = !paused;
    if(paused) lcd.cursor();
    else lcd.noCursor();
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
