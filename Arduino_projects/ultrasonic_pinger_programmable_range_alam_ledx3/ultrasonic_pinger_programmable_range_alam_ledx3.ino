/*
HC-SR04 Ping distance sensor]
VCC to arduino 5v GND to arduino GND
Echo to Arduino pin 13 Trig to Arduino pin 12
Red POS to Arduino pin 9
Green POS to Arduino pin 11
560 ohm resistor to both LED NEG and GRD power rail
More info at: http://goo.gl/kJ8Gl
Original code improvements to the Ping sketch sourced from Trollmaker.com
Some code and wiring inspired by http://en.wikiversity.org/wiki/User:Dstaub/robotcar
*/

#define trigPin 13
#define echoPin 12
#define ledA 2
#define ledB 6
#define ledC 8
#define buzz A0

int inputString = 0;         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledA, OUTPUT);
  pinMode(ledB, OUTPUT);
  pinMode(ledC, OUTPUT);
  pinMode(buzz, OUTPUT);
  
   delay(100);
   Serial.println("");
   Serial.println("Initialize port.....");
   delay(1000);
   Serial.println("OK");
   Serial.println("");
   Serial.println("");
   Serial.println("");
   Serial.println("This program allow you to set a limit of a distance");
   Serial.println("it measure the approximate distance of the object alert it when it reach the limit");
   Serial.println("the measurement stop when it reached the limit"); 
   Serial.println("distance setting  1 = 10 cm, 2 = 20 cm and so on..... max of 90 cm");
   Serial.println("");
   Serial.println("");
   Serial.println("");
   delay(100);
   Serial.println("enter distance limit"); 
   Serial.println("");
   Serial.println(""); 
}

void loop() 
{

    
if (Serial.available() > 0)
 { 
    int inByte = Serial.read() - '0';
    int limit = inByte;
    
    while(limit)
  {
    
  float duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
//  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  
  int w = (limit*10);
  int x = (limit+limit)*M10; 
  int y = (limit+limit+limit)*10;
  int z = (limit+limit+limit+limit)*10;
  
    if (distance > z) 
    {                          // This is where the LED On/Off happens
    digitalWrite(ledA,HIGH); // When the Red condition is met, the Green LED should turn off
    digitalWrite(ledB,LOW);
    digitalWrite(ledC,LOW);
    
    }
   else if (distance >= y)
    {
    digitalWrite(ledA,LOW); // When the Red condition is met, the Green LED should turn off
    digitalWrite(ledB,HIGH);
    digitalWrite(ledC,LOW);
  
    }
   else if (distance >= x)
    {
    digitalWrite(ledA,LOW); // When the Red condition is met, the Green LED should turn off
    digitalWrite(ledB,LOW);
    digitalWrite(ledC,HIGH);
    }
   else if (distance >= w)
    {
    digitalWrite(ledA,LOW); // When the Red condition is met, the Green LED should turn off
    digitalWrite(ledB,LOW);
    digitalWrite(ledC,HIGH);
    limit = 0;
    }
    else 
    {
    digitalWrite(ledA,LOW); 
    digitalWrite(ledB,LOW);
    digitalWrite(ledC,LOW);

    }
      if (distance >= 250 || distance <= 0){
        Serial.println("Out of range");
        }
        else 
        {
          
          Serial.print("distence x =");
          Serial.print(x);
          Serial.print("  y =");
          Serial.print(y);
          Serial.print("  z =");
          Serial.print(z);
          Serial.print("  w =");
          Serial.print(w);
          Serial.print("     ");
          Serial.print(distance);
          Serial.println(" cm");
          if(limit <= 0)
                {
                  limit = 0;
                  Serial.println("distance Reached!!!");
                  Serial.println("stopped");
                  delay(1000);
                  digitalWrite(buzz,HIGH);
                  delay(1000);
                  digitalWrite(buzz,LOW);
                     digitalWrite(ledA,LOW); 
                     digitalWrite(ledB,LOW);
                     digitalWrite(ledC,LOW);
                  Serial.flush(); 
                }
             
            
           
        } 
    delay(50);  
  }
 }
}


