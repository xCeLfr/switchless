 /*  Megadrive
 *   
 *   ATtiny pins
                                         ----------
                        (REGION) JP2 --- 1 ATtiny 8 --- VCC+ 5V
                   Reset button pin3 --- 2 45, 85 7 --- pin2 JP1 (FREQ)
   Reset signal (after the cut) pin4 --- 3        6 --- pin1 LED 1 
                                 GND --- 4        5 --- pin0 LED 2 
                                         ----------

  /!\ READ THIS FIRST OR YOU WILL BRICK YOUR ATTINY /!\
   If you want to use PB5 as I/O port you have to disable reset fuse, load this sketch in the ATTiny fist !!!
   The following command will diable reset fuse ==> You will NOT be able to program the ATTiny anymore unless you use an AVR High Voltage Programmer.
   
   DISABLE reset fuse on ATTiny45 : 
      cmd.exe
      cd C:\Program Files (x86)\Arduino\hardware\tools\avr\bin
      avrdude.exe -c avrisp -C ..\etc\avrdude.conf -p attiny45 -b 19200 -P com4 -U lfuse:w:0x62:m -U hfuse:w:0x5f:m -U efuse:w:0xff:m
  
*/
#include <EEPROM.h>
//#define DEBUG
//#define DEBUG_DISPLAY_REGION

/* OBSOLETE the reset Active state is now checked at console boot
#define RESET_ACTIVE_LOW_OR_HIGH HIGH
 *  RESET_ACTIVE_LOW_OR_HIGH LOW|HIGH
 *  Let you choose if the MEGADRIVE is ACTIVE LOW or ACTIVE HIGH
 *                                        
 *  Seriel no |    Motherboard     | RESET
 *  1600-09   |  IC BD M5 PAL VA4  | HIGH
 *  1601-09   |  IC BD M5 PAL VA6  | LOW
 *  1601-18   |  IC BD M5 PAL      | LOW
 *  
 *  
 */

byte modState = 0;  /* 0 JAP - 1 EUR - 2 USA */

#ifdef DEBUG
int pinLedPAL  = 6; /* LED 1 (Debug) */
int pinLedNTSC = 7; /* LED 2 (Debug) */
#else
int pinLedPAL  = 0; /* LED 1 */
int pinLedNTSC = 1; /* LED 2 */
#endif

int pinFREQ    = 2; /* JP1 */
int pinButton  = 3; 
int pinReset   = 4;
int pinREGION  = 5; /* JP2 */

int buttonPressedLongDelay = 1200; // Press time 1200ms
unsigned long buttonFirstPressTime = 0;
bool buttonPressed = false;
bool buttonPressedLong = false;
bool resetActiveHighOrLow = LOW;

void setup() {
#ifdef DEBUG
    Serial.begin(9600);
#endif

  pinMode(pinReset  , OUTPUT);
  pinMode(pinButton , INPUT_PULLUP);
  pinMode(pinLedPAL , OUTPUT);
  pinMode(pinLedNTSC, OUTPUT);
  pinMode(pinFREQ   , OUTPUT);
  pinMode(pinREGION , OUTPUT);
  
  digitalWrite(pinReset, HIGH); /* reset always high */

  /* Check for saved values */
  modState = EEPROM.read(0); 
  if(modState < 0 || modState > 2) modState = 0; /* EEPROM dead or first EEPROM Write not done yet */
  DoSwitch(modState);

  /* Check if the MEGADRIVE is ACTIVE LOW or ACTIVE HIGH */
  resetActiveHighOrLow = digitalRead(pinButton);
  startBlink(resetActiveHighOrLow);  /* blink at console power on */
    
  /* Set the LED */
  modLedSet();
}

