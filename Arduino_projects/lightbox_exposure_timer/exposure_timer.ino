#define but1 6
#define but2 7
#define but3 9
#define outpin 13

#include <LiquidCrystal595.h>    // include the library
LiquidCrystal595 lcd(2,3,4);     // datapin, latchpin, clockpin
                                 // lcd(datapin, latchpin, clockpin)
                                 
int up = 0;
int down = 0;
int  start_stop = 0;

int state = HIGH;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin
long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers

void setup()
{
  pinMode(but1, INPUT);
  pinMode(but2, INPUT);
  pinMode(but3, INPUT);
  pinMode(outpin, OUTPUT);
  
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print(" Exposure Timer");
  lcd.setCursor(0, 1);
  lcd.print(" v1.0 by R.J.L.  ");
  delay(2000);
  
}

unsigned int time_val = 0 ;
int x, x1;


void button_read()
{
  up = digitalRead(but1);
  down = digitalRead(but2);
  reading = digitalRead(but3);
}

void button_select()
{
  if(up == LOW)
     {
      time_val++;
      delay(1);
     }
    
   if(down == LOW)
     { 
      time_val--;
      delay(1);
     }
     
   if(up == LOW && down == LOW)
     {
       time_val = 0;
       delay(1);
     }
     
   if(time_val == 65535 || time_val == 61)
   { 
     time_val = 0;
   }
   
   
}

void latch_sw()
{
    if (reading == HIGH && previous == LOW && millis() - time > debounce) 
      {
        if (state == HIGH)
          
           state = LOW;
          
          else
          state = HIGH;
 
          time = millis();    
  }
    
 
  
  previous = reading;
}
void loop()
{

  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Set Time (Min)");
  lcd.setCursor(0,1);
  lcd.print("     ");
  lcd.print(time_val);
  lcd.print(" Min");
  
  button_read();
  button_select();
  latch_sw();
  
  while(state == HIGH)
  {
    digitalWrite(outpin, state);
    
     if(time_val !=0 && state == HIGH)
     {
            time_val--;
       
            for(int x = 60 ; x != 0 ; x--)
            {
             lcd.clear();
             lcd.print(" Time Remaining");
             lcd.setCursor(0,1);
             lcd.print(" ");
             lcd.print(time_val);
             lcd.print(" Min. ");
             lcd.print(x);
             lcd.print(" Sec. ");
             delay(1000); 
            }
         
            if(time_val == 0 && x == 0)
            { 
             time_val = 0;
             state = LOW;
             digitalWrite(outpin, state);
             lcd.clear();
             lcd.print("      STOP      ");
             delay(1000);
            }
     }
     
     else if(time_val == 0 && state == HIGH)  
     {
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("  time not set");
       digitalWrite(outpin, LOW);
       delay(1000);
       state = LOW;
       time_val = 0;
     }
     

     
   
  }

 

 
delay(100);   
}
