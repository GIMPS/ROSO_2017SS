//============================================================================
// Name        : soccer_client_1.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Client for RTDB which controls team 1, Ansi-style
//============================================================================

#include <unistd.h>
#include <time.h>
#include <iostream>
#include <bits/stdc++.h>
#include "kogmo_rtdb.hxx"
#include "robo_control.h"
#include "referee.h"
#include <math.h>
//#include "angle.h"
//#include "share.h"

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
    const int client_nr = 11;

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
    int rfcomm_nr0 = 1;
    int rfcomm_nr1 = 0;
    int rfcomm_nr2 = 2;

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
        cout << "buggy"<<endl;
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
        Position goalpatrol1 (+1.35,0);
        Position goalpatrol11 (-1.3,-0.1);
        Position goalpatrol2 (-1.3,0.4);
        Position random_spot (0,0);
        Position kickoff (-0.2, 0);
        Position kickoff_defender (-0.7,0);
        Position rest_point1(1.2,0.8);
        Position rest_point2(1.2,-0.8);
        double degree_delay;
        degree_delay = 40;






        while (1){cout << "going to goal"<<endl;

            time_t endwait;
                time_t start = time(NULL);
                time_t seconds = 2; // end loop after this time has elapsed

                endwait = start + seconds;




        while (abs(robo0.GetPos().GetX() - goalpatrol1.GetX()) >0.005 && abs (robo0.GetPos().GetY()-goalpatrol1.GetY()) >0.005)
        {


            robo0.GotoPos(goalpatrol1);
           usleep(10000);


        }

        //float distance_x = robo0.GetX()-goalpatrol1.GetX();
        //float distance_y = robo0.GetY()-goalpatrol1.GetY();
        while (( robo0.GetPhi().Deg()>(95+ degree_delay) ||  (robo0.GetPhi().Deg()>(-85+ degree_delay) &&robo0.GetPhi().Deg()<(85+ degree_delay) ) || robo0.GetPhi().Deg()<(-95+ degree_delay)))
        {
            cout << "going to goal_2"<<endl;
            robo0.MoveMs(40,-40,500,0);
        }
        while(start< endwait)
        {
            start = time(NULL);

            //robo0.TurnAbs(90);

            // usleep(1000000);
            //goalline = phi of 90 to -90 upwards

            //cout << robo0.GetPhi().Deg()<< "radians" << robo0.GetPhi();

                 while (robo0.GetPhi().Deg()<140 && robo0.GetPhi().Deg()>40)
                {

                    robo0.MoveMs(80,80,50000,0);           //looking down
                    cout <<"sleeping_down"<<endl;
                    usleep(400000);
                    robo0.MoveMs(-80,-80,50000,0);          //1000 is 0.1 sec
                    cout <<"sleepingline2_down"<<endl;

                    usleep(500000);
                    cout << robo0.GetPhi().Deg() << "DEGREES AFTER RUN "<< endl;
                }


                while (robo0.GetPhi().Deg()>-140 && robo0.GetPhi().Deg()<-40)
                {
                                   //looking up
                    robo0.MoveMs(80,80,50000,0);
                    cout <<"sleepingline2"<<endl;

                    usleep(400000);
                    robo0.MoveMs(-80,-80,50000,0);
                    cout <<"sleeping"<<endl;

                    usleep(500000);
                    cout << robo0.GetPhi().Deg() << "DEGREES AFTER RUN ; I AM LINE 2"<< endl;


                }








            /*robo0.GotoXY(goalpatrol2.GetX(),goalpatrol2.GetY(),160,true);
            usleep(1000000);
            robo0.GotoXY(goalpatrol1.GetX(),-goalpatrol1.GetY(),-160,true);
            usleep(1000000)*/;
            //        cout<<sqrt(pow(distance_x,2)+pow(distance_y,2));
            //        while(sqrt(pow(distance_x,2)+pow(distance_y,2))>0.1){
            //            usleep (100000);
            //        }
            //        robo0.GotoXY(goalpatrol2.GetX(),goalpatrol2.GetY(),-160,true);
            //        usleep (100000);

        }



}
    } catch (DBError err) {
        cout << "Client died on Error: " << err.what() << endl;
    }
    cout << "end" << endl;
    return 0;
}