void loop() { /* Main Loop */
 
  if (digitalRead(pinButton) != resetActiveHighOrLow) {    
    if(!buttonPressed) {    /* Get first press */
      buttonPressed = true; /* the button is pressed */
      buttonFirstPressTime = millis();
      buttonPressedLong = false;
    }
    
    if(buttonPressed) {
      if((millis() - buttonFirstPressTime) > buttonPressedLongDelay && buttonPressedLong == false){
        buttonPressedLong = true;
        modSwitch(); /* SWITCH */
      }
    }  
  }
  else {
    if(buttonPressed && !buttonPressedLong) {
      buttonPressedLong = false; 
      buttonPressed = false;
      modReset(); /* RESET */
    }
    else
      buttonPressed = false;  
  } 

  /* prevent double input on button click */
  delay(10);
}

void modLedSet() {  /* Set the led 0:JAP - 1:EUR - 3:USA */

  switch(modState) {
    case 0:
      #ifdef DEBUG_DISPLAY_REGION
        Serial.println("JAP"); 
      #endif
      digitalWrite(pinLedNTSC, LOW);
      digitalWrite(pinLedPAL, HIGH);
      break; 
    case 1:
      #ifdef DEBUG_DISPLAY_REGION
        Serial.println("EUR"); 
      #endif
      digitalWrite(pinLedNTSC, HIGH);
      digitalWrite(pinLedPAL, LOW);
      break; 
    case 2:
      #ifdef DEBUG_DISPLAY_REGION
        Serial.println("USA"); 
      #endif
      digitalWrite(pinLedNTSC, HIGH);
      digitalWrite(pinLedPAL, HIGH);
      break; 
  }  
}

void modReset() {  
  #ifdef DEBUG
    Serial.println("RESET");
  #endif

  /* Quick 0v to Reset pin */
  digitalWrite(pinReset, LOW);
  delay(100);
  digitalWrite(pinReset, HIGH);  
} 

void modSwitch() {
  
  /* Toggle modState */
  modState = (modState<2?modState+1:0); // Switch mod state JAP --> EUR --> NTSC --> JAP ...

  /* Save value to eeprom */
  EEPROM.write(0, modState); /* set the pin to D1 */

  /* Set the mod pin */
  DoSwitch(modState);

  /* Change LED state */
  modLedSet();
}

void DoSwitch(byte mode) {
    
  /* Set the modPin and led accordingly */
  switch(mode) {
    case 0:
      digitalWrite(pinFREQ  ,HIGH); /* 60Hz */
      digitalWrite(pinREGION,LOW);  /* JAP  */
      break; 
    case 1:
      digitalWrite(pinFREQ  ,LOW);  /* 50Hz */
      digitalWrite(pinREGION,HIGH); /* ENG  */
      break; 
    case 2:
      digitalWrite(pinFREQ  ,HIGH); /* 60Hz */
      digitalWrite(pinREGION,HIGH); /* ENG  */
      break; 
  }
} 
/* Function: void startBlink(byte mode)
 * This function is will blink at console power on
 * Active LOW  = the LED will blink 1 time
 * Active HIGH = the LED will blink 2 times
 */
void startBlink(byte mode){

  delay(100);

  if(resetActiveHighOrLow) {    
      /* Active LOW */
      digitalWrite(pinLedNTSC, HIGH);
      digitalWrite(pinLedPAL, HIGH);
      delay(200);
      digitalWrite(pinLedNTSC, LOW);
      digitalWrite(pinLedPAL, LOW);
      delay(300);
  }
  else {
      /* Active HIGH */
      digitalWrite(pinLedNTSC, HIGH);
      digitalWrite(pinLedPAL, HIGH);
      delay(200);
      digitalWrite(pinLedNTSC, LOW);
      digitalWrite(pinLedPAL, LOW);
      delay(300);
      digitalWrite(pinLedNTSC, HIGH);
      digitalWrite(pinLedPAL, HIGH);
      delay(200);
      digitalWrite(pinLedNTSC, LOW);
      digitalWrite(pinLedPAL, LOW);
      delay(300);
   }


    delay(500);
}
