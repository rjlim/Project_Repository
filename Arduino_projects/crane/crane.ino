
#define frwd_L 6

#define bkwd_L 7



#include <String.h> 
#include <Servo.h>

Servo cranehead;

int crane_pos;

int fsrPin = 0;     // the FSR and 10K pulldown are connected to a0
double fsrReading;     // the analog reading from the FSR resistor divider
double fsrVoltage;     // the analog reading converted to voltage
double fsrResistance;  // The voltage converted to resistance, can be very big so make "long"
double fsrConductance; 
double fsrForce;  

void setup()
{
  Serial.begin(9600);   

  cranehead.attach(2);

  pinMode(frwd_L, OUTPUT); 
  pinMode(bkwd_L, OUTPUT);     

  Serial.println("simple demonstration of a tower crane with force measurment with one of its structural member");
  Serial.println("to control the crane input the following character one at a time");
  Serial.println(" w = lift up ");
  Serial.println(" s = lift down");
  Serial.println(" a = turn left 5 degrees");
  Serial.println(" d = turn left 5 degrees ");
  Serial.println(" q= measure force ");
  digitalWrite(frwd_L, LOW); 
  digitalWrite(bkwd_L, LOW); 

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
    case 'q':
      cranenium(); 
      break;

    default:
      stopped(); 
    }   
  }
}


void foward()
{
  digitalWrite(frwd_L, HIGH);
  digitalWrite(bkwd_L, LOW);   
  Serial.println("up");
}

void backward()
{
  digitalWrite(frwd_L, LOW);  
  digitalWrite(bkwd_L, HIGH);   
  Serial.println("down");
}

void left()
{
  crane_pos += 5;
  cranehead.write(crane_pos);
  Serial.println("turn left 5 degrees");
}

void right()
{
  crane_pos -= 5;
  cranehead.write(crane_pos);
  Serial.println("turn right 5 degrees");

}

void stopped()
{
  digitalWrite(frwd_L, LOW); 
  digitalWrite(bkwd_L, LOW);
  Serial.println("stop");
}

void cranenium()
{
  fsrReading = analogRead(fsrPin);  

  // analog voltage reading ranges from about 0 to 1023 which maps to 0V to 5V (= 5000mV)
  fsrVoltage = map(fsrReading, 0, 1023, 0, 5000);

  if (fsrVoltage == 0) 
  {
    Serial.println("No Load");  
  } 
  else 
  {
    // The voltage = Vcc * R / (R + FSR) where R = 10K and Vcc = 5V
    // so FSR = ((Vcc - V) * R) / V        yay math!
    fsrResistance = 5000 - fsrVoltage;     // fsrVoltage is in millivolts so 5V = 5000mV
    fsrResistance *= 10000;                // 10K resistor
    fsrResistance /= fsrVoltage;

    fsrConductance = 1000000;           // we measure in micromhos so 
    fsrConductance /= fsrResistance;

    // Use the two FSR guide graphs to approximate the force
    if (fsrConductance <= 1000) {
      fsrForce = fsrConductance / 80;
      Serial.print(" Force in Newtons: ");
      Serial.println(fsrForce);      
    } 
    else {
      fsrForce = fsrConductance - 1000;
      fsrForce /= 30;
      Serial.print(" Force in Newtons: ");
      Serial.println(fsrForce);            
    }
  }
}


