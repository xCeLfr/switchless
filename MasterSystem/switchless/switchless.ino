/* MASTER SYSTEM  
 *  
 *      
 *      ATtiny pins
                                         ----------
                               Reset --- 1 ATtiny 8 --- VCC+ 5V
                   Reset button pin3 --- 2 45, 85 7 --- pin2 JP1
Reset Switch out(after the cut) pin4 --- 3        6 --- pin1 LED 1 (NTSC)
                                 GND --- 4        5 --- pin0 LED 2 (PAL)
                                         ----------
*/
#include <EEPROM.h>
//#define DEBUG

byte modState = 0;  // 0 PAL - 1 NTSC

#ifdef DEBUG
int pinLedPAL  = 6; // LED 1 (Debug)
int pinLedNTSC = 7; // LED 2 (Debug)
#else
int pinLedPAL  = 0; // LED 1
int pinLedNTSC = 1; // LED 2
#endif

int pinMod     = 2; // JP1
int pinButton  = 3; 
int pinReset   = 4;

int buttonPressedLongDelay = 1200; // Press time 1200ms
long buttonFirstPressTime = 0;
boolean buttonPressed = false;
boolean buttonPressedLong = false;

void setup() {
  #ifdef DEBUG
    Serial.begin(9600);
  #endif

  pinMode(pinReset , OUTPUT);
  pinMode(pinButton, INPUT_PULLUP);
  pinMode(pinLedPAL, OUTPUT);
  pinMode(pinLedNTSC, OUTPUT);
  pinMode(pinMod, OUTPUT);
  
  digitalWrite(pinReset, HIGH); // reset always high

  // Check for saved values
  modState = EEPROM.read(0); 
  if(modState != 0 && modState != 1) modState = 0; // EEPROM dead or first EEPROM Write not done yet
  DoSwitch(modState);
    
  modLedSet(); // Set the LED   
}

void loop() { // Main Loop  
  if (digitalRead(pinButton) == LOW) {    
    if(!buttonPressed) { // Get first press
      buttonPressed = true; // the button is pressed
      buttonFirstPressTime = millis();
      buttonPressedLong = false;
    }
    
    if(buttonPressed) {
      if((millis() - buttonFirstPressTime) > buttonPressedLongDelay && buttonPressedLong == false){
        buttonPressedLong = true;
        modSwitch(); // SWITCH
      }
    }  
  }
  else {
    if(buttonPressed && !buttonPressedLong) {
      buttonPressedLong = false; 
      buttonPressed = false;
      modReset(); // RESET
    }
    else
      buttonPressed = false;  
  }  
}

void modLedSet() {  // Set the led 0 PAL - 1 NTSC
  
  if(!modState) {
    #ifdef DEBUG
      Serial.println("PAL"); 
    #endif
    digitalWrite(pinLedNTSC, LOW);
    digitalWrite(pinLedPAL, HIGH);
  }
  else {
    #ifdef DEBUG
      Serial.println("NTSC");
    #endif
    digitalWrite(pinLedNTSC, HIGH);
    digitalWrite(pinLedPAL, LOW);
  }
}

void modReset() {  
  #ifdef DEBUG
    Serial.println("RESET");
  #endif

  // Quick 0v to Reset pin
  digitalWrite(pinReset, LOW);
  delay(100);
  digitalWrite(pinReset, HIGH);  
} 

void modSwitch() {
  
  // Toggle modState 
  modState = !modState; // Switch mod state PAL <--> NTSC

  // Save value to eeprom
  EEPROM.write(0, modState); //set the pin to D1

  // Set the mod pin
  DoSwitch(modState);

  // Change LED state
  modLedSet();
}

void DoSwitch(byte mode) {
  
  // Set the modPin and led accordingly
  if(mode)
    digitalWrite(pinMod, LOW); // NTSC
  else
    digitalWrite(pinMod, HIGH); // PAL    
}
