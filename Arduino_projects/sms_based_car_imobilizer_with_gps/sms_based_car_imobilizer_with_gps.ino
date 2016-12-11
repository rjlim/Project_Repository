#include <SoftwareSerial.h>

#include <TinyGPS.h>
#include <LiquidCrystal595.h>    // include the library


LiquidCrystal595 lcd(A0,A1,A2);
SoftwareSerial mySerial(2, 3);

// EN: String buffer for the GPRS shield message
String msg = String("");
// EN: Set to 1 when the next GPRS shield message will contains the SMS message
int SmsContentFlag = 0;
//control pins of relay.
int a= 13;
int relay_b= 7;
int relay_c= 8;
float sms_lat;
float sms_lon;
byte  sms_hour, sms_minute, sms_second;
int chkspd;
int dsptmr;

// EN: Code PIN of the SIM card (if applied)
//String SIM_PIN_CODE = String( "XXXX" );

TinyGPS gps;

void getgps(TinyGPS &gps);

void setup()
{
  /*------------------------------- initialize serial communication --------------------*/


  Serial.begin(9600);                  // initialize
  mySerial.begin(9600);
  lcd.begin(20, 4);

  /*-------------------------------------- Initialize  PINs ----------------------------*/
  pinMode( 13, OUTPUT ); 
  pinMode( 7, OUTPUT ); 
  pinMode( 8, OUTPUT ); 
  digitalWrite( 13, LOW ); 
  digitalWrite( 7, LOW ); 
  digitalWrite( 8, HIGH );
  delay(500);

  /* ----------------Initial AT command for the GSM module -------------------------------*/

  lcd.setCursor(0,1);
  lcd.print("SMS BASED IM0BILISER");
  lcd.setCursor(0,2);
  lcd.print(" WITH GPS TRACKING");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0,0);

  delay(5000);
  lcd.clear();


  Serial.print("AT\r\n");           //AT command 
  delay(200);
  ShowSerialData();                   // this will show OK in the terminal
  delay(200);
  Serial.print("ATE1\r\n");         //echo to arduino then to the uart
  delay(200);
  ShowSerialData();                   // this will show OK in the terminal
  delay(200);
  Serial.print("AT+CMEE=2\r\n");    //repport any error in readable format
  delay(200);
  ShowSerialData();                   // this will show OK in the terminal
  delay(200);
  Serial.print("AT+CMGF=1\r\n");    //Because we want to send the SMS in text mode
  delay(200);
  ShowSerialData();                   // this will show OK in the terminal
  delay(200);
  Serial.print("AT+CNMI=3,3,0,0 \r\n"); // set module to send SMS data to serial out upon receipt without storing it to memory
  delay(200);
  ShowSerialData();                   // this will show OK in the terminal
  delay(200);
  lcd.clear();
  
  mySerial.println("AT+CMGD=1,4"); // delete all SMS
  delay(50);
  ShowSerialData();  // echo from gsm module
  delay(50);
  lcd.setCursor(0,0);
  lcd.print("deleting previous ");
  lcd.setCursor(0,1);
  lcd.print("commands stored in ");
  lcd.setCursor(0,2);
  lcd.print("simcard");
  delay(200);
  lcd.print(".");
  delay(200);
  lcd.print(".");
  delay(200);
  lcd.print(".");
  delay(200);
  lcd.print(".");
  delay(200);
  lcd.print(".");
  delay(200);
  lcd.print("ok");
  delay(200);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Getting GPS fix");
  delay(200);
  lcd.clear();
  
}




void loop()
{
  char SerialInByte;
  if(Serial.available())
  {       
    SerialInByte = (unsigned char)Serial.read();
    delay(5);

    // -------------------------------------------------------------------
    // EN: Program also listen to the GPRS shield message.
    // -------------------------------------------------------------------
    // EN: If the message ends with <CR> then process the message
    if( SerialInByte == 13 ){
      // EN: Store the char into the message buffer
      ProcessGprsMsg();
    }
    if( SerialInByte == 10 ){
      // EN: Skip Line feed
    }
    else {
      // EN: store the current character in the message string buffer
      msg += String(SerialInByte);
    }
  }
  byte a;
  if ( mySerial.available() > 0 ) // if there is data coming into the serial line
  {
    a = mySerial.read(); // get the byte of data

    if(gps.encode(a)) // if there is valid GPS data...
    {
      getgps(gps); // grab the data and display it on the LCD
    }
  }
}
// EN: Make action based on the content of the SMS. 
//     Notice than SMS content is the result of the processing of several GPRS shield messages.
void ProcessSms( String sms )
{

  int retrn = 1;
  if( (sms.indexOf("stat") >= 0) && (retrn == 1) )
  {
    digitalWrite( a, HIGH );
    delay(200);
    digitalWrite( a, LOW );
    delay(200);
    Sendmsg();
    delay(1000);
    digitalWrite( a, LOW );
    retrn = 0;
  }
  if( sms.indexOf("start") >= 0 )
  {
    digitalWrite( relay_c, HIGH );
    Send_msg_confirm_on();
  }
  if( sms.indexOf("stop") >= 0 )
  {
    digitalWrite( relay_c, LOW );
    Send_msg_confirm_off();
  }


}
// EN: Request Text Mode for SMS messaging
void GprsTextModeSMS()
{
  Serial.println( "AT+CMGF=1" );
}

