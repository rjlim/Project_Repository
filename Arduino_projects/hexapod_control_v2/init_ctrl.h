/*


  initialize all thing before doing anything


*/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define Servo_Addr_A 0x40
#define Servo_Addr_B 0x41

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

#define BvatSensePin_A 3  // maple mini analog pin input
#define BvatSensePin_B 4 // maple mini analog pin input


// adafruit servo controller
Adafruit_PWMServoDriver servoControl_A = Adafruit_PWMServoDriver(Servo_Addr_A);
Adafruit_PWMServoDriver servoControl_B = Adafruit_PWMServoDriver(Servo_Addr_B);



// initialize pins in maple mini
void initlializeStuff()
{
  pinMode(BvatSensePin_A, INPUT_ANALOG); // set analog input to maple mini pin
  pinMode(BvatSensePin_B, INPUT_ANALOG);  // set analog input to maple mini pin

  servoControl_A.begin();
  servoControl_B.begin();

  servoControl_A.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  servoControl_B.setPWMFreq(60);
  yield();
}


// initiallize serial communication
void initSerialComms(int baud_A, int baud_B)
{
  Serial.begin(baud_A);    // for debugging thru usb to serial
  Serial2.begin(baud_B);   // serial comm for bluetooth communication
}


