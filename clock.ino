#include "Wire.h"
#include <Deuligne.h>

// initialize the library with the numbers of the interface pins
Deuligne lcd;
const int pausePin = 1;
const int leftPin = 2;
const int rightPin = 3;
const int decrementPin = 4;
const int incrementPin = 5;

int seconds;
int minutes;
int hours;
int cursorPos;
unsigned long previous;
unsigned long blinkBegin;
bool pins[15];
bool paused;
int focused = 0;

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

struct Time {
  int seconds;
  int minutes;
  int hours;

  Time(int h, int m, int s): seconds(s), minutes(m), hours(h) {
  }

  void incrementSeconds(bool calculateMinutes) {
    seconds++;
    if(seconds >= 60) {
      seconds = 0;
      if(calculateMinutes) incrementMinutes(true);
    }
  }

  void incrementMinutes(bool calculateHours) {
    minutes++;
    if(minutes >= 60) {
      minutes = 0;
      if(calculateHours) incrementHours();
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
};

class TimePrinter
{
  int mPositionX, mPositionY;
  public:
    Time time;
    TimePrinter(int hours, int minutes, int seconds, int positionX, int positionY): time(hours, minutes, seconds)
    {
      mPositionX = positionX;
      mPositionY = positionY;
    }

    void print()
    {
      lcd.setCursor(mPositionX, mPositionY);
      lcd.print(toString(time.hours) + String(":") + toString(time.minutes) + String(":") + toString(time.seconds));
      lcd.setCursor(mPositionX + cursorPos * 3, mPositionY);
    }

    void increment()
    {
      switch(cursorPos)
      {
        case 0:
          time.incrementHours();
          break;

        case 1:
          time.incrementMinutes(false);
          break;

        case 2:
          time.incrementSeconds(false);
          break;
          
        default:
          time.incrementSeconds(false);
          break;
      }
      print();
    }

    void decrement()
    {
      switch(cursorPos)
      {
        case 0:
          time.decrementHours();
          break;

        case 1:
          time.decrementMinutes();
          break;

        case 2:
          time.decrementSeconds();
          break;

        default:
          time.decrementSeconds();
          break;
      }
      print();
    }

    void moveLeft()
    {
      if(cursorPos > 0)
      {
        cursorPos--;
        updateCursor();
      }
    }

    void moveRight()
    {
      if(cursorPos < 2)
      {
        cursorPos++;
        updateCursor();
      }
    }

    void updateCursor()
    {
      lcd.setCursor(mPositionX + cursorPos * 3, mPositionY);
    }
};

TimePrinter current(0, 0, 0, 0, 0);

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.init();
  // lcd.setCursor(0, 0);
  // lcd.print("00:00:00");
  previous = 0;
  cursorPos = 0;
  paused = false;

  for(int i = 0; i < sizeof(pins)/sizeof(*pins); i++) {
    pins[i] = false;
  }

  pinMode(pausePin, INPUT);
  pinMode(rightPin, INPUT);
  current.print();
}

void loop() {
  if(!paused) {
    if(millis() >= previous + 1000) {
      previous += 1000;
      current.time.incrementSeconds(true);
      current.print();
    }
  } else {
    previous = millis();
    if(getPinState(leftPin) == HIGH) current.moveLeft();
    if(getPinState(rightPin) == HIGH) current.moveRight();
    if(getPinState(decrementPin) == HIGH) {
      current.decrement();
    }
    if(getPinState(incrementPin) == HIGH) {
      current.increment();
    }
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
