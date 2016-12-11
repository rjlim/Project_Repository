/*===============================================
  Program Name: bluetooth test for
                maple mini stm32
*                                               *
  -----------------------------------------------
  Author : Rey John Lim
  -----------------------------------------------
  Description: turn on/off LED when a message
               is recieved
  =============================================**/


const int onBoardLED = 33;    // pin number of on-board LED of maple mini
const int analogInputPin = 3;
char myString;
void setup() {

  Serial.begin(9600);         //begin serial monitor with baud rate of 9600
  Serial3.begin(9600);      //begin hardware serial connected to bluetooth with baud rate of 9600


  pinMode(onBoardLED, OUTPUT);      // set pin to output
  digitalWrite(onBoardLED, LOW);    // turn of LED
  pinMode(analogInputPin, INPUT_ANALOG);
  Serial.println("hello");
}

void loop() {

  // serial comm for bluetooth
  if (Serial3.available() > 0)
  {
    myString = Serial3.read();
    Serial.println( myString);
  }

  if (myString == 'a')
  {
    digitalWrite(onBoardLED, HIGH); // turn On LED
    Serial3.println(Analog_In_To_Voltage(analogInputPin));
    delay(75);
  }
  else
  {
    digitalWrite(onBoardLED, LOW); // turn Off LED
  }
  if (Serial.available() > 0)
  {
    char tring = Serial.read();
    Serial.print(tring);
    Serial.println(tring);
    
    char ch[] = "hello world";

    int len = strlen(ch);

    Serial.println(len);
  }


}


double Analog_In_To_Voltage(int pin)
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
  return vPin;
}

/*
  // send something to android app
  if (Serial.available() > 0)
  {
    char tring = Serial.read();
    Serial3.println(tring);
  }

*/
