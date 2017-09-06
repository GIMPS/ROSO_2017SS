//============================================================================
// Name        : soccer_client_1.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Client for RTDB which controls team 1, Ansi-style
//============================================================================


#include <time.h>
#include <iostream>
#include <bits/stdc++.h>
#include "kogmo_rtdb.hxx"
#include "robo_control.h"
#include "referee.h"
#include "angle.h"
#include "share.h"

using namespace std;

int main(void) {



    /* Coordinate of the field


    (-1,38 0,89)--------------(-1,46 -0,89)
    |             |       |              |
    |             |_______|              |
    |                                    |
    |                                    |
    |                                    |
    |           O                        |
    |          -+-   <-- CR7             |
    |          77 o    <--ball           |
    |               _____                |
    |              /     \               |
    |             /       \              |
    |------------------------------------|
    |             \       /              |
    |              \_____/               |
    |                                    |
    |                                    |
    |                                    |
    |                                    |
    |                                    |
    |                                    |
    |              _______               |
    |             |       |              |
    |             |       |              |
    (1,42  0,875)-------------(1,47 -0,92)



      COMPUTER    COMPUTER    COMPUTER

      ******You may sitting here******

      COMPUTER    COMPUTER    COMPUTER

      ******You may sitting here******

      COMPUTER    COMPUTER    COMPUTER

      ******You may sitting here******


                                                     */







    //--------------------------------- Init --------------------------------------------------

    /** Use client number according to your lab_roso_stud account number!
     *
     *	This is necessary in order to assure that there are unique
     *	connections to the RTDB.
     *
     */
    const int client_nr = 8;

    /** Type in the rfcomm number of the robot you want to connect to.
     *  The numbers of the robots you are connected to can be found on the
     *  screen when you connected to them.
     *
     *  The red robots' number will be in the range of 3 to 5
     *  The blue robots' number will be in the range from 0 to 2
     *
     *  The robots are always connected to the lowest free rfcomm device.
     *  Therefore if you have two blue robots connected the will be
     *  connected to rfcomm number 0 and number 1...
     *
     */
        int rfcomm_nr0 = 3;
         int rfcomm_nr1 = 4;
          int rfcomm_nr2 = 5;

    /*

    // uncomment this if you want to select manually your robot
    // key input not available from QtCreator console, only from linux console.

    cout << "Specify rfcomm number of the robot you want to move: ";
    while (1) {
        cin >> rfcomm_nr;
        if (!((rfcomm_nr >= 0) && (rfcomm_nr <= 7))) {
            cout << "Please specify valid rfcomm number between 0 and 7"
                    << endl;
        } else {
            break;
        }
        }
    */

    try {

        /** Establish connection to the RTDB.
         *
         *  The connection to the RTDB is necessary in order to get access
         *  to the control and the status of the robots which are both stored
         *  in the RTDB.
         *
         *  In the RTDB there are also informations about the ball and the
         *  other robot positions.
         *
         */
        cout << endl << "Connecting to RTDB..." << endl;
        /** Create the client name with the unique client number*/
        string client_name = "pololu_client_";
        client_name.push_back((char) (client_nr + '0'));
        RTDBConn DBC(client_name.data(), 0.1, "");
        /** Create a new RoboControl object.
         *
         *  This is the basis for any communication with the robot.
         *
         *  We need to hand over the RTDB connection (DBC) and the rfcomm
         *  number of the robot we want to control.
         */
        RoboControl robo0(DBC, rfcomm_nr0);
        RoboControl robo1(DBC, rfcomm_nr1);
        RoboControl robo2(DBC, rfcomm_nr2);
        Referee ref(DBC ); ref.Init();
        /** Now let's print out some information about the robot... */
        uint8_t mac[6];
        robo0.GetMac(mac);
        robo1.GetMac(mac);
        robo2.GetMac(mac);
        cout << "Robo @ rfcomm" << rfcomm_nr0 << " with Mac: ";
        for (int j = 0; j < 5; j++)
            cout << hex << (int) mac[j] << ":";
        cout << hex << (int) mac[5] << endl;
         cout << "\t initial position: " << robo0.GetPos() << endl;
                //cout << "\t accuvoltage: " << dec << (int) robo.GetAccuVoltage()
                //		<< "mV" << endl;
        cout << "\t initial position: " << robo0.GetPos() << endl;
        cout << "\t initial rotation: " << robo0.GetPhi() << endl;

        /** Create a ball object
         *
         *  This ball abject gives you access to all information about the ball
         *  which is extracted from the cam.
         *
         */
        RawBall ball(DBC);
        /** lets print this information: */
        cout << "Ball informations:" << endl;
        cout << "\t initial position: " << ball.GetPos() << endl;
        /** Notice that the rotation here refers to the moving direction of the ball.
         *  Therefore if the ball does not move the rotation is not defined.
         */
         cout << "\t initial position: " << robo0.GetPos() << endl;
        cout << "\t initial direction: " << ball.GetPhi() << endl;
        cout << "\t initial velocity: " << ball.GetVelocity() << endl;

        //-------------------------------------- Ende Init ---------------------------------



        Position goalspot (-1.3,0);
        Position goalpatrol1 (-1.3,-0.4);
        Position goalpatrol2 (-1.3,0.4);
        Position random_spot (0,0);
        Position kickoff (-0.2, 0);
        Position kickoff_defender (-0.7,0);
        Position rest_point1(1.2,0.8);
        Position rest_point2(1.2,-0.8);

//class Position new_spot {
// private:
//    Position new_spot = goalkeeper_target (-1.3,ball.GetY()*0.7 );
//    return (Position new_spot);


//}
        while (1){
            Position Ball_spot_1 = ball.GetPos();
            usleep (100000);
            Position Ball_spot_2 = ball.GetPos();
            usleep (100000);
//            Position Ball_spot_3 = ball.GetPos();
//            Ball_spot_1.GetPos()

            int factor = 0;

            factor = (-1.3-Ball_spot_2.GetX())/ (Ball_spot_1.GetX()-Ball_spot_2.GetX())    ;

            double goal_keeper_spot_to_go;
            cout <<Ball_spot_1.DistanceTo(Ball_spot_2)<< endl;

            goal_keeper_spot_to_go = Ball_spot_2.GetY()+ factor*(Ball_spot_1.GetY()- Ball_spot_2.GetY()) ;


            if (Ball_spot_1.DistanceTo(Ball_spot_2)< 0.01){
                usleep (50000);
            }
            else if (Ball_spot_2.GetY() >0.5 || Ball_spot_2.GetY() < -0.5 ){


                usleep (50000);
            }
            else {
            Position Goal_to_go (-1.2, goal_keeper_spot_to_go);
             robo2.GotoPos(Goal_to_go);
            }
        }



        //int (threshold);
      //  threshold = 0.5;

        Position penalty_before (0,0);
        //Position penalty_start (ball.GetX()-0.2,ball.GetY());
cout<<"test";
        while(1)
        {
            cout<< ball.GetPhi()<<endl ;
            Position goalkeeper_target (-1.3,ball.GetY()*0.7 );
          Position*  goalkeeper_pointer = &goalkeeper_target;
          int i;
          i = 0;
 //   cout << goalkeeper_target;

   /*       if(ball.GetX() > 0) {

                robo0.GotoXY(goalspot.GetX(), ball.GetY()*0.3,160,true);
            usleep(100000);
            }

            else if(ball.GetX()> -0.4) {

                robo0.GotoXY(goalspot.GetX(), ball.GetY()*0.5,160,true);
            usleep(100000);
            }
            else {
cout << "robot position";
cout << robo0.GetPos()<< endl;
cout << "goalkeeper" << goalkeeper_target;*/

              while (ball.GetY()< -0.4){
              robo2.GotoXY(-1.2, -0.4, 160,true );{
               usleep(1000000);}}

              while (ball.GetY()>0.4){
                  robo2.GotoXY(-1.2, 0.4, 160,true );

               usleep(1000000);
              }

            if ((ball.GetY() < 0.4 &&ball.GetY() >-0.4)){
                robo2.GotoPos(*goalkeeper_pointer);


                while  (robo2.GetPos().DistanceTo(*goalkeeper_pointer)< 0.3 && i< 10){

                    i++;
                    usleep(50000);
                }
            }







}


} catch (DBError err) {
    cout << "Client died on Error: " << err.what() << endl;
}
cout << "end" << endl;
return 0;
}