void GprsReadSmsStore( String SmsStorePos )
{
  Serial.print( "AT+CMGR=" );
  Serial.println( SmsStorePos );
}

// EN: Clear the GPRS shield message buffer
void ClearGprsMsg()
{
  msg = "";
}

// EN: interpret the GPRS shield message and act appropiately
void ProcessGprsMsg() {
  if( msg.indexOf( "Call Ready" ) >= 0 )
  {
    //  Serial.println( "*** GPRS Shield registered on Mobile Network ***" );
    GprsTextModeSMS();
  }

  // EN: unsolicited message received when getting a SMS message
  if( msg.indexOf( "+CMTI" ) >= 0 )
  {
    //  Serial.println( "*** SMS Received ***" );
    // EN: Look for the coma in the full message (+CMTI: "SM",6)
    //     In the sample, the SMS is stored at position 6
    int iPos = msg.indexOf( "," );
    String SmsStorePos = msg.substring( iPos+1 );
    //  Serial.print( "SMS stored at " );
    //   Serial.println( SmsStorePos );     
    // EN: Ask to read the SMS store
    GprsReadSmsStore( SmsStorePos );
  }

  // EN: SMS store readed through UART (result of GprsReadSmsStore request)  
  if( msg.indexOf( "+CMGR:" ) >= 0 )
  {
    // EN: Next message will contains the BODY of SMS
    SmsContentFlag = 1;
    // EN: Following lines are essentiel to not clear the flag!
    ClearGprsMsg();
    return;
  }

  // EN: +CMGR message just before indicate that the following GRPS Shield message 
  //     (this message) will contains the SMS body 
  if( SmsContentFlag == 1 )
  {
    //   Serial.println( "*** SMS MESSAGE CONTENT ***" );
    //   Serial.println( msg );
    //   Serial.println( "*** END OF SMS MESSAGE ***" );
    ProcessSms( msg );
  }

  ClearGprsMsg();
  // EN: Always clear the flag
  SmsContentFlag = 0; 
}

void ShowSerialData()  // read the data of the GSM/GPRS module then echo it to the serial port of arduino
{
  while(Serial.available()!=0)
    lcd.write(Serial.read());
}
/*----------------------------------------------------------------------------------------------------------------------------------------*/
void Sendmsg()
{
  Serial.print("AT+CMGS=");//send sms message, be careful need to add a country code before the cellphone number
  digitalWrite( a, HIGH );
  delay(200);

  Serial.write((byte)34); // ASCII equivalent of "
  digitalWrite( a, LOW );
  delay(200);

  Serial.print("09052130737");  // your cell phone number here   09359368583 - rose ann
  digitalWrite( a, HIGH );
  delay(200);

  Serial.write((byte)34);  // ASCII equivalent of "
  digitalWrite( a, LOW );
  delay(200);
  
  Serial.println();  // carriage return
  digitalWrite( a, HIGH );
  delay(200);
  
  Serial.println("status check");  // message content
  digitalWrite( a, LOW );
  delay(50);
  
  Serial.print("latitude:");   // message content
  digitalWrite( a, HIGH );
  delay(50);
  
  Serial.println(sms_lat, 7);  // message content
  digitalWrite( a, LOW );
  delay(50);
  
  Serial.print("longtitude");   // message content
  digitalWrite( a, HIGH );
  delay(50);
  
  Serial.println(sms_lon, 7);   // message content
  digitalWrite( a, LOW );
  delay(50);
  
  sms_time();
  digitalWrite( a, HIGH );
  delay(50);
  
  Serial.write((char)26);//the ASCII code of the ctrl+z is 26
  digitalWrite( a, LOW );
  delay(200);
  
  Serial.println();
  digitalWrite( a, HIGH );
}

