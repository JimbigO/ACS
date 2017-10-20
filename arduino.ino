// -------------------------------------------------------------------------
// Aquarium Control System V1.03
// 30/01/17
// Created by Craig Sellwood & James Cooper
// Developed for use with Arduino Mega 2650
// -------------------------------------------------------------------------

#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal.h>
RTC_DS3231 rtc;

int current_hours;                // Current Hours
int current_minutes;              // Current Minutes
int current_seconds;              // Current Seconds
int target_hours;                 // Target Hours
int target_minutes;               // Target Minutes
int target_seconds;               // Target Seconds
int duration_sec;                 // Duration of Pump Cycle seconds
int duration_ms;                  // Duration of Pump Cycle in milliseconds
int Rly_1 = A12;                  // Relay 1 Output 
int Rly_2 = A13;                  // Relay 2 Output
int Rly_3 = A14;                  // Relay 3 Output
int Rly_4 = A15;                  // Relay 4 Output
int Button_Var;                   // LCD Button variable value 0 - 1024
int current_menu;                 // Current menu reference number
int last_menu;                    // Last menu reference number

boolean up;                       // LCD Up button
boolean down;                     // LCD Down Button
boolean left;                     // LCD Left Button
boolean right;                    // LCD Right Button
boolean select;                   // LCD Select Button
boolean standby;                  // LCD No Buttons Pressed

boolean switch_toggle;                // Toggle Function
boolean switch_toggle_ip = A1;        // Switch for Toggle Function
boolean switch_hours;                 // Hours Change
boolean switch_hours_ip = A2;         // Switch for Hours Change
boolean switch_minutes;               // Minutes Change
boolean switch_minutes_ip = A3;       // Switch for Minutes Change
boolean switch_seconds;               // Seconds Change
boolean switch_seconds_ip = A4;       // Switch for Seconds Change
boolean prime;                        // Prime Pump
boolean switch_prime_ip = A5;               // Switch for Prime Pump

// Initialize LCD library & configuration
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
	lcd.begin(16, 2);               // Set up the LCD's number of columns and rows:
	rtc.begin();                    // Begin RTC communications
	pinMode(Rly_1, OUTPUT);         // Set Rly_1 Relay pin as Output
	digitalWrite(Rly_1, LOW);       // Set Rly_1 Pump to off
	pinMode(switch_toggle_ip, INPUT);
	pinMode(switch_hours_ip, INPUT);
	pinMode(switch_minutes_ip, INPUT);
	pinMode(switch_seconds_ip, INPUT);
	switch_toggle = LOW;            // Ensure all outputs are 'Low' on initial run
	switch_hours = LOW;
	switch_minutes = LOW;
	switch_seconds = LOW;
	up = false;
	down = false;
	left = false;
	right = false;
	select = false;
	standby = false;

	current_hours = 00;              // Erase all memory settings
	current_minutes = 00;
	current_seconds = 00;
	duration_sec = 1;               // Set default duration
	target_hours = 14;              // Set default trigger time
	target_minutes = 0;
	target_seconds = 0;
	current_menu = 0;
	last_menu = 0;
}

//------------------------------- Main Program Loop -----------------------------------------------------------------

