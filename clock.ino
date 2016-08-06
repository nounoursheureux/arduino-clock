#include "Wire.h"
#include <Deuligne.h>

// initialize the library with the numbers of the interface pins
const int pauseCurrentPin = 1;
const int pauseAlarmPin = 2;
const int moveCursorPin = 3;
const int decrementPin = 4;
const int incrementPin = 5;

const byte ARROW_CHAR = 1;

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

  bool operator==(const Time& t2)
  {
    return hours == t2.hours && minutes == t2.minutes && seconds == t2.seconds; 
  }
};

/* class TimePrinter
{
  int mPositionX, mPositionY;
  
  public:
    Time time;
    bool paused;
    
    TimePrinter(int hours, int minutes, int seconds, int positionX, int positionY): time(hours, minutes, seconds)
    {
      paused = false;    
      mPositionX = positionX;
      mPositionY = positionY;
    }

    virtual void update() = 0;
    
    void print()
    {
      lcd.setCursor(mPositionX, mPositionY);
      lcd.print(toString(time.hours) + String(":") + toString(time.minutes) + String(":") + toString(time.seconds));
      updateCursor();
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

    void moveCursor()
    {
      cursorPos++;
      if(cursorPos > 2) cursorPos = 0;
      updateCursor();
    }

    void updateCursor()
    {
      lcd.setCursor(mPositionX + cursorPos * 3, mPositionY);
    }

    void togglePause()
    {
      if(!paused && canPause)
      {
        paused = true;
        lcd.cursor();
        updateCursor();

      }
      else if(paused)
      {
        paused = false;
        lcd.noCursor();
      }
    }
};

class RunningTimePrinter: public TimePrinter
{
  private:
    unsigned long previous = 0;
    
  public:
    RunningTimePrinter(int h, int m, int s, int x, int y): TimePrinter(h, m, s, x, y)
    {
      
    }
    
    void update()
    {
      if(!paused)
      {
        if(millis() >= previous + 1000)
        {
          previous += 1000;
          time.incrementSeconds(true);
          if(canPause) print();
        }
      } else {
        previous = millis();
        if(getPinState(moveCursorPin) == HIGH) moveCursor();
        if(getPinState(decrementPin) == HIGH) decrement();
        if(getPinState(incrementPin) == HIGH) increment();
      }
      
      if(getPinState(pauseCurrentPin) == HIGH) {
        togglePause();
      }
    }
};

RunningTimePrinter current(0, 0, 0, 0, 0);

class AlarmTimePrinter: public TimePrinter
{
  public:
    AlarmTimePrinter(int h, int m, int s, int x, int y): TimePrinter(h, m, s, x, y)
    {
    }
    
    void update()
    {
      if(paused)
      {
        if(getPinState(moveCursorPin) == HIGH) moveCursor();
        if(getPinState(decrementPin) == HIGH) decrement();
        if(getPinState(incrementPin) == HIGH) increment();
      }

      // if(time == current.time) lcd.noDisplay();
      
      if(getPinState(pauseAlarmPin) == HIGH) {
        togglePause();
      }
    }
}; */

class Clock
{
  Deuligne* lcd;
  Time current;
  Time alarm;
  bool alarm_enabled;
  unsigned long previous_millis;
  bool current_paused;
  bool alarm_paused;
  int editing;
 
  public:
    Clock(Deuligne* p_lcd, int hours, int minutes, int seconds): lcd(p_lcd), current(hours, minutes, seconds), alarm(0, 0, 0), alarm_enabled(false), previous_millis(0), current_paused(false), alarm_paused(false), editing(0)
    {
    }

    void print()
    {
      lcd->setCursor(0, 0);
      lcd->print(toString(current.hours) + String(":") + toString(current.minutes) + String(":") + toString(current.seconds));
      if (current_paused)
      {
        lcd->setCursor(0, 1);
        for (int i = 0; i < 9; ++i)
        {
          if (i == editing * 3)
          {
            lcd->write(ARROW_CHAR);
          }
          else
          {
            lcd->print(" ");
          }
        }
        lcd->write(ARROW_CHAR);
      }
    }

    void update()
    {
      if (current_paused)
      {
        previous_millis = millis();
      } 
      else if (previous_millis + 1000 <= millis())
      {
        current.incrementSeconds(true);
        previous_millis += 1000;
      }

      print();

      if (getPinState(pauseCurrentPin) == HIGH)
      {
        current_paused = !current_paused;
      }

      if (current_paused && getPinState(moveCursorPin) == HIGH)
      {
        editing++;
        if (editing > 2)
        {
          editing = 0;
        }
      }
    }
};

// AlarmTimePrinter alarm(0, 0, 0, 2, 1);
unsigned long blinkBegin;
bool pins[15];
Deuligne lcd;
Clock clock(&lcd, 0, 0, 0);

byte arrow[8] = {
  B00100,
  B01110,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000
};

void setup() {
  lcd.init();
  lcd.createChar(1, arrow);
  
  for(int i = 0; i < sizeof(pins)/sizeof(*pins); i++) {
    pins[i] = false;
  }

  pinMode(pauseCurrentPin, INPUT);
  pinMode(pauseAlarmPin, INPUT);
  pinMode(moveCursorPin, INPUT);
  pinMode(incrementPin, INPUT);
  pinMode(decrementPin, INPUT);
  /* current.print();
  alarm.print(); */
  clock.print();
}

void loop() {
  /* current.update();
  alarm.update();
  canPause = !(current.paused || alarm.paused); */
  clock.update();
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
