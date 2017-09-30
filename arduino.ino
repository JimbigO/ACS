// -------------------------------------------------------------------------
// Aquarium Control System V1.02
// 30/01/17
// Created by Craig Sellwood & James Cooper
// -------------------------------------------------------------------------

#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal.h>
RTC_DS3231 rtc;

int current_hours;                // Current Hours
int current_minutes;              // Current Mins
int current_seconds;              // Current Seconds
int target_hours;                 // Target Hours
int target_minutes;               // Target Mins
int target_seconds;               // Target Seconds
int duration;                     // Duration of Pump Cycle
int Rly_1 = A12;                  // Relay 1 Output 
int Rly_2 = A13;                  // Relay 2 Output
int Rly_3 = A14;                  // Relay 3 Output
int Rly_4 = A15;                  // Relay 4 Output
int Button_Var;                   // LCD Button variable value 0 - 1024

boolean up;                       // LCD Up button
boolean down;                     // LCD Down Button
boolean left;                     // LCD Left Button
boolean right;                    // LCD Right Button
boolean select;                   // LCD Select Button
boolean standby;                  // LCD No Buttons Pressed

int switch_toggle;                // Toggle Function
int switch_toggle_ip = A1;        // Switch for Toggle Function
int swhrs;                        // Hours Change
int swhrsip = A2;                 // Switch for Hours Change
int swmins;                       // Minutes Change
int swminsip = A3;               // Switch for Minutes Change
int swsecs;                       // Seconds Change
int swsecsip = A4;               // Switch for Seconds Change
int prime;                        // Prime Pump
int swprimeip = A5;              // Switch for Prime Pump

// Initialise LCD library & configuration
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  lcd.begin(16, 2);               // Set up the LCD's number of columns and rows:
  rtc.begin();                    // Begin RTC comms
  pinMode(Rly_1, OUTPUT);         // Set Rly_1 Relay pin as Output
  digitalWrite(Rly_1, LOW);       // Set Rly_1 Pump to off
  pinMode(switch_toggle_ip, INPUT);
  pinMode(swhrsip, INPUT);
  pinMode(swminsip, INPUT);
  pinMode(swsecsip, INPUT);
  switch_toggle = LOW;            // Ensure all outputs are 'Low' on initial run
  swhrs = LOW;
  swmins = LOW;
  swsecs = LOW;
  up = false;
  down = false;
  left = false;
  right = false;
  select = false;
  standby = false;

  current_hours = 00;              // Erase all memory settings
  current_minutes = 00;
  current_seconds = 00;
  duration = 1;                   // Set default duration
  target_hours = 14;              // Set default trigger time
  target_minutes = 0;
  target_seconds = 0;
  
}

//------------------------------- Main Program Loop -----------------------------------------------------------------

