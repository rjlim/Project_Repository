#include <Servo.h> 

Servo myservo;

int pos = 180; 

int startPin = 8, finishPin = 6, done =0;
float distance = 0.4, interval, velocity, work; 
unsigned long strt = 0, finsh = 0;
int testing = 0;

float mass = 100;    // change this value for the specific mass of the object to be dropped

void setup() 
{
  myservo.attach(4); 
  myservo.write(pos);  
  Serial.begin(9600);
  pinMode(startPin, INPUT);
  pinMode(finishPin, INPUT);
  Serial.println("nitialize ports...");
  Serial.println("");
  Serial.println("");

  Serial.println("ok");

  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("this setup compute the final velocity of a free fall object using the formula vf = vi + a*t ");
  Serial.println("where vf = final velocity(m/s, vi = initial velocity((m/s) a = acceleration (9.8 m/s for free fall) and t = time(s)");
  Serial.println("before running the setup be sure the it is in a flat surface to get an accurate reading");
  Serial.println("");
  Serial.println("");
  delay(500);
  Serial.println("intructions");
  Serial.println("");
  Serial.println("1. place the object on the top of the device");
  delay(500);
  Serial.println("2. type any key and then enter to release the object");
  delay(1000);
  Serial.println("3. wait for the object to be dropped");
  delay(500);
  Serial.println("");
  delay(500);
  Serial.println("the device will automatically restart once the object is dropped and the value of final velocity and amount of work is given");
  Serial.println("");
  delay(1000);
  Serial.println("please place the object");
  Serial.println("");
  Serial.println("");
}

void loop() 
{ 

  if (Serial.available() > 0)
  {
    int inByte = Serial.read() - '0';
    int testing = inByte; 
    Serial.flush();

    velocity = 0;
    while(testing)
    { 
      Serial.println("dropping in");
      delay(1000);
      Serial.println("3...");
      delay(1000);
      Serial.println("2...");
      delay(1000);
      Serial.println("1...");
      delay(10);
      Serial.println("release");
      pos = 80;
      myservo.write(pos);

      while(!strt) 
      {
        if(digitalRead(startPin) == LOW)
          strt = micros();
      }
      while(!finsh) 
      {
        if(digitalRead(finishPin) == LOW)
          finsh = micros();
      }
      while(!done) 
      {
        interval = float(finsh-strt);
        velocity = 9.8*(interval/1000000);
        work = 0.5*mass*(velocity*velocity);
        Serial.println("");
        Serial.print("final velocity is ");
        Serial.print(velocity);   
        Serial.print(" m/s "); 
        Serial.print("and amount of work is "); 
        Serial.print(work); 
        Serial.print(" joules "); 
        Serial.println(""); 
        Serial.flush();
        done = 1;

      }
      if(testing != 0)
      { 
        strt = 0;
        finsh = 0;
        done = 0;
        delay(500);
        pos = 180;
        delay(1000);
        myservo.write(pos);  
        Serial.println(""); 
        Serial.println("");     
        Serial.println("to try again just place the object, press any key then hit enter");
        Serial.println("be sure to remove the object before trying again");
        Serial.println(""); 
        Serial.println(""); 
        testing = 0;
        delay(100);
      }


    }
  }
}

















