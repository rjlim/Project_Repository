/*===============================================
  Program Name: bluetooth test with
                maple mini stm32
*                                               *
  -----------------------------------------------
  Author : Rey John Lim
  -----------------------------------------------
  Description: turn on/off LED when a message
               is recieved and send analog value as voltage
              data is sent to an android application
              and plot the voltage into a graph
  =============================================**/


const int onBoardLED = 33;    // pin number of on-board LED of maple mini
const int analogInputPin = 3;
char ch1, ch2;
void setup() {

  //Serial.begin(9600);         //begin serial monitor with baud rate of 9600
  Serial3.begin(9600);          //begin hardware serial connected to bluetooth with baud rate of 9600

  // Serial.print("READY!");
  pinMode(onBoardLED, OUTPUT);      // set pin to output
  digitalWrite(onBoardLED, LOW);    // turn of LED
  pinMode(analogInputPin, INPUT_ANALOG);
}

void loop() {

  // serial comm for bluetooth
  if (Serial3.available())
  {
    ch1 =  Serial3.read();
    if (ch1 == 'a')
    {
      digitalWrite(onBoardLED, HIGH); // turn On LED
      ch1 =  Serial3.read();
    }
    else
    {
      digitalWrite(onBoardLED, LOW); // turn Off LED
    }
  }
  // for some unknown reason, sending data via bluetooth below 25ms causes android app to stop
  // show data on the plotter
  delay(25);
  Serial3.print("-l");
  Serial3.println(Analog_In_To_Voltage(analogInputPin));
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
  // get the average value
  int rawInputVal = total / sampleLength;
  // convert the value into voltage from 0 to 3.3v
  // for maple mini 12bit resolution (4095) @ 3.3v
  double vPin = rawInputVal * (3.3 / 4095.0);
  return vPin;
}


