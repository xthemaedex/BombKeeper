/**
   Gunkook Park, s2204290
   This is the sketch for the timer. Timer can be set in 3:00, 10:00 and 15:00 and can be controlled by 3 buttons which are "countdown", "setting time" and "reset".
*/

#include <Wire.h>
#include <U8g2lib.h> // OLED display library
#include <Bounce2.h> // using Bounce2 library for buttons

#define SetButton 3 // pin D3
#define TimerButton 2 // pin D2
#define ToSecondArduino 12

Bounce deBouncerS = Bounce(); // instantiate "deBouncerS" as a bounce object
Bounce deBouncerT = Bounce(); // instantiate "deBouncerT" as a bounce object

// software i2c:
//U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(7, 8, U8X8_PIN_NONE);
// hardware i2c:
U8X8_SSD1306_128X64_NONAME_HW_I2C display(U8X8_PIN_NONE);

int buttonCount = 0; // declare "buttonCount" and set it in 0
int t = 0; // declare "t" and set it to 0
int tt = 60; // declare "tt" and set it to 60
int ttt = 30; // declare "ttt" and set it to 30

char buf[10]; // text buffer; to be able to use draw2x2String to show the value

void setup() {
  Serial.begin(9600);

  display.begin();
  display.setPowerSave(0);

  display.setFont(u8x8_font_pxplusibmcgathin_f);
  display.drawString(0, 0, "Set the time"); // print a String on line 0

  deBouncerS.attach(SetButton, INPUT_PULLUP); // attach the debouncer to "SetButton" pin with INPUT_PULLUP (use input pullup resistance) mode
  deBouncerS.interval(20); // set SetButton bounce interval in 20ms
  deBouncerT.attach(TimerButton, INPUT_PULLUP); // attach the debouncer to "TimerButton" pin with INPUT_PULLUP (use input pullup resistance) mode
  deBouncerT.interval(20); // set TimerButton bounce interval in 20ms
}

void loop() {
  deBouncerS.update(); // update the bounce instance for "deBouncerS"
  deBouncerT.update(); // update the bounce instance for "deBouncerT"

  if ( deBouncerS.fell() ) { // if the transistion of "SetButton" changes from HIGH to LOW (if "SetButton" is pressed)
    buttonCount++; // add 1 to "buttonCount"
  }

  if ( buttonCount == 0 ) { // if "buttonCount" is 0
    t = 180; // "t" becomes 180 (3 mins)
    display.clearLine(2); // clear line 2 of display
    displayCountdown(); // use "displayCountdown" method
  }

  if ( buttonCount == 1 ) { // if "buttonCount" is 1
    t = 600; // "t" becomes 600 (10 mins)
    display.clearLine(2); // clear line 2 of display
    displayCountdown(); // use "displayCountdown" method
  }

  if ( buttonCount == 2 ) { // if "buttonCount" is 2
    t = 900; // "t" becomes 900 (15 mins)
    display.clearLine(2); // clear line 2 of display
    displayCountdown(); // use "displayCountdown" method
  }

  if ( buttonCount == 3) { // if "buttonCount' is 3
    buttonCount = 0; // the value of "buttonCount" returns to 0
  }

  if ( buttonCount == 4) {
    display.clearLine(0);
    display.clearLine(2);
    display.draw2x2String(0, 3, "TimeOver");
  }

  if ( deBouncerT.fell() ) { // if the transistion of "TimerButton" changes from HIGH to LOW (if "TimerButton" is pressed)
    digitalWrite(ToSecondArduino, HIGH);
    display.clearLine(0);
    display.drawString(0, 0, "Hide the bomb");
    while (tt >= 0 ) {
      delay(1000);
      tt = tt - 1;
      display.clearLine(2);
      displayCountdown2();
      if ( tt == 0 ) {
        display.clearLine(0);
        display.drawString(0, 0, "Give the tracker");
        while ( ttt >= 0 ) {
          delay(1000);
          ttt = ttt - 1;
          display.clearLine(2);
          displayCountdown3();
          if ( ttt == 0 ) {
            display.clearLine(0);
            display.drawString(0, 0, "Find the bomb");
            Serial.print("t: ");
            Serial.println(t);
            while ( t >= 0 ) { // until the value of "t" becomes 0
              delay(1000); // delay 1 second
              t = t - 1; // "t" gets a new value which is "t-1"
              display.clearLine(2); // clear line 2 of display
              displayCountdown(); // use "displayCountdown" method
              if ( t == 0 ) {
                buttonCount = 4;
              }
            }
          }
        }
      }
    }
  }
  else digitalWrite(ToSecondArduino, LOW);
}

void displayCountdown() { // new method
  int min = t / 60; //
  int sec = t % 60; //
  sprintf(buf, "%02d:%02d", min, sec); //
  display.draw2x2String(2, 3, buf); //
}

void displayCountdown2() { // new method
  int min = tt / 60; //
  int sec = tt % 60; //
  sprintf(buf, "%02d:%02d", min, sec); //
  display.draw2x2String(2, 3, buf); //
}

void displayCountdown3() { // new method
  int min = ttt / 60; //
  int sec = ttt % 60; //
  sprintf(buf, "%02d:%02d", min, sec); //
  display.draw2x2String(2, 3, buf); //
}
