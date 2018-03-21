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
int potVal;  // variable to read the value from the analog pin
int angle;   // variable to hold the angle for the servo motor

 void setup() 
{
    Serial.begin(9600); // open a serial connection to your computer
    lcd.begin(16, 2); //begin reading/writing to LCD monitor
}

void loop() 
{
  potVal = analogRead(potPin); // read the value of the potentiometer
  // print out the value to the Serial Monitor
  Serial.print("potVal: ");
  Serial.println(potVal);
  delay(100);
  
}




