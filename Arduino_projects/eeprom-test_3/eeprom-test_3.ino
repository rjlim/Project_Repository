// Reference for getting data via serial comms: http://arduino.stackexchange.com/questions/1013/how-do-i-split-an-incoming-string
// servo code format A/xx-
// where: A = Servo ID
//        / = seperator for servo ID and angle
//        xx = angle
//        - = seperator for next command

//mod on arduino linux mint
//  /usr/share/arduino


// command number allocation
/*
    1-12 - servo A to D
      a/b     where: a = servo number, b = angle of servo

    666/666  -  enable edit to eeprom
    999/999  -  disable edit to eeprom

    21/n  -  eeprom write servo address. where: n = servo address.
    22/x  -  eeprom write servo angles.  where: x = servo angle.

    arduino respond command.
    23/n  -  send value from/to javaFX for setting EEPROM edit mode.
    24/n  -  send to javaFX. the value of the incremented index of servo addres.
    25/n  -  send to javaFX. the value of the incremented index of servo angles.
    27/n  -  send to javaFX. size of the current sequence.
    28/n  -  send to javaFX. starting location of the servo angle. (NOT USED)
    29/n  -  send to javaFX. confirmation of size allocation.
    30/30 -  send to javaFX. sequence has succesfully saved.
    26/26 -  save commands to eeprom.

    14/14  - read eeprom.
    15/15  - read all contents of the array from 0 to 127.
    99/99 -  erase eeprom contents Note: 666/666 must be true to erase the contents.
*/

#include <Servo.h>
#include <EEPROM.h>

#define INPUT_SIZE 128                        // command stream size
#define svrDelay 50                           // wait do nothing for 10 ms
#define ServoAddr_Max_memAlloc_Size 128       //max array size for servo address.
#define ServoAngle_Max_memAlloc_Size 128      //max array size for servo angle.

// location for size allocation for the sequence
const int seqSize_memLoc = 0;       // memory location for total number of servo address
const int servoAngle_memLoc = 1;    // mempry location for total number of servo angles
const int memWrite_StartLoc = 3;    // initial location where memory should start writing

int servoAngle_memStartLoc;         // servo angle start location in EEPROM

// array that will contain the commands of the servo
int Servo_addr_array[ServoAddr_Max_memAlloc_Size];      // array for servo address. EEPROM will pass the value to this array
int Servo_angle_array[ServoAngle_Max_memAlloc_Size];    // array  for servo angle. EEPROM will pass the value to this array

// temporary variable for size allocation
int tmp_sizeAlloc = 0;

// index counter for the array
int memIndex_Servo_Addr = 0;
int memIndex_Servo_angle = 0;

// enable/disable eeprom edit
bool eepEnA = false, eepEnB = false;       // EEPROM write enable

void enableEEPROMwrite(int, int);


void setup()
{
  // initiallize Serial port BAUD rate
  Serial.begin(115200);
  // initialize starting memory location after the memory size allocation
  /*--------------------- attach all servos -------------------------*/


  tmp_sizeAlloc = EEPROM.read(seqSize_memLoc);                  //get command size
  servoAngle_memStartLoc = EEPROM.read(servoAngle_memLoc);      // get the starting location of servo angle
  Serial.print(F("-27/"));
  Serial.println(tmp_sizeAlloc);              // from send the size of the stored sequence from 0 to n.
  //Serial.print("-28/");
  //Serial.println(servoAngle_memStartLoc);   // send the starting location of servo address

  // total amount of data in the EEPROM
  /*
    int alpha = (2 * tmp_sizeAlloc) + 6 ;
    // get the data from EEPROM
    for (int in = 0 ; in <= alpha ; in++ )
    {

    int delta = EEPROM.read(in);
    Serial.print(in);
    Serial.print(" Stored Data: ");
    Serial.println(delta);
    }
  */
  // load the data to the servo address array
  for (int in = memWrite_StartLoc ; in <= tmp_sizeAlloc + memWrite_StartLoc ; in++ )
  {
    Servo_addr_array[in - memWrite_StartLoc] = EEPROM.read(in);
  }
  // load the data to the servo angle array
  for (int in = servoAngle_memStartLoc ; in <= (2 * tmp_sizeAlloc) + memWrite_StartLoc + 2 ; in++ )
  {
    Servo_angle_array[in - servoAngle_memStartLoc] = EEPROM.read(in);
  }
  showServoSequence(14, 14);
  /*----------------- initial leg position ---------------------------*/
}

/*=====================================================================*/
void loop()
{
  if (Serial.available() > 0)
  {
    // int cmdType;
    // int cmdCode;
    // Get next command from Serial (add 1 for final 0)
    char input[INPUT_SIZE + 1];
    byte size = Serial.readBytes(input, INPUT_SIZE);
    // Add the final 0 to end the C string
    input[size] = 0;

    // Read each command pair
    char* command = strtok(input, "-");
    while (command != 0)
    {
      // Split the command in 2 values
      char* separator = strchr(command, '/');
      if (separator != 0)
      {
        // Actually split the string in 2: replace ':' with 0
        *separator = 0;
        int cmdType = atoi(command);
        ++separator;
        int cmdCode = atoi(separator);
        /* get the address and command*/
        eepromWrite(cmdType, cmdCode);
        enableEEPROMwrite(cmdType, cmdCode);
        disableEEPROMwrite(cmdType, cmdCode);
        showServoSequence(cmdType, cmdCode);
        saveToEEPROM(cmdType, cmdCode);
        reSetup(cmdType, cmdType);
      }
      // Find the next command in input string
      command = strtok(0, "-");
    }
  }
}

