/*===============================================
  Program Name: simple car alarm project 
                 RJL car alarm ver 1                  
  -----------------------------------------------
  Author : Rey John Lim
  -----------------------------------------------
  Description: car alarm project using arduino mega
               this code is part of my assessment 
               in TAFE NSW
  =============================================**/

/*
  assuming the situation is:

  1. car is parked, engine is off and all doors are closed to be armed.

  2. car will alarm if any of the doors are opened
   whilst remoteArmed or Armed is set and when hijcacked.



Code Description and limitiation:

***car is armed or remoteArmed in the state of:

  1. car is parked, engine is off and all doors are closed.

  2. cannot be armed or remoteArmed if any of the doors are opened

  3. cannot be armed or remoteArmed if ignition is ON.

  4. armed or remoteArmed is set and armed counter is 10
   and any of the doors are closed.

   (Assuming that: A. car is armed from inside, there will be a
                      10 sec delay before the car is armed.
                      This will provide time for the driver to
                      close the driver's side door assuming that
                      the rest of doors are closed beforehand.

                   B. there will be no delay if when remoteArmed
                      is pressed. cannot be armed if any of the
                      door is opend or ignition is ON)

   5. diplay on the terminal which doors are not closed

*** car alarmed in the state of:

  1.  Armed or remoteArmed is set and any of the doors are open.

  2.  siren beeps for 20 sec.

  3.  alarmed state is deactivated if:

        A. remoteArmed is pressed.

        B. Armed button is pressed for 10 sec.


***car is hijack if the following conditions are met:

  1. Armed or remoteArmed is set and any of the doors are open for 20 sec.

  2. and if someone turn on the ignition switch while on alarmed state.

  3. the ignition cutoff will activate in the state of alarmed and ignition is turn On

  5. hijacked state is active if alarmed, ignition cutoff are on.

  6. hijacked state is deactivated if:

          A.  the driver/owner pressed armed, brake, ignition, and right door is open
              for 10 sec.

          B.  deactivated by remoteArmed.

*** car is in mobilized if the following conditions are met

  1. car is not in armed state or in remote armed state

  2. all doors must be closed
     (except for the boot assuming the situation where
       you have a over sized box that doesn't fit the boot )

       display warning that the boot is open.

  2. car brakes and ignition are pressed to start the car

*/

#define armed       1
#define unarmed     2
#define mobilized   3
#define rntpend1    4
#define hijacked    5
#define prearmed    7
#define armpending  6
#define alarmed     9
#define silence     10
#define pre_armed   11    // preliminary check before arming
 
/* delay*/

#define half   500
#define one    1000




/* +------------------+
   | SKETCH CONSTANTS |
   +------------------+ */

const int ldoor  =   2;   // if LHS/NKS door close led1 = off,p2 = high
const int rdoor  =   3;   // if RHS/OKS door close led2 = off, p3 = high
const int brake  =   4;   // if not pressed led3 = off led 4 = off, p4 = low
const int rmtpb  =   5;   // if not p/b not pressed  i/p5 = low
const int armpb  =   6;   // if armed p/b not pressed i/p6 = high
const int ignsw  =   7;   // if IGN sw4 = ON & IGNco relay= off i/p7= high
const int bonnet =   8;   // bonnet = close, led7 = off and i/p8= high
const int boot   =   9;   // if boot = closed, led8 = off and i/p9=high
const int dome   =   10;  // if sw8 = on , dome light led9 = on & i/p10 =low
const int tilt   =   11;  // if car tilt is shocked, TILT led10 = on , i/p11 = low

const int aled   =   15;  // if o/p15 = low, Aled LED11 = ON
const int hled   =   16;  // if o/p16 = low, hled LED12 = ON
const int siren  =   17;  // if o/p17 = high, siren = ON
const int ignco  =   18;  // if o/p19 = low LED15,LED13,relay all ON
const int fuelco =   19;  // if o/p18 = low led14, led13 both are ON


/* +------------------+
   | SKETCH VARIABLES |
   +------------------+ */
   
/* counter */
int state      = 0;
int armcnt     = 0;
int aledcnt    = 0;
int hledcnt    = 0;
int wailcnt    = 0;
int hjackcnt   = 0;
int sirencnt   = 0;
int rmtp1cnt   = 0;
int silentcnt  = 0;
int mobilcnt   = 0;
int armpendcnt = 0;

int alarmCnt = 0;

/* status */

int ldost, rdost, 
    brkst, rmtst, 
    armst , ignst, 
    bnetst, bootst, 
    domest, tiltst;


int armpb_out = LOW;       // the current state of the output pin
int armst_prev = HIGH;    // the previous reading from the input pin


int rmtpb_out = LOW;       // the current state of the output pin
int rmtpb_prev = HIGH;    // the previous reading from the input pin
int remoteArmed = 0;


// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long timed_armedpb = 0;         // the last time the output pin was toggled
long timed_rmtpb = 0;           // the last time the output pin was toggled
long debounce = 200;            // the debounce time, increase if the output flickers



