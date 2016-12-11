
#include "init_ctrl.h"
#include "customFunc.h"
#include "presets.h"
#define INPUT_SIZE 7

#define BvatSensePin_A 3  // maple mini analog pin input
#define BvatSensePin_B 4 // maple mini analog pin input

int cmdAddr = 0, cmdVal = 5;

int previousMillis = 0;        // will store the last time the LED was updated
int interval = 1000;           // interval at which to blink (in milliseconds)

const int lowbatLED = 14,      // Led Indicator when Low Battery
          EnableBtPin = 18;       // enable/diable servo when programming while hexpod is on



void setup() {

  // initialize servo controllers
  initlializeStuff();
  pinMode(lowbatLED, OUTPUT);
  // initialize serial
  initSerialComms(115200, 115200);
  pinMode(18, OUTPUT);
  digitalWrite(EnableBtPin, LOW);
  // stand and wait for further instructions
  standPosition();
  //undeploy();
  delay(1000);
  digitalWrite(EnableBtPin, HIGH);

}

void loop() {

  /* =======================================================================//
    ||            SEND BATTERY VOLTAGE TO ANDROID ON A CERTAIN INTERVAL     ||
    ========================================================================*/
  if (millis() - previousMillis > interval) {
    // Save the last time you blinked the LED
    previousMillis = millis();

    // If the LED is off, turn it on, and vice-versa:
    //  digitalWrite(33, !digitalRead(33)); // Turn the LED from off to on, or on to off
    double vbatt_A = checkVoltage(BvatSensePin_A, 0, 10);
    double vbatt_B = checkVoltage(BvatSensePin_B, 0, 5);
    //Serial.print("CV:");
    //Serial.println(vbatt_A);
    Serial2.print("1.1/");
    Serial2.print(vbatt_A - vbatt_B);
    Serial2.print("-");
    Serial2.print("2.1/");
    Serial2.print(vbatt_B);
    Serial2.println("-");

    // blink led when lowbat NOTE: change output pin to another
    if (((vbatt_A - vbatt_B) < 3.4) || vbatt_B < 3.4 )
    {
      digitalWrite(lowbatLED, !digitalRead(lowbatLED));
    }
    else
    {
      digitalWrite(lowbatLED, LOW);
    }
    Serial2.flush();
  }
  /* =======================================================================//
    ||               GET DATA FROM SERIAL2 (BLUETOOTH)                      ||
    ========================================================================*/
  if (Serial2.available() > 0) {
    // Get next command from Serial (add 1 for final 0)
    char input[INPUT_SIZE + 1];
    byte sizes = Serial2.readBytes(input, INPUT_SIZE);
    // Add the final 0 to end the C string
    input[sizes] = 0;
    // Read each command pair
    char* command = strtok(input, "-");

    while (command != 0) {
      // Split the command in 2 values
      char* separator = strchr(command, '/');

      if (separator != 0) {
        // Actually split the string in 2: replace ':' with 0
        *separator = 0;
        int controlID = atoi(command);
        ++separator;
        int IDcommand = atoi(separator);

        if (controlID != 0 && IDcommand != 0)
        {
          cmdAddr = controlID;
          cmdVal = IDcommand ;
        }
      }
      //Serial.print(cmdAddr);
      //Serial.print("/");
      //Serial.print(cmdVal);
      //Serial.println("-");
      // Find the next command in input string
      command = strtok(0, "-");
      // Serial2.println("99/99-");
      Serial2.flush();

      setServo(cmdAddr, cmdVal);
      // run preset movement
      if (cmdAddr > 19)
      {
        switch (cmdAddr)
        {
          case 20:
            tripodwalk(cmdVal);
           // InverseTripodWalk(cmdVal);
            cmdAddr = 0;
            break;
          case 21:
            reverseTripodWalk(cmdVal);
            cmdAddr = 0;
            break;
          case 22:
            turnLeft(cmdVal);
            cmdAddr = 0;
            break;
          case 23:
            turnRight(cmdVal);
            cmdAddr = 0;
            break;
          case 24:
            standPosition();
            cmdAddr = 0;
            break;
        }
      }
    }
  }


  /* =======================================================================//
    |         GET DATA FROM SERIAL (SEND TO SERIAL MONITOR)                  ||
    ==========================================================================*/

  // hardware serial
  if (Serial.available() > 0) {
    // Get next command from Serial (add 1 for final 0)
    char input[INPUT_SIZE + 1];
    byte sizes = Serial.readBytes(input, INPUT_SIZE);
    // Add the final 0 to end the C string
    input[sizes] = 0;
    // Read each command pair
    char* command = strtok(input, "-");

    while (command != 0) {
      // Split the command in 2 values
      char* separator = strchr(command, '/');

      if (separator != 0) {
        // Actually split the string in 2: replace ':' with 0
          
        
        *separator = 0;
        int controlID = atoi(command);
      Serial.println(command);
        ++separator;
         Serial.println(separator);
        int IDcommand = atoi(separator);

        cmdAddr = controlID;
        cmdVal = IDcommand ;
        setServo(cmdAddr, cmdVal);
      }

      // Find the next command in input string
      command = strtok(0, "-");
      Serial.flush();
      Serial.println("99/99-");

      setServo(cmdAddr, cmdVal);
      // run preset movement
      if (cmdAddr > 19)
      {
        switch (cmdAddr)
        {
          case 20:
            tripodwalk(cmdVal);
            cmdAddr = 0;
            break;
          case 21:
            reverseTripodWalk(cmdVal);
            cmdAddr = 0;
            break;
          case 22:
            turnLeft(cmdVal);
            cmdAddr = 0;
            break;
          case 23:
            turnRight(cmdVal);
            cmdAddr = 0;
            break;
          case 24:
            standPosition();
            cmdAddr = 0;
            break;
        }
      }
    }
  }
}

