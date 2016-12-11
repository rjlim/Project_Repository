







/*



  preset movement of the hexapod


*/
/*

  1/40-
  2/160-
  3/120-

  4/90-
  5/150-
  6/145-

  7/155-
  8/160-
  9/145-

  10/30-
  11/10-
  12/40-

  13/90-
  14/5-
  15/45-

  16/150-
  17/20-
  18/40-




*/

// servo angle offset
// compensate for slight variation of servo
const int s1cl = 0, s2cl = 20, s3cl = 15,
          s4cl = 0, s5cl = -10, s6cl = -2,
          s7cl = 0, s8cl = 0, s9cl = 10,
          s10cl = 0, s11cl = 15, s12cl = -10,
          s13cl = 0, s14cl = 5, s15cl = -10,
          s16cl = 0, s17cl = 10, s18cl = 6;


// final default value of servo
int servoSetAngle[18] = {(90 + s1cl), (90 + s2cl), (90 + s3cl),
                         (90 + s4cl), (90 + s5cl), (90 + s6cl),
                         (90 + s7cl), (90 + s8cl), (90 + s9cl),
                         (90 + s10cl), (90 + s11cl), (90 + s12cl),
                         (90 + s13cl), (90 + s14cl), (90 + s15cl),
                         (90 + s16cl), (90 + s17cl), (90 + s18cl)
                        };


// send the final value of servo to android app to be used as reference
void GetDefaultStandValue()
{
  for (int ind = 0; ind < 18; ind++)
  {
    Serial2.print(ind + 1);
    Serial2.print("/");
    Serial2.print(servoSetAngle[ind]);
    Serial2.println("-");
  }
}

void standPosition()
{

  for (int snum = 0; snum < 18 ; snum++)
  {
    // servoControl_A.setPWM(snum, 0, map(90, 0, 180, SERVOMIN, SERVOMAX));
    // servoControl_B.setPWM(snum, 0, map(90, 0, 180, SERVOMIN, SERVOMAX));

    setServo(snum + 1, servoSetAngle[snum]);

    delay(5);
    Serial.print(snum + 1);
    Serial.print("/");
    Serial.print(servoSetAngle[snum]);
    Serial.println("-");

    //delay(10);
  }
}

void undeploy()
{
  int servoLegNum_A[] = {1, 4, 7, 10, 13, 16};

  int servoLegNum_B[] = {2, 5, 8, 11, 14, 17};
  int servoAngle_B[] = {155, 160, 165, 10, 20, 20};

  int servoLegNum_C[] = {3, 6, 9, 12, 15, 18};
  int servoAngle_C[] = {150, 130, 120, 50, 25, 40};

  for ( int i = 0 ; i < 6 ; i++)
  {
    setServo(servoLegNum_A[i], 30);
  }

  for ( int i = 0 ; i < 6 ; i++)
  {
    setServo(servoLegNum_B[i], servoAngle_B[i]);
  }

  for ( int i = 0 ; i < 6 ; i++)
  {
    setServo(servoLegNum_C[i], servoAngle_C[i]);
  }

}


