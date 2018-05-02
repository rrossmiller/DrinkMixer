/*
 * Arduino code for lab project for BSE 365. 
 * This code is for automatically dispensing a drink with two liquid ingredients.
 * 
 * Authors: Rob Rossmiller, Matthew Caulfield
 */


#include <LiquidCrystal.h>

// Init LiquidCrystal object with pins corresponding to Crystal Ball project
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Valves
int alcValvePin = 6; // attach the alcohol valve to a digital pin
int mixerValvePin = 7; // attach the mixer valveto a digital pin

// Potentiometer
int const potPin = A0; // analog pin used to connect the potentiometer

// Flow meters
int alcoholMeterPin = 13;
int mixerMeterPin = 8;
int flowMeterCalibration = 20; // mL per pulse 

// Other pins
int confirmButtonState = 0;
int confirmButtonPin = 9;

// Booleans
bool correctVolume = false;

// Cup features
int cupVol = 200; // 295 mL = 10 fl oz
int alcoholVol;
int mixerVol;
int drinkStrength;
int drinkStrengthAnalog;

 void setup() 
{
    Serial.begin(9600); // open a serial connection to your computer
    lcd.begin(16, 2); //begin reading/writing to LCD monitor, 16 col x 2 row
    pinMode(confirmButtonPin, INPUT);
    pinMode(alcoholMeterPin, INPUT);
    pinMode(mixerMeterPin, INPUT);
    pinMode(alcValvePin, OUTPUT);
    pinMode(mixerValvePin, OUTPUT);
}

void loop() 
{
Serial.println("Loop Start");

    // Welcome user
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Insert cup");
    lcd.setCursor(0,1);
    lcd.print("Push confirm");
    
    while(confirmButtonState == 0) // wait for the confirm botton to be pushed
    {
      confirmButtonState = digitalRead(confirmButtonPin); // pushing the confirm button signals that a cup is under the spout
    }
    
    delay(1000);
    confirmButtonState = 0;                 // reset the confirmButton state
    lcd.clear();                            // clear the lcd display
    lcd.setCursor(0,0);                     // set the cursor to the first column of the first row
    lcd.print("Strength: ");                // display the strength of the drink to be made
    lcd.setCursor(0,1);
    lcd.print("Push Confirm");
    while(confirmButtonState == 0)
    {
      lcd.setCursor(10,0);
      lcd.print(getDrinkStrength());
      confirmButtonState = digitalRead(confirmButtonPin);    // check if the button has been pushed
    }
    confirmButtonState = 0;        // reset the confirmButton state
    
    // Start mixing drink
    // dispense drink
    delay(1000);  
    dispense();
    delay(1000);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Drink is ready");
    lcd.setCursor(0,1);
    lcd.print("Press Confirm");

     while(confirmButtonState == 0)
    {
      confirmButtonState = digitalRead(confirmButtonPin);
    }
    confirmButtonState = 0;
    delay(500);

  
}

int getDrinkStrength()
{
  drinkStrengthAnalog = analogRead(potPin);              // store the selected strength
  drinkStrength = map(drinkStrengthAnalog, 0, 1023, 0, 10);  // read the current potentiometer value and map it from 0 to 10
  return drinkStrength; // return the drink strength so it can be displayed
}

void dispense()
{
Serial.println("Dispensing");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Mixing...");
    
    int measured = 0; // current volume dispensed
    int alcoholToDispense = map(drinkStrengthAnalog, 0, 1023, 0, cupVol/3); // alcohol to be dispensed based on selected drink strenght. Max is 1/4 cup volume
    int mixerToDispense = cupVol-alcoholToDispense;                        // fill the rest of the cup with mixer
    
int displayPulseAlc = alcoholToDispense/flowMeterCalibration;
int displayPulseMix = mixerToDispense/flowMeterCalibration;
//Serial.print("mL Alc: ");
//Serial.println(alcoholToDispense);
Serial.print("Pulses: ");
Serial.println(displayPulseAlc);
//Serial.print("mL Mix: ");
//Serial.println(mixerToDispense);
Serial.print("Pulses: ");
Serial.println(displayPulseMix);
//Serial.println();
delay(3000);
    // each pulse is ~5.14 mL
    int pulses = 0;

    if(measured < alcoholToDispense) // open the alcohol valve if more is needed
      digitalWrite(alcValvePin,HIGH);     
           
    while(!correctVolume)
    {
      // measure flow
      if(digitalRead(alcoholMeterPin) == 1)
        pulses++;

      measured = pulses * flowMeterCalibration; 
Serial.println("Alcohol");
Serial.print("Pulses: ");
Serial.println(pulses);
//Serial.print("Measured: ");
//Serial.println(measured);
//Serial.println();
        
      // close valve when correct volume measured
      if(measured >= alcoholToDispense)
      {
        digitalWrite(alcValvePin,LOW);
        correctVolume = true;
      }
      delay(100);
    }

    // reset variables
    delay(1000);
    correctVolume = false;
    measured = 0;
    pulses = 0;
    
// Dispense Mixer
    if(measured < mixerToDispense) // open the alcohol valve if more is needed
      digitalWrite(mixerValvePin,HIGH); 

    while(!correctVolume)
    {
      // measure flow
      if(digitalRead(mixerMeterPin) == 1)
        pulses++;

      measured = pulses * flowMeterCalibration;
Serial.println("MIXER");
Serial.print("Pulses: ");
Serial.println(pulses);
//Serial.print("Measured: ");
//Serial.println(measured);
//Serial.println();
      // close valve when correct volume measured
      if(measured >= mixerToDispense)
      {
        digitalWrite(mixerValvePin,LOW);
        correctVolume = true;
      }
      delay(100);
    }
     // reset variables
    delay(1000);
    correctVolume = false;
    measured = 0;
    pulses = 0;
}



