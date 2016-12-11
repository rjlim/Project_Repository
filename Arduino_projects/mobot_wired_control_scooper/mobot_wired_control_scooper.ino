
#define frwd_L 6
#define frwd_R 9
#define bkwd_L 7
#define bkwd_R 8

#define trigPin 4
#define echoPin 5

#include <String.h> 
#include <Servo.h>

Servo scooped;

int pos = 0;

void setup()
{
  Serial.begin(9600);   

  scooped.attach(2);
  scooped.write(0);

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
  Serial.println("");
  Serial.println("");
  Serial.println("  distance log starts in...");
  delay(1000);
  Serial.println("5...");
  delay(1000);
  Serial.println("4...");
  delay(1000);
  Serial.println("3...");
  delay(1000);
  Serial.println("2...");
  delay(1000);
  Serial.println("1...");
  delay(1000);
  Serial.println(" now logging distance");
}

void loop()
{
  if (Serial.available())
  {
    switch(Serial.read())
    {
    case 's':
      foward();
      break;
    case 'w':
      backward();
      break;
    case 'a':
      left();     
      break;
    case 'd':
      right(); 
      break;
    case 't':
      scooped_up(); 
      break;
    case 'g':
      scooped_down(); 
      break;
      
      case 'q':
      distancia(); 
      break;
    default:
      stopped(); 


    }   
  }

  delay(500);
}


void foward()
{
  digitalWrite(frwd_L, HIGH);
  digitalWrite(frwd_R, HIGH);
  digitalWrite(bkwd_L, LOW);   
  digitalWrite(bkwd_R, LOW);
  Serial.println("moving forward");
}

void backward()
{
  digitalWrite(frwd_L, LOW);  
  digitalWrite(frwd_R, LOW);
  digitalWrite(bkwd_L, HIGH);   
  digitalWrite(bkwd_R, HIGH);
  Serial.println("moving backward");
}

void left()
{
  digitalWrite(frwd_L, LOW);   
  digitalWrite(frwd_R, HIGH);
  digitalWrite(bkwd_L, HIGH); 
  digitalWrite(bkwd_R, LOW);
  Serial.println("turning left");
}

void right()
{
  digitalWrite(frwd_L, HIGH);  
  digitalWrite(frwd_R, LOW);
  digitalWrite(bkwd_L, LOW);  
  digitalWrite(bkwd_R, HIGH);
  Serial.println("turning right");
}

void stopped()
{
  digitalWrite(frwd_L, LOW); 
  digitalWrite(frwd_R, LOW);
  digitalWrite(bkwd_L, LOW); 
  digitalWrite(bkwd_R, LOW);
  Serial.println("stop");
}

void scooped_up()
{
  Serial.println("moving up");
  for(pos = 0; pos < 45; pos += 1)  // goes from 0 degrees to 45 degrees 
  {                                  // in steps of 1 degree 
    scooped.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(10);                       // waits 15ms for the servo to reach the position 
  } 
}

void scooped_down()
{
  Serial.println("moving down");
  for(pos = 45; pos>=1; pos-=1)     // goes from 45 degrees to 0 degrees 
  {                                
    scooped.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(10);                       // waits 15ms for the servo to reach the position 
  } 
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