/*=====================================================================*/
// change value of the eeprom
void eepromWrite(int memType, int memVal)
{
  // check if eeprom write mode is enable
  if ((eepEnA == true) && (eepEnA == true))
  {
    /*----------------------------------------------------------------------*/
    // store the size of the sequence (how many commands are given)
    if ((memType == 20) && (memVal <= ServoAddr_Max_memAlloc_Size))
    {
      // get the allocated size
      tmp_sizeAlloc = memVal;

      // reset the index counter
      memIndex_Servo_Addr = 0;
      memIndex_Servo_angle = 0;

      // display the value
      Serial.print(F("-29/"));
      Serial.println(tmp_sizeAlloc);
    }
    else if ((memType == 20) && (memVal >= ServoAddr_Max_memAlloc_Size))
    {
      // change to a string that send a error command
      Serial.println(F("-29/0"));
    }
    /*----------------------------------------------------------------------*/
    // check command if the value is for writing memory and the memory is not full
    if ((memType == 21) && ( memIndex_Servo_Addr <= tmp_sizeAlloc))
    {
      // get the servo address
      Servo_addr_array[memIndex_Servo_Addr] = memVal;
      // increment the counter
      memIndex_Servo_Addr++;

      // send the incremented counter
      // for the javaFX to know that the arduino is ready for the next value
      Serial.print(F("-24/"));
      Serial.println(memIndex_Servo_Addr);
    }
    /*--------------------------------------------------------------------------*/
    // check command if the value is for writing memory and the memory is not full
    else if ((memType == 22) && ( memIndex_Servo_angle <= tmp_sizeAlloc))
    {
      // get the servo address
      Servo_angle_array[memIndex_Servo_angle] = memVal;
      // increment the counter
      memIndex_Servo_angle++;

      // send the incremented counter
      // for the javaFX to know that the arduino is ready for the next value
      Serial.print(F("-25/"));
      Serial.println(memIndex_Servo_angle);

    }
    /*--------------------------------------------------------------------------*/
    else if ((memType == 22) && !( memIndex_Servo_angle < tmp_sizeAlloc))
    {
      // change to a string that send a memory full command
      Serial.println(F("-22/999"));
    }
    /*--------------------------------------------------------------------------*/
    else if ((memType == 21) && !( memIndex_Servo_Addr < tmp_sizeAlloc))
    {
      // change to a string that send a memory full command
      Serial.println(F("-21/999"));
    }
    /*--------------------------------------------------------------------------*/
    else if ((memType == 99) && (memVal == 99)  )
    {
      for (int in = 0 ; in <= 128 ; in++ )
      {
        EEPROM.write(in, 0);
      }
      Serial.println(F("-0/0"));
    }
  }
}

/*=====================================================================*/
void showServoSequence(int Show, int show)
{
  if ((Show == 14) && (show == 14) )
  {
    for (int i = 0 ; i <= tmp_sizeAlloc ; i++)
    {
      Serial.print("-");
      Serial.print(Servo_addr_array[i]);
      Serial.print("/");
      Serial.print(Servo_angle_array[i]);
    }
  }
  else if ((Show == 15) && (show == 15)  )
  {
    //Serial.println("Display all Content");
    for (int in = 0 ; in <= 128 ; in++ )
    {
      int memContent = EEPROM.read(in);

      Serial.print(F("-128/"));
      Serial.println(memContent);
    }
  }
}

/*=====================================================================*/
void saveToEEPROM(int saveA, int saveB)
{
  int tempServoAngle_memStartLoc = 0;

  if ((eepEnA == true) && (eepEnA == true))
  {
    if ((saveA == 26) && (saveB == 26))
    {
      EEPROM.write(seqSize_memLoc, tmp_sizeAlloc); // save the number of sequence
      //Serial.println("write to mem");
      /*=============================================================================*/
      for (int in = memWrite_StartLoc ; in <= tmp_sizeAlloc + memWrite_StartLoc ; in++ )
      {
        EEPROM.write(in, Servo_addr_array[in - memWrite_StartLoc]);
        //  Serial.println(Servo_addr_array[in - memWrite_StartLoc]);
        tempServoAngle_memStartLoc = in;
      }
      servoAngle_memStartLoc = tempServoAngle_memStartLoc + 2;
      EEPROM.write(servoAngle_memLoc, servoAngle_memStartLoc);
      /*=============================================================================*/
      for (int im = servoAngle_memStartLoc ; im <= ((2 * tmp_sizeAlloc) + memWrite_StartLoc + 2) ; im++ )
      {
        EEPROM.write(im, Servo_angle_array[im - servoAngle_memStartLoc]);
        //  Serial.println(Servo_angle_array[im - servoAngle_memStartLoc]);
      }
      Serial.println(F("-30/30"));
    }
  }
}

void reSetup(int enA, int enB)
{
  if ( (enA == 101) && (enB == 101) )
  {
    setup();
  }
}

/*=====================================================================*/
// enable eeprom write mode
void enableEEPROMwrite(int enA, int enB)
{
  if ( (enA == 666) && (enB == 666) )
  {
    eepEnA = true;
    eepEnB = true;
    Serial.println(F("-23/1"));
  }
}
/*=====================================================================*/
// disable eeprom write mode
void disableEEPROMwrite(int enA, int enB)
{
  if ( (enA == 999) && (enB == 999) )
  {
    eepEnA = false;
    eepEnB = false;
    Serial.println(F("-23/0"));
  }
}




