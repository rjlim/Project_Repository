
#define frwd_L 6
#define frwd_R 9
#define bkwd_L 7
#define bkwd_R 8

#define trigPin 4
#define echoPin 5

#include <String.h>   


int inputString = 0;         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete


void setup()
{
  Serial.begin(9600);   

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(frwd_L, OUTPUT); 
  pinMode(frwd_R, OUTPUT);     
  pinMode(bkwd_L, OUTPUT);     
  pinMode(bkwd_R, OUTPUT);  

  Serial.println("usb controlled 3- wheeled robot with ultrasonic sensor");
  Serial.println("utrasonic sensor is used to measure approximate distance from an object ");
  Serial.println("controlled by inputing w,a,s,d, characters ");
  Serial.println(" instruction:");
  Serial.println("        input each character one at a time the press enter to send the command");

  Serial.println(" w = forward");
  Serial.println(" s = backward");
  Serial.println(" a = left");
  Serial.println(" d = right");
  Serial.println(" any key input = stop / brake");

  digitalWrite(frwd_L, LOW); 
  digitalWrite(frwd_R, LOW);
  digitalWrite(bkwd_L, LOW); 
  digitalWrite(bkwd_R, LOW);
}

void loop()
{
  if (Serial.available())
  {
    switch(Serial.read())
    {
    case 'w':    
      foward();
      break;
    case 's':
      backward();
      break;
    case 'a':
      left();     
      break;
    case 'd':
      right(); 
      break;
    case 'm':
      distancia(); 
      break;
    default:
      stopped(); 
    }   
  }
}


void foward()
{
  digitalWrite(frwd_L, HIGH);
  digitalWrite(frwd_R, HIGH);
  digitalWrite(bkwd_L, LOW);   
  digitalWrite(bkwd_R, LOW);
  Serial.println("forward");
}

void backward()
{
  digitalWrite(frwd_L, LOW);  
  digitalWrite(frwd_R, LOW);
  digitalWrite(bkwd_L, HIGH);   
  digitalWrite(bkwd_R, HIGH);
  Serial.println("backward");
}

void left()
{
  digitalWrite(frwd_L, LOW);   
  digitalWrite(frwd_R, HIGH);
  digitalWrite(bkwd_L, HIGH); 
  digitalWrite(bkwd_R, LOW);
  Serial.println("left");
}

void right()
{
  digitalWrite(frwd_L, HIGH);  
  digitalWrite(frwd_R, LOW);
  digitalWrite(bkwd_L, LOW);  
  digitalWrite(bkwd_R, HIGH);
  Serial.println("right");
}

void stopped()
{
  digitalWrite(frwd_L, LOW); 
  digitalWrite(frwd_R, LOW);
  digitalWrite(bkwd_L, LOW); 
  digitalWrite(bkwd_R, LOW);
  Serial.println("stop");
}

void distancia()
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
  Serial.println("measuring distance....");
  Serial.println(" ");
  delay(500);
  Serial.println("distance from object is:");
  Serial.print(distance);
  Serial.println(" cm");
}