void loop() {
 
	DateTime now = rtc.now();     // Align Arduino clock to RTC clock
	lcd.setCursor(0,0);           // Set to starting write location Top Line
	lcd.print("Date: ");          // OUTPUT DATE & TIME to LCD
	lcd.print(now.day(), DEC);    // Print current day DD
	lcd.print("/");               // Add / Separator
	lcd.print(now.month(), DEC);  // Print current month MM
	lcd.print("/");               // Add / Separator
	lcd.print(now.year(), DEC);   // Print current year YY
	lcd.print("   ");
	lcd.setCursor(0,1);           // Set to starting write location Bottom Line
	lcd.print("Time: ");          // Print Time: header to LCD
	lcd.print(now.hour(), DEC);   // Print current Hour HH
	lcd.print(":");               // Print : separator
	lcd.print(now.minute(), DEC); // Print current Minute MM
	lcd.print(":");               // Print : separator
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
		current_menu = current_menu + 1;
		if(current_menu >= 5){
			current_menu = 0;
		}                              
	}
	else if (Button_Var > 100 && Button_Var <275){              // Down LCD Button Pressed
		down = true;
		current_menu = current_menu - 1
		if(current_menu <= 0){
			current_menu = 0
		}
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

	if (current_menu != last_menu){
	last_menu = current_menu;
	menu()
	}
		//--------------------------Update Memory Locations from RTC-----------------------------------------------------------

	current_seconds = (now.second());                        // Update current seconds to memory location current_seconds
	current_minutes = (now.minute());                        // Update current minutes to memory location current_minutes
	current_hours = (now.hour());                            // Update current hours to memory location current_hours

		// ------------------------------------ Test Relays --------------------------------------------------------------------

		while (left == true){
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Testing Relay 1");
		digitalWrite(Rly_1, LOW);
		delay(1000);
		digitalWrite(Rly_1, HIGH);
		delay(1000);
		}

		while (up == true){
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Testing Relay 2");
		digitalWrite(Rly_2, LOW);
		delay(1000);
		digitalWrite(Rly_2, HIGH);
		}

		while (right == true){
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Testing Relay 3");
		digitalWrite(Rly_3, LOW);
		delay(1000);
		digitalWrite(Rly_3, HIGH);
		delay(1000);
		}

		while (down == true){
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Testing Relay 4");
		digitalWrite(Rly_4, LOW);
		delay(1000);
		digitalWrite(Rly_4, HIGH);
		delay(1000);
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

void.Menu() {
// -------------------------------- Menu Setup -----------------------------------------------------------------------
		
			if (current_menu == 1){
				lcd.clear();
				lcd.setCursor(0,0);
				lcd.print("Setup Time");
				delay(500);
				return;
			}
			else if(current_menu == 2){
				lcd.clear();
				lcd.setCursor(0,0);
				lcd.print("CO2 Dose");
				lcd.setCursor(0,1);
				lcd.print("Schedule");
				delay(5000);
				lcd.clear();
				lcd.setCursor(0,0);
				lcd.print(target_hours);
				lcd.print(":");
				lcd.print(target_minutes)
				return
			}
			else if(current_menu == 3){
				lcd.clear();
				lcd.setCursor(0,0);
				lcd.print("Setup Alarm ");
				delay(500);
				return
			}
			else if(current_menu == 4){
				lcd.clear();
				lcd.setCursor(0,0);
				lcd.print("Setup Dose Time");
				delay(500);
				return
			}
			else if(current_menu == 5){
				lcd.clear();
				lcd.setCursor(0,0);
				lcd.print("Setup Dose Time");
				delay(500);
				return
			}
return;
}
	
	// ------------------------------- Pump Prime Sequence ----------------------------------------------------------------

		prime = digitalRead(switch_prime_ip);
		while (prime == HIGH){
		prime = digitalRead(switch_prime_ip);
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
		switch_hours = digitalRead(switch_hours_ip);
		
		if (switch_hours == HIGH){
			target_hours = target_hours + 1;
			if (target_hours == 24){
			target_hours = 0;
		}
		delay(200);
		}
	switch_minutes = digitalRead(switch_minutes_ip);
		if (switch_minutes == HIGH){
		target_minutes = target_minutes + 1;
		if (target_minutes == 60){
			target_minutes = 0;
		}
		delay(200);
		}
	switch_seconds = digitalRead(switch_seconds_ip);
	if (switch_seconds == HIGH){
		target_seconds = target_seconds + 1;
			if (target_seconds == 60){
			target_seconds = 0;
			}
		delay(200);
	}
	delay(100);
}
	//---------------------------------------- Setting Pump Run Duration ---------------------------------------------------------
switch_hours = digitalRead(switch_hours_ip);         // Set pump duration loop
while (switch_hours == HIGH){
	switch_hours = digitalRead(switch_hours_ip);
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Run Duration (s):");
	lcd.setCursor(0,1);
	lcd.print(duration_sec, DEC);
	switch_seconds = digitalRead(switch_seconds_ip);
	if (switch_seconds == HIGH){
		duration_sec = duration_sec + 1;
		if (duration_sec >= 91){                         // to prevent excessive duration times being set limit to 91 seconds
		duration_sec = 1;                                // reset the duration_sec value back to 1 (rollover)                               
		}
		delay(200);
	}
	delay(100);
}
//----------------------------------- Activate Pump Sequence ----------------------------------------------------------------
duration_ms = duration_sec * 1000;                  // multiply the displayed value for duration by 1000 to convert into ms

if (current_seconds == target_seconds){             //Check current time against the alarm target time
	if (current_minutes == target_minutes){           // if hours, minutes and seconds match the target then continue
		if (current_hours == target_hours){
				lcd.clear();                                // clear the LCD
				lcd.setCursor(0,0);                         // set cursor to the top line first character
				lcd.print("Dosing for ");                   // print text to screen
				lcd.print(duration_sec, DEC);               // print the value of variable duration as a decimal
				lcd.print(" Sec");                          // print the suffix Sec
				digitalWrite(Rly_1, HIGH);                  // set the output relay pin High to activate Rly_1
				delay(duration_ms);                         // delay by the number of ms calculated previously
				digitalWrite(Rly_1, LOW);                   // turn off the pump by setting relay 1 LOW
			}
	}

}

}