/* +--------------+
   | SKETCH SETUP |
   +--------------+ */

void setup() {
  Serial.begin(115200);

  pinMode(ldoor, INPUT);    // if ldoor = closed ip2 = h & LED1 = off
  pinMode(rdoor, INPUT);    // if rdoor = closed ip3 = h & LED2 = off
  pinMode(brake, INPUT);    // if brake = not pressed, ip4 & LED 3 & 4 = OFF
  pinMode(rmtpb, INPUT);     // if not p/b not pressed  i/p5 = low
  pinMode(armpb, INPUT);     // if armed p/b not pressed i/p6 = high
  pinMode(ignsw, INPUT);     // if IGN sw4 = ON & IGNco relay= off i/p7= high
  pinMode(bonnet, INPUT);    //if bonnet = close, led7 = off and i/p8= high
  pinMode(boot, INPUT);      // if boot = closed, led8 = off and i/p9=high
  pinMode(dome, INPUT);      // if sw8 = on , dome light led9 = on & i/p10 =low
  pinMode(tilt, INPUT);      // if car tilt is shocked, TILT led10 = on , i/p11 = low
  pinMode(aled , OUTPUT);    // if o/p15 = low, Aled LED11 = ON
  pinMode(hled, OUTPUT);     // if o/p16 = low, hled LED12 = ON
  pinMode(siren, OUTPUT);    // if o/p17 = high, siren = ON
  pinMode(ignco, OUTPUT);    // if o/p19 = low LED15,LED13,relay all ON
  pinMode(fuelco, OUTPUT);   // if o/p18 = low led14, led13 both are ON

  digitalWrite(aled, LOW);   // Aled LED 1 = OFF
  digitalWrite(hled, LOW);   // HLED LED12 = OFF
  digitalWrite(siren, LOW);  // Siren = mute
  digitalWrite(ignco, LOW);  // IGNco relay =  & eng-disable LED13=OFF
  digitalWrite(fuelco, LOW); //  fuelco  LED14 & eng-disable LED13 = OFF

  serialPrintInstruct();
  Serial.println("Ready");
  state = unarmed;
}