void loop() {
 
  DateTime now = rtc.now();     // Align Arduino clock to RTC clock
  lcd.setCursor(0,0);           // Set to starting write location Top Line
  lcd.print("Date: ");          // OUTPUT DATE & TIME to LCD
  lcd.print(now.day(), DEC);    // Print current day DD
  lcd.print("/");               // Add / Seperator
  lcd.print(now.month(), DEC);  // Print current month MM
  lcd.print("/");               // Add / Seperator
  lcd.print(now.year(), DEC);   // Print current year YY
  lcd.print("   ");
  lcd.setCursor(0,1);           // Set to starting write location Bottom Line
  lcd.print("Time: ");          // Print Time: header to LCD
  lcd.print(now.hour(), DEC);   // Print current Hour HH
  lcd.print(":");               // Print : seperator
  lcd.print(now.minute(), DEC); // Print current Minute MM
  lcd.print(":");               // Print : seperator
  lcd.print(now.second(), DEC); // Print current Second SS
  lcd.print("   ");
  lcd.display();

  //------------------------------ Monitor LCD Push Buttons ------------------------------------------------------------
  
  Button_Var = analogRead(0);
  if (Button_Var < 60) {                                      // Right LCD Button Pressed
  right = true;                              
  }
  else if (Button_Var > 75 && Button_Var < 150) {             // Up LCD Button Pressed
  up = true;                              
  }
  else if (Button_Var > 100 && Button_Var <275){              // Down LCD Button Pressed
  down = true;                              
  }
  else if (Button_Var > 375 && Button_Var < 450) {            // Left LCD Button Pressed
  left = true;                              
  }
  else if (Button_Var > 600 && Button_Var < 700) {            // Select LCD Button Pressed
  select = true;                              
  }
  else if (Button_Var < 999) {                                // No Button Pressed
  standby = true;
  delay(200);
  up = false;
  down = false;
  left = false;
  right = false;
  }

    //--------------------------Update Memory Locations from RTC-----------------------------------------------------------

  current_seconds = (now.second());                        // Update current seconds to memory location current_seconds
  current_minutes = (now.minute());                        // Update current minutes to memory location current_minutes
  current_hours = (now.hour());                            // Update current hours to memory location current_hours

    // ------------------------------------ Test Relays --------------------------------------------------------------------

  while (select == true && left == true){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Testing Relay 1");
  digitalWrite(Rly_1, LOW);
  delay(5000);
  }

  while (select == true && up == true){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Testing Relay 2");
  digitalWrite(Rly_2, LOW);
  delay(5000);
  }

  while (select == true && right == true){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Testing Relay 3");
  digitalWrite(Rly_3, LOW);
  delay(5000);
  }

  while (select == true && down == true){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Testing Relay 4");
  digitalWrite(Rly_4, LOW);
  delay(5000);
  }

  while (standby == true){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Standby State");
  delay(5000);
  digitalWrite(Rly_1, HIGH);
  digitalWrite(Rly_2, HIGH);
  digitalWrite(Rly_3, HIGH);
  digitalWrite(Rly_4, HIGH);
  }

    // ------------------------------- Pump Prime Sequence ----------------------------------------------------------------

  prime = digitalRead(swprimeip);
  while (prime == HIGH){
    prime = digitalRead(swprimeip);
    digitalWrite(Rly_1, HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Priming");
    lcd.setCursor(0,1);
    lcd.print("Pump");
  }
  digitalWrite(Rly_1, LOW);
  
  switch_toggle = digitalRead(switch_toggle_ip);             // Set pump time loop
  while (switch_toggle == HIGH){
    switch_toggle = digitalRead(switch_toggle_ip);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Current Set Trigger Time:");
    lcd.setCursor(0,1);
    lcd.print(target_hours, DEC);
    lcd.print(":");
    lcd.print(target_minutes, DEC);
    lcd.print(":");
    lcd.print(target_seconds, DEC);
    swhrs = digitalRead(swhrsip);
    
    if (swhrs == HIGH){
      target_hours = target_hours + 1;
      if (target_hours == 24){
      target_hours = 0;
    }
    delay(200);
  }
swmins = digitalRead(swminsip);
  if (swmins == HIGH){
    target_minutes = target_minutes + 1;
    if (target_minutes == 60){
      target_minutes = 0;
    }
    delay(200);
  }
swsecs = digitalRead(swsecsip);
  if (swsecs == HIGH){
    target_seconds = target_seconds + 1;
      if (target_seconds == 60){
      target_seconds = 0;
      }
    delay(200);
  }
  delay(100);
  }


swhrs = digitalRead(swhrsip);                                // Set pump duration loop
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
//----------------------------------- Activate Pump Sequence ---------------------------------------------------

if (current_seconds == target_seconds){                      // Pump running loop (target time = current time)
  if (current_minutes == target_minutes){
    if (current_hours == target_hours){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Dosing for ");
        lcd.print(duration, DEC);
        lcd.print(" Sec");
        duration = duration * 1000;
        digitalWrite(Rly_1, HIGH);
        delay(duration);
        digitalWrite(Rly_1, LOW);
        duration = duration / 1000;
      }
  }

}

}
