/*

 bot program for duino uno 
 
 by: Rey John M. Lim
 
 */

//-------------define digital pins-----------------------------------

#define MTright_ctrl_1   4     // motor left side 1  (IN 1)
#define MTright_ctrl_2   5     // motor left side 2  (IN 2)
#define MTleft_ctrl_1    6     // motor right side 1 (IN 3)
#define MTleft_ctrl_2    7     // motor right side 2 (IN 4)
//#define MTpwm_ctrl_1          // motor speed control by PWM EN_A
//#define MTpwm_ctrl_2          // motor speed control by PWM EN_B

//-------------define analog inputs --------------------------------- 

#define IRS_left         A0           // IR sensor left
#define IRS_center       A1           // IR sensot center
#define IRS_right        A2           // IR sensor right

void setup()
{ 
  //--------------- initilize digital pin (input or output) ------------------
      pinMode(MTright_ctrl_1, OUTPUT);
      pinMode(MTright_ctrl_2, OUTPUT);
      pinMode(MTleft_ctrl_1, OUTPUT);
      pinMode(MTleft_ctrl_2, OUTPUT);
      
  //-------------- set internal ADC reference for IR Sensor ------------
      analogReference(INTERNAL);
      
  //-------------- set Serial communication for debugging ---------------  
      Serial.begin(9600);
}

//-----------------motor function------------------------------------


void fwrd()
  {
    digitalWrite(MTright_ctrl_1, HIGH);
    digitalWrite(MTright_ctrl_2,  LOW);
    digitalWrite(MTleft_ctrl_1,  HIGH);
    digitalWrite(MTleft_ctrl_2,   LOW);
    
  }
  
void rvrse()
  {
    digitalWrite(MTright_ctrl_1,  LOW);
    digitalWrite(MTright_ctrl_2, HIGH);
    digitalWrite(MTleft_ctrl_1,   LOW);
    digitalWrite(MTleft_ctrl_2,  HIGH);
    
  }

void T_left()
  {
    digitalWrite(MTright_ctrl_1, HIGH);
    digitalWrite(MTright_ctrl_2,  LOW);
    digitalWrite(MTleft_ctrl_1,   LOW);
    digitalWrite(MTleft_ctrl_2,  HIGH);
  }


void T_right()
  {
    digitalWrite(MTright_ctrl_1,  LOW);
    digitalWrite(MTright_ctrl_2, HIGH);
    digitalWrite(MTleft_ctrl_1,  HIGH);
    digitalWrite(MTleft_ctrl_2,   LOW);
  }
  
void MTstop()
  {
    digitalWrite(MTright_ctrl_1, LOW);
    digitalWrite(MTright_ctrl_2, LOW);
    digitalWrite(MTleft_ctrl_1,  LOW);
    digitalWrite(MTleft_ctrl_2,  LOW);
  }
  
//--------------- initialize variables ---------------------

  int IR_sensor_left = 0;     // read the value from left IR sensor
  int IR_sensor_center = 0;   // read the value from center IR sensor
  int IR_sensor_right = 0;    // read the value from right IR sensor
  int total_adc_val = 0;
  
//---------------- main loop -----------------------

void loop()
{
  
    IR_sensor_left = analogRead(IRS_left)/100;                // read analog input from left IR
   
    IR_sensor_center = analogRead(IRS_center)/100;              // read analog input from center IR
   
    IR_sensor_right = analogRead(IRS_right)/100;               //  read analog input from right IR
    
    total_adc_val = (IR_sensor_left + IR_sensor_center + IR_sensor_right )/3;
    
    if(total_adc_val >= 10)
    {
      /* if the average value of 3 IR sensor is greater than a certain value, run the random to choose either to left or right turn */
      MTstop();
      delay(1500);
      rvrse();
      delay(200);
      T_right();
      delay(500);
      T_left();
      Serial.print("no available route");
      Serial.print(" IR left = ");
      Serial.print(IR_sensor_left);
      Serial.print(" IR center = ");
      Serial.print(IR_sensor_center);
      Serial.print(" IR right = ");
      Serial.println(IR_sensor_right);       
    }  
    else if(((IR_sensor_left >= 10)&&(IR_sensor_right >= 10)))
    {
      MTstop();
      delay(500);
      rvrse();
      delay(1000);
      T_right();
      delay(1000);
    } 
    else if(((IR_sensor_center >= 10 )&&(IR_sensor_right >= 10)))
    {
      /*if right and center or right IR sensor only  is greater than certain value turn left */
      T_left();
      Serial.print("turning left");
      Serial.print(" IR left = ");
      Serial.print(IR_sensor_left);
      Serial.print(" IR center = ");
      Serial.print(IR_sensor_center);
      Serial.print(" IR right = ");
      Serial.println(IR_sensor_right); 
    }
    else if(IR_sensor_right >= 10)
    {
      /*for right IR sensor only  */
      T_left();
      Serial.print("turning left");
      Serial.print(" IR left = ");
      Serial.print(IR_sensor_left);
      Serial.print(" IR center = ");
      Serial.print(IR_sensor_center);
      Serial.print(" IR right = ");
      Serial.println(IR_sensor_right); 
    }
    
    else if(((IR_sensor_center >= 10 )&&(IR_sensor_left >= 10)))
    {
      /*if left and center or left IR sensor only  is greater than certain value turn right */
      T_right();
      Serial.print("turning right");
      Serial.print(" IR left = ");
      Serial.print(IR_sensor_left);
      Serial.print(" IR center = ");
      Serial.print(IR_sensor_center);
      Serial.print(" IR right = ");
      Serial.println(IR_sensor_right);
    }
     else if(IR_sensor_left >= 10)
    {
      /*separated code for left sensor only*/
      T_right();
      Serial.print("turning right");
      Serial.print(" IR left = ");
      Serial.print(IR_sensor_left);
      Serial.print(" IR center = ");
      Serial.print(IR_sensor_center);
      Serial.print(" IR right = ");
      Serial.println(IR_sensor_right);
    }
    else
    {
    /* just continue moving forward */
    fwrd();
    Serial.print(" forward");
    Serial.print(" IR left = ");
    Serial.print(IR_sensor_left);
    Serial.print(" IR center = ");
    Serial.print(IR_sensor_center);
    Serial.print(" IR right = ");
    Serial.println(IR_sensor_right);
    }
    
  // conditional statement for IR sensors
  

}


