 volatile byte rpmcount;
 unsigned int rpm;
 unsigned long timeold;
 
 int motor_out = 3;
 int motor_off = 0;
 
 int speedA = 32;
 int speedB = 63;
 int speedC = 126;
 int speedD = 189;
 int speedE = 255;

 void setup()
 {
   Serial.begin(9600);
   attachInterrupt(0, rpm_fun, RISING);
   rpmcount = 0;
   rpm = 0;
   timeold = 0;
   pinMode(motor_out, OUTPUT);
   
   delay(100);
   Serial.println(" Initialize port.....");
   delay(1000);
   Serial.println("OK");
   Serial.println("");
   Serial.println("");
   Serial.println("");
   Serial.println(" this program allow you to control speed of a DC motor whith five speed settings");
   Serial.println(" while measuring its rotation speed (RPM).");
   Serial.println(" to change speed enter letters a, b, c, d, and e"); 
   Serial.println(" minimum speed = a");
   Serial.println(" maximum speed = e");
   Serial.println(" any button stop ");
    
 }
 void loop()
 {
   
   if (Serial.available() > 0) {
    int inByte = Serial.read();
    // do something different depending on the character received.  
    // The switch statement expects single number values for each case;
    // in this exmaple, though, you're using single quotes to tell
    // the controller to get the ASCII value for the character.  For 
    // example 'a' = 97, 'b' = 98, and so forth:

    switch (inByte) {
    case 'a':    
      analogWrite(motor_out, speedA);
      break;
    case 'b':    
      analogWrite(motor_out, speedB);
      break;
    case 'c':    
      analogWrite(motor_out, speedC);
      break;
    case 'd':    
      analogWrite(motor_out, speedD);
      break;
    case 'e':    
      analogWrite(motor_out, speedE);
      break;
    default:
      analogWrite(motor_out, motor_off);
   
      }
    } 
    
    
    if (rpmcount >= 20) 
   { 
     //Update RPM every 20 counts, increase this for better RPM resolution,
     //decrease for faster update
     rpm = 30*1000/(millis() - timeold)*rpmcount;
     timeold = millis();
     rpmcount = 0;
     Serial.print(rpm,DEC);
     Serial.println(" RPM");
   }
    
}
   
 void rpm_fun()
 {
   rpmcount++;
   //Each rotation, this interrupt function is run twice
 }