void loop() {


  pinRead();
  if (ignst && brkst && armst && state == unarmed)
  {
    Serial.println("MOBILIZED");
    state = mobilized;
  }


  rmtst  =  digitalRead(rmtpb);
  armst = digitalRead(armpb);
  ignst  =  digitalRead(ignsw);
  /*================================ LATCH  ARM =====================================*/
  if (armst == LOW && armst_prev == HIGH && (millis() - timed_armedpb > debounce) && (!ignst))
  {
    if (armpb_out == HIGH)
    {
      armpb_out = LOW;
      armcnt = 0;
      state = unarmed;
      Serial.println("Unarmed");
    }
    else
    {
      armpb_out = HIGH;
      state = pre_armed;
      Serial.println("Armed");
    }
    timed_armedpb = millis();
  }
  armst_prev = armst;
  digitalWrite(aled, armpb_out);
  /*===============================================================================*/


  /*============================ LATCH REMOTE ARM =================================*/
  if ( rmtst == LOW && rmtpb_prev == HIGH && (millis() - timed_rmtpb > debounce) && (!ignst))
  {
    if (rmtpb_out == HIGH)
    {
      rmtpb_out = LOW;
      state = unarmed;
      remoteArmed = 0;
      Serial.println("Remote Unarmed");
      digitalWrite(siren, LOW); // Siren = mute

      digitalWrite(hled, LOW);  // HLED LED12 = OFF

      digitalWrite(ignco, LOW); // IGNco relay =  & eng-disable LED13=OFF
      digitalWrite(fuelco, LOW); //  fuelco  LED14 & eng-disable LED13 = OFF
    }
    else
    {
      rmtpb_out = HIGH;
      remoteArmed = 1;
      state = pre_armed;
      Serial.println("Remote Armed");
    }
    timed_rmtpb = millis();
  }
  rmtpb_prev = rmtst;

  digitalWrite(hled, rmtpb_out);
  /*===============================================================================*/

  // preliminary check before setting the car to armed state
  // and provide the driver some time to close all the doors
  if (state == pre_armed)
  {
    // wait for 10 sec before doing the next line of code
    while (armcnt < 10  && !(remoteArmed))
    {
      armcnt++;
      delay(1000);
      statusDebug("Arming in...", 11 - armcnt );
    }

    //check all door if they are all closed
    pinRead();
    // after waiting for 10 sec check if any of these doors are closed
    if (armcnt <= 10 && (ldost) && (rdost) && (bootst) && (bnetst) && (domest))
    {
      Serial.println("all doors are closed...");
      Serial.println("System is now armed");
      state = armed;
    }
    else
    {
      Serial.println("System Cannot be Armed");
      checkDoors();
      state = unarmed;
    }
  }
  
  // else if remote Armed is pressed
  // read all inputs;
  pinRead();
  // check if car state is armed and waited for 10 sec
  // or remote push button is
  if ( (state == armed && armcnt <= 10) || (state == armed && remoteArmed) )
  {
    if ((!ldost) || (!rdost) || (!bootst) || (!bnetst) || (!domest))
    {
      Serial.println("Car Alarmed");
      state = alarmed;
    }
  }

  // car is on alarm state and will wait for 20 sec
  // if all conditions are met it will consider it as false alarm and
  // go back to armed state
  if (state == alarmed && !ignst)
  {
    // wait for 20 sec
    while (alarmCnt < 20 )
    {
      pinRead();
      //and check all doors for every second. if yes go back to rearmed state
      if ((ldost) && (rdost) && (bootst) && (bnetst) && (domest) && !ignst)
      {
        Serial.println(alarmCnt);
        // go back to armed state
        if (alarmCnt == 19)
        {
          Serial.println("False alarm... System rearming");
          state = armed;
        }
      }
      else
      {
        // if this condition is met then it is hijacked
        // assuming that the hijacker is able to get into the car
        // and turn on the ignition for less than 20 sec. <-- this guy is a PRO ('',)
        if (ignst == 1 && alarmCnt <= 20)
        {
          Serial.println("HIJACKED!!!");
          state = hijacked;
        }
        checkDoors();
      }
      alarmCnt++;
      delay(80);
      digitalWrite(siren, HIGH); // Siren = mute

      delay(80);
      digitalWrite(siren, LOW); // Siren = mute
      delay(1000);
    }
    alarmCnt = 0;
  }


  // assuming the situation in which the hijacker manager to enter the car,
  // close the door , hotwire for more than 20 sec and triggers the false alarm
  // and putting the car on armed state.
  if (state == armed && ignst)
  {
    state = hijacked;
  }


  // when car is parked and someone tries to
  // tow the car
  pinRead();
  if (state == armed && !ignst && !tiltst)
  {
    Serial.println("TOWED");
    state = hijacked;
  }


  while (state == hijacked && hjackcnt <= 11)
  {
    hjackcnt++;

    statusDebug("hijack counter", hjackcnt );
    pinRead();
    if ( brkst && !armst && ignst && !rdost && hjackcnt >= 10)
    {
      Serial.println("HIJACK DISABLED -- via code");
      state = unarmed;
    }
    else if (rmtst)
    {
      Serial.println("HIJACK DISABLED -- via remote");
      state = unarmed;
    }

    if (hjackcnt >= 10)
    {
      hjackcnt = 0;
    }
    delay(80);
    digitalWrite(siren, HIGH); // Siren = mute
    delay(80);
    digitalWrite(siren, LOW); // Siren = mute
    digitalWrite(hled, HIGH);  // HLED LED12 = OFF
    digitalWrite(ignco, HIGH); // IGNco relay =  & eng-disable LED13=OFF
    digitalWrite(fuelco, HIGH); //  fuelco  LED14 & eng-disable LED13 = OFF
    delay(500);
  }










}

void pinRead()
{
  brkst  =  digitalRead(brake);
  tiltst =  digitalRead(tilt);
  ldost  =  digitalRead(ldoor);
  bootst =  digitalRead(boot);
  ignst  =  digitalRead(ignsw);
  rmtst  =  digitalRead(rmtpb);
  armst =  digitalRead(armpb);
  bnetst =  digitalRead(bonnet);
  domest =  digitalRead(dome);
  rdost =  digitalRead(rdoor);
}

// check all door  and display which is open
void checkDoors()
{
  rdost =  digitalRead(rdoor);
  if (!rdost)
    Serial.println("R door is open");

  domest =  digitalRead(dome);
  if (!domest)
    Serial.println("Dome is on");

  bnetst =  digitalRead(bonnet);
  if (!domest)
    Serial.println("Bonnet is open");

  bootst =  digitalRead(boot);
  if (!bootst)
    Serial.println("Boot is open");

  ldost  =  digitalRead(ldoor);
  if (!ldost)
    Serial.println("L door is open");
}



void statusDebug(String names, int val)
{
  Serial.print(names);
  Serial.print(" : ");
  Serial.println(val);
  Serial.flush();
}


void serialPrintInstruct()
{

  Serial.println("            ");
  Serial.println(" Instructions:           ");
  Serial.println("            ");
  Serial.println(" press Armed or Remote Armed  to enable or disable system ");
  Serial.println("            ");
  Serial.println(" car will alarm for 20sec if armed and any of the door is opend");
  Serial.println("            ");
  Serial.println(" car will rearmed after 20 sec");
  Serial.println("            ");
  Serial.println(" car will be on hijack state indefinitely if armed, alarmed, and ignition is on ");
  Serial.println("            ");
  Serial.println(" disable hijacked state via remote Armed or pressing brake, ignition, armed ");
  Serial.println(" and right door must be opened for 20 Sec.");
}




