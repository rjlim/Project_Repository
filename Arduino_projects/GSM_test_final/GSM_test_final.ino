/* 
 This is a simple program for using a e-gizmo's gsm/gprs shield.
 This sketch will echo the the AT command executed to arduio IDE terminal for debugging purposes
 
 based on the code found at http://linksprite.com/wiki/index.php5?title=SIM900_GPRS/GSM_Shield
 
 before uploading the program the software serial buffer is modified from 64, 128, or 256. modify according to your need if necessary 


 (NOTE: this mode is done using Arduino 1.5.x im not sure if it will work on the latest version)
 To change the software serial buffer do the following:
 
 1. Open the file: SoftwareSerial.h in arduino\libraries\SoftwareSerial do not run the arduino yet while doing this
 2. look for the following line #define _SS_MAX_RX_BUFF 64 // RX buffer size
 3. the original value is 64 so change the value to 128 or 256 
 4. the line will look like this #define _SS_MAX_RX_BUFF 256 // RX buffer size
 5. save the modified SoftwareSerial.h file 
 
 once done you can now upload this sketch to your gizduino128/328
 
 Created 2014
 by Rey John M. Lim
 
 */

#include <SoftwareSerial.h>    // software serial library
#include <String.h>            // string library

SoftwareSerial mySerial(4, 5); // set the SS RX and TX pin

void setup()
{
  mySerial.begin(9600);  // the GSM baud rate   
  Serial.begin(9600);    // the duino baud rate 
  delay(500);
  /* ----------------Initial AT command for the GSM module -------------------------------*/

  mySerial.print("AT\r\n");           //AT command 
  delay(200);
  ShowSerialData();                   // this will show OK in the terminal
  delay(200);
  mySerial.print("ATE1\r\n");         //echo to arduino then to the uart
  delay(200);
  ShowSerialData();                   // this will show OK in the terminal
  delay(200);
  mySerial.print("AT+CMEE=2\r\n");    //repport any error in readable format
  delay(200);
  ShowSerialData();                   // this will show OK in the terminal
  delay(200);
  mySerial.print("AT+CMGF=1\r\n");    //Because we want to send the SMS in text mode
  delay(200);
  ShowSerialData();                   // this will show OK in the terminal
  delay(200);
  mySerial.print("AT+CNMI=2,2,0,0,0 \r\n"); // set module to send SMS data to serial out upon receipt without storing it to memory
  delay(200);
  ShowSerialData();                   // this will show OK in the terminal
  delay(200);
}

void loop()
{
  // after the initial AT commads was executed you can choose the following 
  // input the following character for a certain command
  // t - send a text message
  // a - show all message
  // d - delete all message

  if (Serial.available())
    switch(Serial.read())
    {
    case 't':
      SendTextMessage(); // send a message
      break;
    case 'a':
      showallMessage();  // function to display all simcard messages
      break;
    case 'd':
      deleteAll();      // delete all message received 
      break;

    } 
  ShowSerialData();
  if (mySerial.available()) 
  {  // get data from GSM 
    Serial.write(mySerial.read());   // send data from GSM to arduino
  }
}

///SendTextMessage()
///this function is to send a sms message
void SendTextMessage()
{

  mySerial.print("AT+CMGS=");//send sms message, be careful need to add a country code before the cellphone number
  delay(50);
  mySerial.write((byte)34); // ASCII equivalent of "
  delay(50);               
  mySerial.print("09*********");  // your cell phone number here
  delay(50);
  mySerial.write((byte)34);  // ASCII equivalent of "
  delay(50);
  mySerial.println();  // carriage return
  delay(200);
  ShowSerialData();  // echo from gsm module
  delay(200);
  mySerial.println("A test message!");//the content of the message
  delay(200);
  delay(200);
  mySerial.write((char)26);//the ASCII code of the ctrl+z is 26
  delay(200);
  ShowSerialData();  // echo from gsm module
  delay(200);
  mySerial.println();
}

///showallMessage()
///this function is to show all messages in the simcard memory
void showallMessage()
{
  mySerial.print("AT+CMGL=");// show all the message
  delay(50);
  mySerial.write((byte)34); // ASCII equivalent of "
  delay(50);
  mySerial.print("ALL");
  delay(50);
  mySerial.write((byte)34);  // ASCII equivalent of "
  delay(50);
  mySerial.println();
}

void deleteAll()
{
  mySerial.println("AT+CMGD=1,4"); // delete all SMS
  delay(50);
  ShowSerialData();  // echo from gsm module
  delay(50);
}


void ShowSerialData()  // read the data of the GSM/GPRS module then echo it to the serial port of arduino
{
  while(mySerial.available()!=0)
    Serial.write(mySerial.read());
}