void Send_msg_confirm_on()
{
   Serial.print("AT+CMGS=");//send sms message, be careful need to add a country code before the cellphone number
  digitalWrite( a, HIGH );
  delay(200);

  Serial.write((byte)34); // ASCII equivalent of "
  digitalWrite( a, LOW );
  delay(200);

  Serial.print("09052130737");  // your cell phone number here   09359368583 - rose ann
  digitalWrite( a, HIGH );
  delay(200);

  Serial.write((byte)34);  // ASCII equivalent of "
  digitalWrite( a, LOW );
  delay(200);
  
  Serial.println();  // carriage return
  digitalWrite( a, HIGH );
  delay(200);
  
  Serial.println("confirmation: car engine enabled");  // message content
  digitalWrite( a, LOW );
  delay(50);
  
  Serial.println("status check");  // message content
  digitalWrite( a, LOW );
  delay(50);
  
  Serial.print("latitude:");   // message content
  digitalWrite( a, HIGH );
  delay(50);
  
  Serial.println(sms_lat, 7);  // message content
  digitalWrite( a, LOW );
  delay(50);
  
  Serial.print("longtitude");   // message content
  digitalWrite( a, HIGH );
  delay(50);
  
  Serial.println(sms_lon, 7);   // message content
  digitalWrite( a, LOW );
  delay(50);
  
  sms_time();
  digitalWrite( a, HIGH );
  delay(50);
  
  Serial.write((char)26);//the ASCII code of the ctrl+z is 26
  digitalWrite( a, LOW );
  delay(200);
  
  Serial.println();
  digitalWrite( a, LOW ); 
}

void Send_msg_confirm_off()
{
   Serial.print("AT+CMGS=");//send sms message, be careful need to add a country code before the cellphone number
  digitalWrite( a, HIGH );
  delay(200);

  Serial.write((byte)34); // ASCII equivalent of "
  digitalWrite( a, LOW );
  delay(200);

  Serial.print("090xxxxxxxx");  // mobile phone here <--- that number is only example 
  digitalWrite( a, HIGH );
  delay(200);

  Serial.write((byte)34);  // ASCII equivalent of "
  digitalWrite( a, LOW );
  delay(200);
  
  Serial.println();  // carriage return
  digitalWrite( a, HIGH );
  delay(200);
  
  Serial.println("confirmation: car engine disabled");  // message content
  digitalWrite( a, LOW );
  delay(50);
  
  Serial.println("status check");  // message content
  digitalWrite( a, LOW );
  delay(50);
  
  Serial.print("latitude:");   // message content
  digitalWrite( a, HIGH );
  delay(50);
  
  Serial.println(sms_lat, 7);  // message content
  digitalWrite( a, LOW );
  delay(50);
  
  Serial.print("longtitude");   // message content
  digitalWrite( a, HIGH );
  delay(50);
  
  Serial.println(sms_lon, 7);   // message content
  digitalWrite( a, LOW );
  delay(50);
  
  sms_time();
  digitalWrite( a, HIGH );
  delay(50);
  
  Serial.write((char)26);//the ASCII code of the ctrl+z is 26
  digitalWrite( a, LOW );
  delay(200);
  
  Serial.println();
  digitalWrite( a, HIGH ); 
}


void sms_time()
{
  Serial.println("24 Hour time format");
  Serial.print("Time: ");

  if (sms_hour > 23)
  {
    sms_hour = sms_hour - 24;
  }

  if (sms_hour<10)
  {

    Serial.print("0");
  }
  Serial.print(sms_hour, DEC);
  Serial.print(":");
  if (sms_minute<10)
  {
    Serial.print("0");
  }
  Serial.print(sms_minute, DEC);
  Serial.print(":");
  if (sms_second<10)
  {
    Serial.print("0");

  }
  Serial.print(sms_second, DEC);
}


void getgps(TinyGPS &gps)
// The getgps function will display the required data on the LCD
{
  float latitude , longitude;
  int year;
  float speedo = gps.f_speed_kmph() ;
  byte month, day, hour, minute, second, hundredths;
  //decode and display position data
  gps.f_get_position(&latitude, &longitude);
  gps.crack_datetime(&year,&month,&day,&hour,&minute,&second,&hundredths);
  lcd.setCursor(0,0);
  lcd.print("->Lat: ");
  lcd.print(latitude,7);
  lcd.print(" ");
  lcd.setCursor(0,1);
  lcd.print("->Lon: ");
  lcd.print(longitude,7);
  lcd.print(" ");
  lcd.setCursor(0,2);
  lcd.print("->Time:");
  hour=hour+8;
  sms_lat = latitude;
  sms_lon = longitude;
  sms_hour = hour;
  sms_minute = minute;
  sms_second = second;
  chkspd = speedo;
  if (hour>23)
  {
    hour=hour-24;
  }

  if (hour<10)
  {

    lcd.print("0");
  }
  lcd.print(hour, DEC);
  lcd.print(":");
  if (minute<10)
  {
    lcd.print("0");
  }
  lcd.print(minute, DEC);
  lcd.print(":");
  if (second<10)
  {
    lcd.print("0");

  }
  lcd.print(second, DEC);
  lcd.print(" ");
  lcd.setCursor(0,3);
  lcd.print(chkspd);
  lcd.print("s Spd:");
  lcd.print(speedo);
  lcd.println("km/h");

  delay(10); // wait for 3 seconds 
}

void car_park_timer()
{
  while(chkspd == 0)
  {
    for(int tmr=120 ; tmr != 0; tmr--)
    {
      tmr = dsptmr;
      delay(1000);
    }
   if((chkspd == 0) && (dsptmr == 0))
   {
     Send_msg_confirm_off();
   }
  }
}





