/*  Work Flow
 *  
 *  "Place cup under spout. Press 'Confirm' button to continue"
 *  
 *  Once the button is pushed
 *  "Adjust Strength"
 *  "Current Strength: "
 *  -- User uses potentiometer to adjust strength
 *  
 *  User pushes 'Confirm'
 *    Open alcohol valve
 *    Measure alcohol output
 *    Close alcohol valve when correct amount has been released
 *    
 *    Open mixer valve
 *    Measure mixer output
 *    Close mixer valve when correct amount has been released
 *    
 *  User pushes 'Cancel' AT ANY TIME
 *    Machine stops -- important that this comes first
 *    "Canceling"
 *    Arduino/variables reset
 *      Process returns to the beginning
 *  "Your Drink is done. Enjoy!"
 */


#include <LiquidCrystal.h>

// Init LiquidCrystal object with pins corresponding to Crystal Ball project
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Valves
int alcValvePin; // attach the alcohol valve to a digital pin
int mixValvePin; // attach the mixer valveto a digital pin

// Potentiometer
int const potPin = A0; // analog pin used to connect the potentiometer

// Flow meters
int alcoholMeterPin;
int mixerMeterPin;
int flowMeterCalibration = 5.14 // mL/pulse

// Other pins
int confirmButtonState = 0;
int confirmButtonPin;

int drinkStrength;
int drinkStrengthAnalog;

// Booleans
bool correctVolume = false;

// Cup features
int cupVol = 295 // 295 mL = 10 fl oz
int mixerVol;

 void setup() 
{
    Serial.begin(9600); // open a serial connection to your computer
    lcd.begin(16, 2); //begin reading/writing to LCD monitor
    pinMode(confirmButtonPin, INPUT);
}

void loop() 
{
  // Welcome user
    lcd.clear();
    lcd.println("Place cup under spout");
    lcd.print("Press Confirm to continue");
    
    while(confirmButtonState == 0)
    {
      confirmButtonState = digitalRead(confirmButtonPin);
    }

    confirmButtonState = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Strength: ");
    lcd.setCursor(0,1);
    lcd.print("Push Confirm to continue");
    while(confirmButtonState == 0)
    {
      lcd.setCursor(10,0);
      lcd.println(drinkStrength());
      drinkStrengthAnalog = analogRead(potPin);
      confirmButtonState = digitalRead(confirmButtonPin);
    }
    confirmButtonState = 0;
    
    // Start mixing drink
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Mixing...");
    // dispense drink
    dispense();
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.println("Drink is ready.");
    lcd.print("Press Confirm to reset");

     while(confirmButtonState == 0)
    {
      confirmButtonState = digitalRead(confirmButtonPin);
    }
    
  
}

int drinkStrength()
{
  drinkStrength = map(analogRead(potPin), 0, 1023, 0, 10);
  return drinkStrength;
}

void dispense()
{
    int measured = 0;
    int alcoholToDispense = map(drinkStrengthAnalog, 0, 1023, 0, cupVol/4)
    mixerToDispense = cupVol-alcoholToDispense; // fill the rest of the cup with mixer

    // each pulse is ~5.14 mL
    int pulses = 0;

    if(!correctVolume)
      // open the alcohol valve
      digitalWrite(alcoholValvePin,HIGH);
      
    while(!correctVolume)
    {
      // measure flow
      if(digitalRead(alcoholMeterPin) == 1)
        pulses++;

      measured = pulses * flowMeterCalibration; 
      // close valve when correct volume measured
      if(measure >= alcoholToDispense)
      {
        digitalWrite(alcoholValvePin,LOW)
        correctVolume = true;
      }
    }

    // reset variables
    correctVolume = false;
    measured = 0;
    pulses = 0;
    
    // Dispense Mixer
    if(!correctVolume)
      // open the alcohol valve
      digitalWrite(mixerValvePin,HIGH);
      
    while(!correctVolume)
    {
      // measure flow
      if(digitalRead(mixerMeterPin) == 1)
        pulses++;

      measured = pulses * flowMeterCalibration; 
      // close valve when correct volume measured
      if(measured >= mixerToDispense)
      {
        digitalWrite(mixerValvePin,LOW)
        correctVolume = true;
      }
    }
}



