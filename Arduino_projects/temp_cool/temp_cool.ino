
#define heater 3
#define temp_maxed 60   // change this value for setting maximum heating temp NOTE: temp sensor is recomended only below 100 degrees C.
#define buzzpin 4

#include <String.h>


float temp_sense_pin =   A0; 

float read_temp = 0;
float temp_celsius = 0;

int temp_set = 0, fin_flag = 0, start_out;

unsigned long start, finished, elapsed;



void setup()
{
  Serial.begin(9600); // initialize serial communication with 9600 baud per second
  analogReference(INTERNAL);

  pinMode(heater, OUTPUT);
  pinMode(buzzpin, OUTPUT);
  
  digitalWrite(buzzpin, LOW);

  digitalWrite(heater, LOW);
    Serial.println("simple temperature controlled that measure the time that it take for an object to lower its temperature ");
    Serial.println("select from a pre defined temperature in which you want to stop the time this will also start the program automaticaly");
    Serial.println("one started");
    Serial.println("to change maximum heating temp see the program line #define temp_maxed change the value next to it. ");
    Serial.println("NOTE: the temperature sensor is only recommended at 100 degrees celsius,");
    Serial.println("more than that it could permanently damage the sensor");
    Serial.println("");
    Serial.println("controls:");
    Serial.println("input s to abort ");
    Serial.println("press from 1 to 5 to select from 50 to 90 degrees cenlsius input one data at a time and start imidiately");
    Serial.println("input s to abort ");
    Serial.println("input t to check temperature");

}


void displayResult()
{
  float h, m, s, ms;
  unsigned long over;
  elapsed = finished - start;
  h = int(elapsed / 3600000);
  over = elapsed % 3600000;
  m = int(over / 60000);
  over = over % 60000;
  s = int(over / 1000);
  ms = over % 1000;
  Serial.print("cool down time: ");
  Serial.print(h, 0);
  Serial.print("h ");
  Serial.print(m, 0);
  Serial.print("m ");
  Serial.print(s, 0);
  Serial.print("s ");
  Serial.print(ms, 0);
  Serial.println("ms");
  Serial.println();
}

void loop()
{
  if (Serial.available())
  {
    switch(Serial.read())
    {
    case '1':    
      tempA();
      break;
    case '2':
      tempB();
      break;
    case '3':
      tempC();     
      break;
    case '4':
      tempD(); 
      break;
    case '5':
      tempD(); 
      break;
    case 't':
      check_temp(); 
      break;
    default:
      abort();
    }   
  }


  read_temp = analogRead(temp_sense_pin);
  temp_celsius = read_temp / 9.31;


  if ((temp_set != 0) && (temp_celsius >= temp_maxed) && (start_out == 1))
  {
    start = millis();
    Serial.println("Start cool down time log started...");
    digitalWrite(heater, LOW);
    fin_flag = 1;
    start_out = 0;
    
  }

  if ((temp_set != 0) && (temp_set >= temp_celsius) && (fin_flag == 1))
  {
    Serial.println("finished");`
    finished = millis();
    displayResult();
    digitalWrite(buzzpin, HIGH);
    delay(1000);
    digitalWrite(buzzpin, LOW);
     Serial.println("to test again just input the pre defined temperatures");
    fin_flag = 0;
  }

}


void tempA()
{
  temp_set = 50;
  Serial.println("cool temp is set to 50 degrees C");
  digitalWrite(heater, HIGH);
  start_out = 1;
}

void tempB()
{
  temp_set = 60;
  Serial.println("cool temp is set to 60 degrees C");
  digitalWrite(heater, HIGH);
  start_out = 1;
}

void tempC()
{
  temp_set =70;
  Serial.println("cool temp is set to 70 degrees C");
  digitalWrite(heater, HIGH);
  start_out = 1;
}

void tempD()
{
  temp_set = 80;
  Serial.println("cool temp is set to 80 degrees C");
  digitalWrite(heater, HIGH);
  start_out = 1;
}

void tempE()
{
  temp_set = 90;
  Serial.println("cool temp is set to 90 degrees C");
  digitalWrite(heater, HIGH);
  start_out = 1;
}

void check_temp()
{
  Serial.print("temperature in celsius = ");
  Serial.println(temp_celsius);
}



void abort()
{
  temp_set = 0;
  start_out = 0;
  Serial.println("abort test");
  digitalWrite(heater, LOW);
}








