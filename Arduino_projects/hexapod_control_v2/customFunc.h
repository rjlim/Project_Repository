/*


  list of function for getting information about the hexapod

*/


// debugger
void debug(String label, int Content)
{
  Serial.print(label);
  Serial.println(Content);
}

// This function send strings only using bluetooth
void data_str_via_bt(String data)
{
  Serial2.println(data);
}

// This function send integers only using bluetooth
void data_int_via_bt(int data)
{
  Serial2.println(data);
}

/*
    this function is a copy of map() function, for some unknown reason map() function
    doesn't work propery with maple mini clone baite bte-14-07
    instead of using long data type, double is used and renamed scaler
*/
double scaler(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double checkVoltage(int pin, int Vmin , int Vmax)
{
  int total = 0;    // total length of voltage sample
  int sampleLength = 10;  // number of samples
  int sampleRead[sampleLength]; // array containing voltages
  
  // set array contents to 0
  for (int arrIndex = 0; arrIndex < sampleLength ; arrIndex++)
  {
    sampleRead[arrIndex] = 0 ;
  }
  // take a sample of 10 values from analog input
  for (int arrIndex = 0; arrIndex < sampleLength ; arrIndex++)
  {
    sampleRead[arrIndex] = analogRead(pin);

  }
  // sum of all values of the array
  for (int arrIndex = 0; arrIndex < sampleLength ; arrIndex++)
  {
    total = sampleRead[arrIndex] + total;
  }
  int rawInputVal = total / sampleLength;
  double vPin = rawInputVal * (3.3 / 4095.0); // for maple mini 12bit resolution (4095) @ 3.3v
  return  scaler(vPin, 0.0, 3.3, Vmin , Vmax); // scaled the actual voltage to the specified voltage
}

// simplified servo function for sevocontroller A
void SetServoAngle_A(int ServNum, int setAngle)
{
  servoControl_A.setPWM(ServNum, 0, map(setAngle, 0, 180, SERVOMIN, SERVOMAX));
}

// simplified servo function for sevocontroller B
void SetServoAngle_B(int ServNum, int setAngle)
{
  servoControl_B.setPWM(ServNum, 0, map(setAngle, 0, 180, SERVOMIN, SERVOMAX));
}

// set servo angle function
void setServo(int servonum, int angle)
{
  switch (servonum)
  {
    case 1:
      SetServoAngle_A(1, angle);
      break;
    case 2:
      SetServoAngle_A(2, angle);
      break;
    case 3:
      SetServoAngle_A(3, angle);
      break;
    case 4:
      SetServoAngle_A(4, angle);
      break;
    case 5:
      SetServoAngle_A(5, angle);
      break;
    case 6:
      SetServoAngle_A(6, angle);
      break;
    case 7:
      SetServoAngle_A(8, angle);
      break;
    case 8:
      SetServoAngle_A(9, angle);
      break;
    case 9:
      SetServoAngle_A(10, angle);
      break;
    case 10:
      SetServoAngle_B(1, angle);
      break;
    case 11:
      SetServoAngle_B(2, angle);
      break;
    case 12:
      SetServoAngle_B(3, angle);
      break;
    case 13:
      SetServoAngle_B(4, angle);
      break;
    case 14:
      SetServoAngle_B(5, angle);
      break;
    case 15:
      SetServoAngle_B(6, angle);
      break;
    case 16:
      SetServoAngle_B(8, angle);
      break;
    case 17:
      SetServoAngle_B(9, angle);
      break;
    case 18:
      SetServoAngle_B(10, angle);
      break;
  }
}





