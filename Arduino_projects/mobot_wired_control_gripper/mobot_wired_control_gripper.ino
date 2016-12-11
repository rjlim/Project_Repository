
#define frwd_L 6
#define frwd_R 9
#define bkwd_L 7
#define bkwd_R 8

#define Lsense A3
#define Rsense A5
#define Csense A4

#include <String.h> 
#include <Servo.h>

Servo gripped;

int pos = 0;

int fsrPin = 0;     // the FSR and 10K pulldown are connected to a0
double fsrReading;     // the analog reading from the FSR resistor divider
double fsrVoltage;     // the analog reading converted to voltage
double fsrResistance;  // The voltage converted to resistance, can be very big so make "long"
double fsrConductance; 
double fsrForce;  

int IR_sensor_left = 0;     // read the value from left IR sensor
int IR_sensor_center = 0;   // read the value from center IR sensor
int IR_sensor_right = 0;    // read the value from right IR sensor
int total_adc_val = 0;

void setup()
{
  Serial.begin(9600);   

  gripped.attach(2);
  gripped.write(0);


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
  Serial.println(" input t to open gripper, g to close gripper");

  digitalWrite(frwd_L, LOW); 
  digitalWrite(frwd_R, LOW);
  digitalWrite(bkwd_L, LOW); 
  digitalWrite(bkwd_R, LOW);
  Serial.println("");
  Serial.println("");
}

void loop()
{
  if (Serial.available())
  {
    switch(Serial.read())
    {
    case 's':
      backward();       
      break;
    case 'w':
      foward();
      break;
    case 'a':
      left();     
      break;
    case 'd':
      right(); 
      break;
    case 't':
      gripped_up(); 
      break;
    case 'g':
      gripped_down(); 
      break;
    case 'q':
      gripforce(); 
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
  Serial.println("moving foward");

}

void backward()
{
  digitalWrite(frwd_L, LOW);  
  digitalWrite(frwd_R, LOW);
  digitalWrite(bkwd_L, HIGH);   
  digitalWrite(bkwd_R, HIGH);
  Serial.println("moving backward");

}

void right()
{
  digitalWrite(frwd_L, HIGH);  
  digitalWrite(frwd_R, LOW);
  digitalWrite(bkwd_L, LOW);  
  digitalWrite(bkwd_R, HIGH);
  Serial.println("turning right");
}

void left()
{
  digitalWrite(frwd_L, LOW);   
  digitalWrite(frwd_R, HIGH);
  digitalWrite(bkwd_L, HIGH); 
  digitalWrite(bkwd_R, LOW);
  Serial.println("turning left");
}

void stopped()
{
  digitalWrite(frwd_L, LOW); 
  digitalWrite(frwd_R, LOW);
  digitalWrite(bkwd_L, LOW); 
  digitalWrite(bkwd_R, LOW);
  Serial.println("stop");
}

void gripped_up()
{
  Serial.println("gripper open");
  pos = 55;
  gripped.write(pos);              // tell servo to go to position in variable 'pos' 
  delay(10);                       // waits 15ms for the servo to reach the position 
}

void gripped_down()
{
  Serial.println("gripper close");
  pos = 0;                              
  gripped.write(pos);              // tell servo to go to position in variable 'pos' 
  delay(10);                       // waits 15ms for the servo to reach the position 
}


void gripforce()
{
  fsrReading = analogRead(fsrPin);  

  // analog voltage reading ranges from about 0 to 1023 which maps to 0V to 5V (= 5000mV)
  fsrVoltage = map(fsrReading, 0, 1023, 0, 5000);

  if (fsrVoltage == 0) 
  {
    Serial.println("No pressure");  
  } 
  else 
  {
    // The voltage = Vcc * R / (R + FSR) where R = 10K and Vcc = 5V
    // so FSR = ((Vcc - V) * R) / V        yay math!
    fsrResistance = 5000 - fsrVoltage;     // fsrVoltage is in millivolts so 5V = 5000mV
    fsrResistance *= 100000;                // 10K resistor
    fsrResistance /= fsrVoltage;

    fsrConductance = 1000000;           // we measure in micromhos so 
    fsrConductance /= fsrResistance;

    // Use the two FSR guide graphs to approximate the force
    if (fsrConductance <= 1000) {
      fsrForce = fsrConductance / 80;
      Serial.print("grip Force in Newtons: ");
      Serial.println(fsrForce);      
    } 
    else {
      fsrForce = fsrConductance - 1000;
      fsrForce /= 30;
      Serial.print("grip Force in Newtons: ");
      Serial.println(fsrForce);            
    }
  }
}








