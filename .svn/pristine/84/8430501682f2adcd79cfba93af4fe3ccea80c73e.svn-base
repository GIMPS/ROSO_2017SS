//============================================================================
// Name        : soccer_client_1.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Client for RTDB which controls team 1, Ansi-style
//============================================================================


#include <time.h>
#include <iostream>
#include "kogmo_rtdb.hxx"
#include "robo_control.h"
#include "referee.h"
#include "angle.h"

using namespace std;

int main(void) {
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
        int rfcomm_nr0 = 0;
         int rfcomm_nr1 = 1;
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

        /** Define four positions which form a rectangle...
         *
         */



        // ref.GetPlayMode are integers indicating status of the game pregame, setup, kickoff etc

       //0 == referee init
        // 1 == before kickoff
        // 2 == Kick_off
        //3 == before_penalty
        //4 == penalty
        // 5 == play_on

if (ref.GetPlayMode() == 0)
    cout << "referee init";
else if (ref.GetPlayMode() == 1)
    cout << "before kickoff";
else if (ref.GetPlayMode() == 2)
    cout << "kick off";
else if (ref.GetPlayMode() == 3)
    cout << " before penalty";
else if (ref.GetPlayMode() == 4)
    cout << "penalty";
else if (ref.GetPlayMode() == 5)
    cout << "play on"    ;

        Position goalspot (-1.2,0);
    Position random_spot (0,0);
    Position kickoff (-0.2, 0);
    Position kickoff_defender (-0.7,0);
    int signum_ball_y       ;
if (ball.GetY()<0) signum_ball_y = -1;
else signum_ball_y = +1;

    Position penalty_start (ball.GetX()-0.2,ball.GetY()*(0.6 ));
     cout << signum_ball_y<< "signum, ball koord"<< ball.GetY();
        //GOALKEEPER START
        class goalkeeper{
        public:
        };


            /** Sequentially move to the four different positions.
                         *  The while is excworkspace/soccerexampleited if the position is reached.
             */
            cout << "referee mode"<< ref.GetPlayMode()<< endl;
// ROBOT 0 GOALKEEPER
if (ref.GetPlayMode() == 2)
                {
    robo0.GotoXY(goalspot.GetX(), goalspot.GetY(),160,true);
    while (robo0.GetPos().DistanceTo(goalspot) > 0.10) usleep(10000);
            robo0.TurnAbs(0);


}
else   robo0.GotoXY(goalspot.GetX()-0.1, ball.GetY()*0.3,160, true);
//            while (robo.GetPos().DistanceTo(goalspot) > 0.10) usleep(10000);








//GOALKEEPER END





// ROBOT 1 STRIKER

if (ref.GetPlayMode() == 2){
                robo1.GotoXY(kickoff.GetX(), kickoff.GetY(),160,true);
while (robo1.GetPos().DistanceTo(kickoff) > 0.10) usleep(10000);
robo1.TurnAbs(0);

          }
//            while (robo.GetPos().DistanceTo(goalspot) > 0.10) usleep(10000);
else if (ref.GetPlayMode() == 1)
    robo1.GotoXY(kickoff_defender.GetX(), kickoff_defender.GetY(),160, true);
        //GOALKEEPER END



// ROBOT 2 DEFENDER


if (ref.GetPlayMode() == 2){

                robo2.GotoXY(kickoff_defender.GetX(), kickoff_defender.GetY(),160,true);
                while (robo2.GetPos().DistanceTo(kickoff_defender) > 0.10) usleep(10000);
                cout << robo2.GetPhi()<< endl<<endl;
                robo2.TurnAbs(0);

}
//            while (robo.GetPos().DistanceTo(goalspot) > 0.10) usleep(10000);
else if (ref.GetPlayMode() == 4)
    robo2.GotoXY(penalty_start.GetX(), penalty_start.GetY(),160, true);
while (robo2.GetPos().DistanceTo(penalty_start) > 0.001) usleep(10000);
    robo2.TurnAbs(0);
    while (robo2.GetPhi() != 90) usleep(10000);


        //GOALKEEPER END





    } catch (DBError err) {
        cout << "Client died on Error: " << err.what() << endl;
    }
    cout << "ende" << endl;
    return 0;
}

