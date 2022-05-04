/**
 * bomb.
 */
#include <U8g2lib.h>

U8X8_SSD1306_128X64_NONAME_HW_I2C display(U8X8_PIN_NONE);

#define BUZZER 9 // define buzzer pin
#define toneFreq 400

// variables for beep:
int countstep = 0;
int beeps = 1;

// variable for counting:
int count=-1; // -1 means not counting yet


// variables for communicaiton (recieve):
String line;
char buf[10]; // text buffer; to be able to use draw2x2String to show the value

void setup() {
  Serial.begin(9600);
  display.begin();
  display.setPowerSave(0);
  display.setFont(u8x8_font_pxplusibmcgathin_f);
  display.drawString(0,0,"Wait for count");
}




void loop() {
  // receive stuff
  while (Serial.available() > 0) { //Code that reads the bottles
    line = Serial.readString(); // read the data coming from the bottle/slave
  }
  if (line.length()>1) {
    if (line.startsWith("t")) { // did we receive the "t XXX"?
      // code to read count
      String word2 = getValue(line, ' ', 1); // get second part of line
      count = word2.toInt(); // convert the String to an integer
      countstep = count/20; // determine the amount of steps
      beeps = 1;
      delay(4000); //hide the bomb and give the tracker " message will be shown on the plate display 
      if (count>0) {
        display.clearLine(0);
        display.drawString(0,0,"Counting");
      }
    }

    line=""; // make line empty to be able to receive a next line
  }

  // count stuff
  if (count>=0) { // if counter not finished
    display.clearLine(3);
    display.clearLine(4);
    snprintf(buf, 10, "%2d", count);
    display.draw2x2String(4,3,buf); // show the counter
    /* here you could do stuff with leds, for example:
       if (count==10) turn on all 10 leds
       else if (count==9) turn off led10
       else if (count==8) turn off led9
       ... and so on */
    count = count - 1; // decrease counter

    // beep increasingly
    // calculate beep_delay, subtract that from 1 sec delay below
    int beep_delay=0;
    if (count % countstep == 0) {
       for (int i=0; i<beeps; i++) { // repeat beep
         tone(BUZZER, toneFreq, 1000); // short beep
         delay(100); // wait shortly
         beep_delay += 450; // add up the time of the beep + time of delay
       }
       beeps++; // beeps = beeps +1
    }
    
    if (count==-1) { // clear lines when counting finished
      display.clearLine(3);
      display.clearLine(4);
      display.drawString(0,0,"Wait for count");
      tone(BUZZER, toneFreq, 10000); // 10 secons long beep (bomb goes off)
    }
    if (beep_delay>1000) beep_delay=1000;
    delay(1000-beep_delay); // wait one second (or less depending on delay caused by beeping)
  }
  
}

// https://stackoverflow.com/questions/9072320/split-string-into-string-array
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