void reverseTripodWalk(int seqSpeed)
{
  //lift leg for s5,s11,s17
  for (int serv_angle = 90 ; serv_angle < 120 ; serv_angle++ )
  {
    servoControl_B.setPWM(2, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S11
    servoControl_B.setPWM(9, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S17
    servoControl_A.setPWM(5, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                      // S5
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
  }

  // power sweep leg s13,s1,s7
  for (int serv_angle = 90 ; serv_angle > 60 ; serv_angle-- )
  {
    servoControl_B.setPWM(4, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));              // S13
    servoControl_A.setPWM(1, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX)); // S1
    servoControl_A.setPWM(8, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX)); // S7
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
    // after this loop angle of the following servo will be S16,S10 = 120 , S4 = 60
  }

  // leg on ground s5,s11,s17
  for (int serv_angle = 120 ; serv_angle > 90 ; serv_angle-- )
  {
    servoControl_B.setPWM(2, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S11
    servoControl_B.setPWM(9, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S17
    servoControl_A.setPWM(5, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                      // S5
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
  }

  // lift legs s2,s8,s14
  for (int serv_angle = 90 ; serv_angle < 120 ; serv_angle++ )
  {
    servoControl_A.setPWM(2, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S2
    servoControl_A.setPWM(9, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S8
    servoControl_B.setPWM(5, 0, map(90 - (serv_angle - 90 ) , 0, 180, SERVOMIN, SERVOMAX));   // S14
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
  }

  // return pos s1,s7,s13
  for (int serv_angle = 120 ; serv_angle > 90 ; serv_angle-- )
  {
    servoControl_B.setPWM(4, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));              // S13
    servoControl_A.setPWM(1, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX)); // S1
    servoControl_A.setPWM(8, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX)); // S7
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
    // after this loop angle of the following servo will be S16,S10 = 120 , S4 = 60
  }

  // power sweep for leg s16,s10,s4
  for (int serv_angle = 90 ; serv_angle >60 ; serv_angle-- )
  {
    servoControl_B.setPWM(8, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));              // S16
    servoControl_B.setPWM(1, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));              // S10
    servoControl_A.setPWM(4, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX)); // S4

    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
    // after this loop angle of the following servo will be S16,S10 = 120 , S4 = 60
  }

  //return pos  legs s2,s8,s14
  for (int serv_angle = 120 ; serv_angle > 90 ; serv_angle-- )
  {
    servoControl_A.setPWM(2, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S2
    servoControl_A.setPWM(9, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S8
    servoControl_B.setPWM(5, 0, map(90 - (serv_angle - 90 ) , 0, 180, SERVOMIN, SERVOMAX));   //14
    //  Serial.println(90 - (serv_angle - 90 ) );

    delay(seqSpeed);
  }

  //lift leg for s5,s11,s17
  for (int serv_angle = 90 ; serv_angle < 120 ; serv_angle++ )
  {
    servoControl_B.setPWM(2, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S11
    servoControl_B.setPWM(9, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S17
    servoControl_A.setPWM(5, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                      // S5
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
  }

  // return pos leg s16,s10,s4
  for (int serv_angle = 120 ; serv_angle > 90 ; serv_angle-- )
  {
    servoControl_B.setPWM(8, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));              // S16
    servoControl_B.setPWM(1, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));              // S10
    servoControl_A.setPWM(4, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX)); // S4

    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);

  }
  // leg on ground s5,s11,s17
  for (int serv_angle = 120 ; serv_angle > 90 ; serv_angle-- )
  {
    servoControl_B.setPWM(2, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S11
    servoControl_B.setPWM(9, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S17
    servoControl_A.setPWM(5, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                      // S5
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
  }


}

void InverseTripodWalk(int seqSpeed)
{
  //lift leg for s5,s11,s17
  for (int serv_angle = 90 ; serv_angle < 120 ; serv_angle++ )
  {
    servoControl_B.setPWM(2, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S11
    servoControl_B.setPWM(9, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S17
    servoControl_A.setPWM(5, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                      // S5
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
  }

  // power sweep leg s13,s1,s7
  for (int serv_angle = 90 ; serv_angle < 120 ; serv_angle++ )
  {
    servoControl_B.setPWM(4, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));              // S13
    servoControl_A.setPWM(1, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX)); // S1
    servoControl_A.setPWM(8, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX)); // S7
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
    // after this loop angle of the following servo will be S16,S10 = 120 , S4 = 60
  }

  // leg on ground s5,s11,s17
  for (int serv_angle = 120 ; serv_angle > 90 ; serv_angle-- )
  {
    servoControl_B.setPWM(2, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S11
    servoControl_B.setPWM(9, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S17
    servoControl_A.setPWM(5, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                      // S5
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
  }

  // lift legs s2,s8,s14
  for (int serv_angle = 90 ; serv_angle < 120 ; serv_angle++ )
  {
    servoControl_A.setPWM(2, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S2
    servoControl_A.setPWM(9, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S8
    servoControl_B.setPWM(5, 0, map(90 - (serv_angle - 90 ) , 0, 180, SERVOMIN, SERVOMAX));   // S14
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
  }

  // return pos s1,s7,s13
  for (int serv_angle = 120 ; serv_angle > 90 ; serv_angle-- )
  {
    servoControl_B.setPWM(4, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));              // S13
    servoControl_A.setPWM(1, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX)); // S1
    servoControl_A.setPWM(8, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX)); // S7
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
    // after this loop angle of the following servo will be S16,S10 = 120 , S4 = 60
  }

  // power sweep for leg s16,s10,s4
  for (int serv_angle = 90 ; serv_angle < 120 ; serv_angle++ )
  {
    servoControl_B.setPWM(8, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));              // S16
    servoControl_B.setPWM(1, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));              // S10
    servoControl_A.setPWM(4, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX)); // S4
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
    // after this loop angle of the following servo will be S16,S10 = 120 , S4 = 60
  }

  //return pos  legs s2,s8,s14
  for (int serv_angle = 120 ; serv_angle > 90 ; serv_angle-- )
  {
    servoControl_A.setPWM(2, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S2
    servoControl_A.setPWM(9, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S8
    servoControl_B.setPWM(5, 0, map(90 - (serv_angle - 90 ) , 0, 180, SERVOMIN, SERVOMAX));   //14
    //  Serial.println(90 - (serv_angle - 90 ) );

    delay(seqSpeed);
  }

  //lift leg for s5,s11,s17
  for (int serv_angle = 90 ; serv_angle < 120 ; serv_angle++ )
  {
    servoControl_B.setPWM(2, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S11
    servoControl_B.setPWM(9, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S17
    servoControl_A.setPWM(5, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                      // S5
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
  }

  // return pos leg s16,s10,s4
  for (int serv_angle = 120 ; serv_angle > 90 ; serv_angle-- )
  {
    servoControl_B.setPWM(8, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));              // S16
    servoControl_B.setPWM(1, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));              // S10
    servoControl_A.setPWM(4, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX)); // S4
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);

  }
  // leg on ground s5,s11,s17
  for (int serv_angle = 120 ; serv_angle > 90 ; serv_angle-- )
  {
    servoControl_B.setPWM(2, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S11
    servoControl_B.setPWM(9, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S17
    servoControl_A.setPWM(5, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                      // S5
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
  }



}





void tripodwalk(int seqSpeed)
{

  // int inverse_angle = 90;
  // int diff_val;

  // lift legs s2,s8,s14
  for (int serv_angle = 90 ; serv_angle < 120 ; serv_angle++ )
  {
    servoControl_A.setPWM(2, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S2
    servoControl_A.setPWM(9, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S8
    servoControl_B.setPWM(5, 0, map(90 - (serv_angle - 90 ) , 0, 180, SERVOMIN, SERVOMAX));   // S14
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
  }

  // power sweep for leg s16,s10,s4
  for (int serv_angle = 90 ; serv_angle < 120 ; serv_angle++ )
  {
    servoControl_B.setPWM(8, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));              // S16
    servoControl_B.setPWM(1, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));              // S10
    servoControl_A.setPWM(4, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX)); // S4
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
    // after this loop angle of the following servo will be S16,S10 = 120 , S4 = 60
  }

  //return pos  legs s2,s8,s14
  for (int serv_angle = 120 ; serv_angle > 90 ; serv_angle-- )
  {
    servoControl_A.setPWM(2, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S2
    servoControl_A.setPWM(9, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S8
    servoControl_B.setPWM(5, 0, map(90 - (serv_angle - 90 ) , 0, 180, SERVOMIN, SERVOMAX));   //14
    //  Serial.println(90 - (serv_angle - 90 ) );

    delay(seqSpeed);
  }

  //lift leg for s5,s11,s17
  for (int serv_angle = 90 ; serv_angle < 120 ; serv_angle++ )
  {
    servoControl_B.setPWM(2, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S11
    servoControl_B.setPWM(9, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S17
    servoControl_A.setPWM(5, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                      // S5
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
  }

  // return pos leg s16,s10,s4
  for (int serv_angle = 120 ; serv_angle > 90 ; serv_angle-- )
  {
    servoControl_B.setPWM(8, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));              // S16
    servoControl_B.setPWM(1, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));              // S10
    servoControl_A.setPWM(4, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX)); // S4
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);

  }
  // power sweep leg s13,s1,s7
  for (int serv_angle = 90 ; serv_angle < 120 ; serv_angle++ )
  {
    servoControl_B.setPWM(4, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));              // S13
    servoControl_A.setPWM(1, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX)); // S1
    servoControl_A.setPWM(8, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX)); // S7
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
    // after this loop angle of the following servo will be S16,S10 = 120 , S4 = 60
  }

  // leg on ground s5,s11,s17
  for (int serv_angle = 120 ; serv_angle > 90 ; serv_angle-- )
  {
    servoControl_B.setPWM(2, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S11
    servoControl_B.setPWM(9, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S17
    servoControl_A.setPWM(5, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                      // S5
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
  }
  // lift legs s2,s8,s14
  for (int serv_angle = 90 ; serv_angle < 120 ; serv_angle++ )
  {
    servoControl_A.setPWM(2, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S2
    servoControl_A.setPWM(9, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S8
    servoControl_B.setPWM(5, 0, map(90 - (serv_angle - 90 ) , 0, 180, SERVOMIN, SERVOMAX));   // S14
    // Serial.println(90 - (serv_angle - 90 ) );

    delay(seqSpeed);
  }

  // return pos s1,s7,s13
  for (int serv_angle = 120 ; serv_angle > 90 ; serv_angle-- )
  {
    servoControl_B.setPWM(4, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));              // S13
    servoControl_A.setPWM(1, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX)); // S1
    servoControl_A.setPWM(8, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX)); // S7
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
    // after this loop angle of the following servo will be S16,S10 = 120 , S4 = 60
  }
  //return pos  legs s2,s8,s14
  for (int serv_angle = 120 ; serv_angle > 90 ; serv_angle-- )
  {
    servoControl_A.setPWM(2, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S2
    servoControl_A.setPWM(9, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S8
    servoControl_B.setPWM(5, 0, map(90 - (serv_angle - 90 ) , 0, 180, SERVOMIN, SERVOMAX));   //14
    //  Serial.println(90 - (serv_angle - 90 ) );

    delay(seqSpeed);
  }
}




/*==============================================
  T U R N   L E F T   W A L K   S E Q U E N C E
  ===============================================*/
void turnRight(int seqSpeed)
{
  // lift legs s2,s8,s14
  for (int serv_angle = 90 ; serv_angle < 120 ; serv_angle++ )
  {
    servoControl_A.setPWM(2, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S2
    servoControl_A.setPWM(9, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S8
    servoControl_B.setPWM(5, 0, map(90 - (serv_angle - 90 ) , 0, 180, SERVOMIN, SERVOMAX));   // S14
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
  }

  // sweep servo to 60 s4, s16, s10
  for (int serv_angle = 90 ; serv_angle < 120 ; serv_angle++ )
  {
    servoControl_A.setPWM(4, 0, map(90 - (serv_angle - 90 ) , 0, 180, SERVOMIN, SERVOMAX)); // S4
    servoControl_B.setPWM(8, 0, map(90 - (serv_angle - 90 ) , 0, 180, SERVOMIN, SERVOMAX)); // S16
    servoControl_B.setPWM(1, 0, map(90 - (serv_angle - 90 ) , 0, 180, SERVOMIN, SERVOMAX)); //S10
    delay(seqSpeed);
  }
  //return pos legs s2,s8,s14
  for (int serv_angle = 120 ; serv_angle > 90 ; serv_angle-- )
  {
    servoControl_A.setPWM(2, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S2
    servoControl_A.setPWM(9, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S8
    servoControl_B.setPWM(5, 0, map(90 - (serv_angle - 90 ) , 0, 180, SERVOMIN, SERVOMAX));   //14
    //  Serial.println(90 - (serv_angle - 90 ) );

    delay(seqSpeed);
  }
  //lift leg for s5,s11,s17
  for (int serv_angle = 90 ; serv_angle < 120 ; serv_angle++ )
  {
    servoControl_B.setPWM(2, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S11
    servoControl_B.setPWM(9, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S17
    servoControl_A.setPWM(5, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                      // S5
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
  }
  // sweep servo back to 90 deg s4, s16, s10
  for (int serv_angle = 120 ; serv_angle > 90 ; serv_angle-- )
  {
    servoControl_A.setPWM(4, 0, map(90 - (serv_angle - 90 ) , 0, 180, SERVOMIN, SERVOMAX)); // S4
    servoControl_B.setPWM(8, 0, map(90 - (serv_angle - 90 ) , 0, 180, SERVOMIN, SERVOMAX)); // S16
    servoControl_B.setPWM(1, 0, map(90 - (serv_angle - 90 ) , 0, 180, SERVOMIN, SERVOMAX)); //S10
    delay(seqSpeed);
  }
  // power sweep to 120 deg S1, S7, S13
  for (int serv_angle = 90 ; serv_angle < 120 ; serv_angle++ )
  {
    servoControl_A.setPWM(1, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX)); // S1
    servoControl_A.setPWM(8, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX)); // S7
    servoControl_B.setPWM(4, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX)); // S13
    delay(seqSpeed);
  }
  // leg on ground s5,s11,s17
  for (int serv_angle = 120 ; serv_angle > 90 ; serv_angle-- )
  {
    servoControl_B.setPWM(2, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S11
    servoControl_B.setPWM(9, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S17
    servoControl_A.setPWM(5, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                      // S5
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
  }
  // lift legs s2,s8,s14
  for (int serv_angle = 90 ; serv_angle < 120 ; serv_angle++ )
  {
    servoControl_A.setPWM(2, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S2
    servoControl_A.setPWM(9, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S8
    servoControl_B.setPWM(5, 0, map(90 - (serv_angle - 90 ) , 0, 180, SERVOMIN, SERVOMAX));   // S14
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
  }
  // return sweep to 90 deg S1, S7, S13
  for (int serv_angle = 120 ; serv_angle > 90 ; serv_angle-- )
  {
    servoControl_A.setPWM(1, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX)); // S1
    servoControl_A.setPWM(8, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX)); // S7
    servoControl_B.setPWM(4, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX)); // S13
    delay(seqSpeed);
  }
  //return pos legs s2,s8,s14
  for (int serv_angle = 120 ; serv_angle > 90 ; serv_angle-- )
  {
    servoControl_A.setPWM(2, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S2
    servoControl_A.setPWM(9, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S8
    servoControl_B.setPWM(5, 0, map(90 - (serv_angle - 90 ) , 0, 180, SERVOMIN, SERVOMAX));   //14
    //  Serial.println(90 - (serv_angle - 90 ) );

    delay(seqSpeed);
  }
}

/*=========================================

  ===========================================*/
void turnLeft(int seqSpeed)
{
  // lift legs s2,s8,s14
  for (int serv_angle = 90 ; serv_angle < 120 ; serv_angle++ )
  {
    servoControl_A.setPWM(2, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S2
    servoControl_A.setPWM(9, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S8
    servoControl_B.setPWM(5, 0, map(90 - (serv_angle - 90 ) , 0, 180, SERVOMIN, SERVOMAX));   // S14
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
  }
  // sweep servo to 120 s4, s16, s10
  for (int serv_angle = 90 ; serv_angle < 120 ; serv_angle++ )
  {
    servoControl_A.setPWM(4, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX)); // S4
    servoControl_B.setPWM(8, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX)); // S16
    servoControl_B.setPWM(1, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX)); //S10
    delay(seqSpeed);
  }
  for (int serv_angle = 120 ; serv_angle > 90 ; serv_angle-- )
  {
    servoControl_A.setPWM(2, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S2
    servoControl_A.setPWM(9, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S8
    servoControl_B.setPWM(5, 0, map(90 - (serv_angle - 90 ) , 0, 180, SERVOMIN, SERVOMAX));   //14
    //  Serial.println(90 - (serv_angle - 90 ) );

    delay(seqSpeed);
  }
  //lift leg for s5,s11,s17
  for (int serv_angle = 90 ; serv_angle < 120 ; serv_angle++ )
  {
    servoControl_B.setPWM(2, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S11
    servoControl_B.setPWM(9, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S17
    servoControl_A.setPWM(5, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                      // S5
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
  }
  // return sweep servo to 90 s4, s16, s10
  for (int serv_angle = 120 ; serv_angle > 90 ; serv_angle-- )
  {
    servoControl_A.setPWM(4, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX)); // S4
    servoControl_B.setPWM(8, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX)); // S16
    servoControl_B.setPWM(1, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX)); //S10
    delay(seqSpeed);
  }
  // power sweep to 120 deg S1, S7, S13
  for (int serv_angle = 90 ; serv_angle < 120 ; serv_angle++ )
  {
    servoControl_A.setPWM(1, 0, map(serv_angle , 0, 180, SERVOMIN, SERVOMAX)); // S1
    servoControl_A.setPWM(8, 0, map(serv_angle , 0, 180, SERVOMIN, SERVOMAX)); // S7
    servoControl_B.setPWM(4, 0, map(serv_angle , 0, 180, SERVOMIN, SERVOMAX)); // S13
    delay(seqSpeed);
  }
  // leg on ground s5,s11,s17
  for (int serv_angle = 120 ; serv_angle > 90 ; serv_angle-- )
  {
    servoControl_B.setPWM(2, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S11
    servoControl_B.setPWM(9, 0, map(90 - (serv_angle - 90 ), 0, 180, SERVOMIN, SERVOMAX));    // S17
    servoControl_A.setPWM(5, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                      // S5
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
  }
  // lift legs s2,s8,s14
  for (int serv_angle = 90 ; serv_angle < 120 ; serv_angle++ )
  {
    servoControl_A.setPWM(2, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S2
    servoControl_A.setPWM(9, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S8
    servoControl_B.setPWM(5, 0, map(90 - (serv_angle - 90 ) , 0, 180, SERVOMIN, SERVOMAX));   // S14
    // Serial.println(90 - (serv_angle - 90 ) );
    delay(seqSpeed);
  }
  // return sweep to 90 deg S1, S7, S13
  for (int serv_angle = 120 ; serv_angle > 90 ; serv_angle-- )
  {
    servoControl_A.setPWM(1, 0, map(serv_angle , 0, 180, SERVOMIN, SERVOMAX)); // S1
    servoControl_A.setPWM(8, 0, map(serv_angle , 0, 180, SERVOMIN, SERVOMAX)); // S7
    servoControl_B.setPWM(4, 0, map(serv_angle , 0, 180, SERVOMIN, SERVOMAX)); // S13
    delay(seqSpeed);
  }
  //return pos legs s2,s8,s14
  for (int serv_angle = 120 ; serv_angle > 90 ; serv_angle-- )
  {
    servoControl_A.setPWM(2, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S2
    servoControl_A.setPWM(9, 0, map(serv_angle, 0, 180, SERVOMIN, SERVOMAX));                 // S8
    servoControl_B.setPWM(5, 0, map(90 - (serv_angle - 90 ) , 0, 180, SERVOMIN, SERVOMAX));   //14
    //  Serial.println(90 - (serv_angle - 90 ) );

    delay(seqSpeed);
  }
}

