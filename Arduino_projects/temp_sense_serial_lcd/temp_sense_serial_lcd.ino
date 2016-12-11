/*
temperature controller using arduino program
*/


#include <LiquidCrystal.h>     // LCD library 

#define but3    4              // enable or disable button

LiquidCrystal lcd(13, 12, 11, 10, 9, 8); 

float temp_sense_pin =   A4;     // temperature sense pin (sensor is connected in A0 analog input 0 )

float temp_celsius = 0;
float temp_max = 30.00;          //  max temperature
float temp_mid = 27.50;          // mid temperature
float temp_min = 25.00;          // minimum temperature

const int numReadings = 10;

int relay = 6;
int buzzer = 7;

int readings[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
float total = 0;                  // the running total
float average = 0;  
int a = 0;
int inputPin = A0;

int state = HIGH;                               // the current state of the output pin
int reading;                                    // the current reading from the input pin
int previous = LOW;                             // the previous reading from the input pin
long time = 0;                                  // the last time the output pin was toggled
long debounce = 200;                            // the debounce time, increase if the output flickers




void setup()
{
  analogReference(INTERNAL);
  pinMode(relay, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(but3, INPUT);
  digitalWrite(relay, LOW);
  digitalWrite(buzzer, LOW);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Temp Controller");
  lcd.setCursor(0, 1);
  lcd.print("       by      ");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  delay(2000);
  
  lcd.clear();

  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;   
}



void loop()
{


  total= total - readings[index];         
  // read from the sensor:  
  readings[index] = analogRead(temp_sense_pin); 
  // add the reading to the total:
  total= total + readings[index];       
  // advance to the next position in the array:  
  index = index + 1;                    

  // if we're at the end of the array...
  if (index >= numReadings)              
    // ...wrap around to the beginning: 
    index = 0;                           

  // calculate the average:
  average = total / numReadings;         
  // send it to the computer as ASCII digits
  temp_celsius = average / 9.31;


  
  reading = digitalRead(but3);


  if(reading == LOW)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" System Standby ");
    digitalWrite(relay, LOW);
    digitalWrite(buzzer, LOW); 
  }
  else
  {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tc:");
    lcd.print(temp_celsius, 1);
    lcd.setCursor(7, 0);
    lcd.print(" Tm:");
    lcd.print(temp_max,1);
    lcd.setCursor(0, 1);
    lcd.print("check temp");

    if((temp_celsius >= temp_max) && (temp_celsius >= temp_mid))
    {
      digitalWrite(relay, HIGH);
      digitalWrite(buzzer, LOW); 
      lcd.setCursor(0, 1);
      lcd.print("HOT - A.C. On");
    }
    if((temp_celsius <= temp_min) && (temp_celsius <= temp_mid))
    {
      digitalWrite(relay, LOW);
      digitalWrite(buzzer, HIGH); 
      lcd.setCursor(0, 1);
      lcd.print("COLD - A.C. Off");
      a = 1;
    }
  }
  delay(150);
}

