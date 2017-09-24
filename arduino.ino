#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal.h>
RTC_DS3231 rtc;

int chrs;          // Current Hours
int cmins;         // Current Mins
int csecs;         // Current Seconds
int thrs;          // Target Hours
int tmins;         // Target Mins
int tsecs;         // Target Seconds
int duration;      // Duration of Pump Cycle
int CO2 = A0;      // CO2 Relay Output Pin

int swtog;        // Toggle Function
int swtogip = 6;  // Switch for Toggle Function
int swhrs;        // Hrs Change
int swhrsip = 7;  // Switch for Hours Change
int swmins;       // Mins Change
int swminsip = 8; // Switch for Mins Change
int swsecs;       // Seconds Change
int swsecsip = 9; // Switch for Seconds Change
int prime;        // Prime Pump
int swprimeip = 10; // Switch for Prime Pump

// Initialise LCD library & configuration
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  lcd.begin(16, 2);       // Set up the LCD's number of columns and rows:
  rtc.begin();            // Begin RTC comms
  pinMode(CO2, OUTPUT);   // Set CO2 Relay pin as Output
  digitalWrite(CO2, LOW); // Set CO2 Pump to off
  pinMode(swtogip, INPUT);
  pinMode(swhrsip, INPUT);
  pinMode(swminsip, INPUT);
  pinMode(swsecsip, INPUT);
  swtog = LOW;            // Ensure all outputs are 'Low' on initial run
  swhrs = LOW;
  swmins = LOW;
  swsecs = LOW;
  chrs = 00;              // Erase all memory settings
  cmins = 00;
  csecs = 00;
  duration = 1;           // Set default duration
  thrs = 14;              // Set default trigger time
  tmins = 0;
  tsecs = 0;
}

void loop() {
   lcd.setCursor(0,0);           // Set to starting write location
   DateTime now = rtc.now();     // Align Arduino clock to RTC clock
   lcd.print("Date: ");          // OUTPUT DATE & TIME - FILLED WITH APPROPRIATE
   lcd.print(now.day(), DEC);
   lcd.print("/");
   lcd.print(now.month(), DEC);
   lcd.print("/");
   lcd.print(now.year(), DEC);
   lcd.print("   ");
   lcd.setCursor(0,1);
   lcd.print("Time: ");
   lcd.print(now.hour(), DEC);
   lcd.print(":");
   lcd.print(now.minute(), DEC);
   lcd.print(":");
   lcd.print(now.second(), DEC);
   lcd.print("   ");
   lcd.display();

   csecs = (now.second());        // Update current time to mem locations
   cmins = (now.minute());
   chrs = (now.hour());

  prime = digitalRead(swprimeip);     // Prime Loop Begins
  while (prime == HIGH){
    prime = digitalRead(swprimeip);
    digitalWrite(CO2, HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Priming");
    lcd.setCursor(0,1);
    lcd.print("Pump");
  }
  digitalWrite(CO2, LOW);
  
  swtog = digitalRead(swtogip);       // Set pump time loop
while (swtog == HIGH){
  swtog = digitalRead(swtogip);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Current Set Trigger Time:");
  lcd.setCursor(0,1);
  lcd.print(thrs, DEC);
  lcd.print(":");
  lcd.print(tmins, DEC);
  lcd.print(":");
  lcd.print(tsecs, DEC);
  swhrs = digitalRead(swhrsip);
  if (swhrs == HIGH){
    thrs = thrs + 1;
    if (thrs == 24){
    thrs = 0;
    }
    delay(200);
    }
  swmins = digitalRead(swminsip);
  if (swmins == HIGH){
    tmins = tmins + 1;
    if (tmins == 60){
    tmins = 0;
    }
    delay(200);
    }
  swsecs = digitalRead(swsecsip);
  if (swsecs == HIGH){
    tsecs = tsecs + 1;
    if (tsecs == 60){
    tsecs = 0;
    }
    delay(200);
    }
  delay(100);
  }


swhrs = digitalRead(swhrsip);           // Set pump duration loop
while (swhrs == HIGH){
  swhrs = digitalRead(swhrsip);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Run Duration (s):");
  lcd.setCursor(0,1);
  lcd.print(duration, DEC);
  swsecs = digitalRead(swsecsip);
  if (swsecs == HIGH){
    duration = duration + 1;
    if (duration == 91){
    duration = 1;
    }
    delay(200);
    }
    delay(100);
}

if (csecs == tsecs){                      // Pump running loop (target time = current time)
  if (cmins == tmins){
    if (chrs == thrs){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("O/P for ");
        lcd.print(duration, DEC);
        lcd.print(" seconds");
        duration = duration * 1000;
        digitalWrite(CO2, HIGH);
        delay(duration);
        digitalWrite(CO2, LOW);
        duration = duration / 1000;
      }
  }

}

}
