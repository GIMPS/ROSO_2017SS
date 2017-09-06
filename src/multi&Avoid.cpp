//============================================================================
// Name        : soccer_client_1.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Client for RTDB which controls team 1, Ansi-style
//============================================================================


#include <time.h>
#include <iostream>
#include <math.h>
#include "kogmo_rtdb.hxx"
#include "robo_control.h"
#include <pthread.h>

using namespace std;

#define NUM_THREADS     4
#define STRIKER_NUMBER  3
#define GUARD_NUMBER    4
#define GOALIE_NUMBER 5

#define fconstant1 1
#define fconstant2 5
#define fconstant3 1
const double roboRadius=0.043;

Position pos1(-1, -0.7);
Position pos2(1, 0.7);
Position pos3(1, 0.7);
Position pos4(-1, 0.7);

Position pos[8];


void *guard_thread(void *);
void *striker_thread(void *);
void *goalie_thread(void *);
void *info_monitor(void *);


int main(void) {
    //--------------------------------- Init --------------------------------------------------

    /** Use client number according to your account number!
     *
     *	This is necessary in order to assure that there are unique
     *	connections to the RTDB.
     *
     */
        const int client_nr = 1;

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
        //int rfcomm_nr = 1;

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
        pthread_t threads[NUM_THREADS];
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
         RoboControl robo[6]={RoboControl(DBC,0),RoboControl(DBC,1),RoboControl(DBC,2),RoboControl(DBC,3),RoboControl(DBC,4),RoboControl(DBC,5)};
        /** Now let's print out some information about the robot... */
                //uint8_t mac[6];
                //robo.GetMac(mac);
                //cout << "Robo @ rfcomm" << rfcomm_nr << " with Mac: ";
                //for (int j = 0; j < 5; j++)
        //		cout << hex << (int) mac[j] << ":";
        //	cout << hex << (int) mac[5] << endl;

                //cout << "\t accuvoltage: " << dec << (int) robo.GetAccuVoltage()
                //		<< "mV" << endl;

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
        cout << "\t initial direction: " << ball.GetPhi() << endl;
        cout << "\t initial velocity: " << ball.GetVelocity() << endl;

        //-------------------------------------- Ende Init ---------------------------------

        /** Define four positions which form a rectangle...
         *
         */



       pthread_create(&threads[1], NULL,guard_thread, (void *)&robo);
       //pthread_create(&threads[0], NULL,striker_thread, (void *)&robo);
       pthread_create(&threads[2], NULL,goalie_thread,(void *)&robo);
       for(int i=1;i<3;i++)
       {
           pthread_join(threads[i],NULL);
       }


    } catch (DBError err) {
        cout << "Client died on Error: " << err.what() << endl;
    }
    cout << "ende" << endl;
    return 0;
}



void *striker_thread(void *robo_pointer)
{
    RoboControl robo[6]={*((RoboControl *)robo_pointer),
                         *((RoboControl *)robo_pointer+1),
                         *((RoboControl *)robo_pointer+2),
                         *((RoboControl *)robo_pointer+3),
                         *((RoboControl *)robo_pointer+4),
                         *((RoboControl *)robo_pointer+5)};
    RoboControl striker=robo[STRIKER_NUMBER];
    RoboControl guard=robo[GUARD_NUMBER];
    RoboControl goalie=robo[GOALIE_NUMBER];
    do
    {

            striker.GotoXY(pos3.GetX(), pos3.GetY(), 160,true);
            usleep(1000);

    } while (striker.GetPos().DistanceTo(pos3)> 0.1);
    pthread_exit(NULL);


}

void *guard_thread(void *robo_pointer)
{


    RoboControl robo[6]={*((RoboControl *)robo_pointer),
                         *((RoboControl *)robo_pointer+1),
                         *((RoboControl *)robo_pointer+2),
                         *((RoboControl *)robo_pointer+3),
                         *((RoboControl *)robo_pointer+4),
                         *((RoboControl *)robo_pointer+5)};
    RoboControl striker=robo[STRIKER_NUMBER];
    RoboControl guard=robo[GUARD_NUMBER];
    RoboControl goalie=robo[GOALIE_NUMBER];
    while (1) {
                    cout << "Guard Moving to 2: " << pos2 << endl;
                    do
                    {

                        double relativeDir=atan2((robo[0].GetY()-guard.GetY()),(robo[0].GetX()-guard.GetX()));
                        cout << relativeDir<<endl;
                        cout<<"theta is";
                        cout << guard.GetPhi()<<endl;

                        if(guard.GetPos().DistanceTo(robo[0].GetPos())<0.2 && cos(relativeDir-guard.GetPhi().Get())>0){
                            while(guard.GetPos().DistanceTo(robo[0].GetPos())<0.2 && cos(relativeDir-guard.GetPhi().Get())>0){
                                double diff=relativeDir-guard.GetPhi().Get();
                                if(sin(diff)<0)
                                     {guard.MoveMs(250,125,1000);
                                        cout<<"Turn Right"<<endl;
                                        cout<<diff<<endl;

                                }
                                else
                                {guard.MoveMs(125,250,1000);
                                    cout<<"Turn Left"<<endl;
                                    cout<<diff<<endl;
                                    relativeDir=atan2((robo[0].GetY()-guard.GetY()),(robo[0].GetX()-guard.GetX()));
                                }

                            }

                        }

                        else
                        {
                            guard.GotoXY(pos2.GetX(), pos2.GetY(), 160,true);
                            usleep(1000);
                        }


                    } while (guard.GetPos().DistanceTo(pos2)> 0.1);

    }

    pthread_exit(NULL);
}



void *goalie_thread(void *robo_pointer)
{
    RoboControl robo[6]={*((RoboControl *)robo_pointer),
                         *((RoboControl *)robo_pointer+1),
                         *((RoboControl *)robo_pointer+2),
                         *((RoboControl *)robo_pointer+3),
                         *((RoboControl *)robo_pointer+4),
                         *((RoboControl *)robo_pointer+5)};
    RoboControl striker=robo[STRIKER_NUMBER];
    RoboControl guard=robo[GUARD_NUMBER];
    RoboControl goalie=robo[GOALIE_NUMBER];

    while (1) {
                    cout << "Goalie Moving to 1: " << pos1 << endl;
                    do
                    {

                        double relativeDir=atan2((robo[0].GetY()-goalie.GetY()),(robo[0].GetX()-goalie.GetX()));
                        cout << relativeDir<<endl;
                        cout<<"theta is";
                        cout << goalie.GetPhi()<<endl;

                        if(goalie.GetPos().DistanceTo(robo[0].GetPos())<0.2 && cos(relativeDir-goalie.GetPhi().Get())>0){
                            while(goalie.GetPos().DistanceTo(robo[0].GetPos())<0.2 && cos(relativeDir-goalie.GetPhi().Get())>0){
                                double diff=relativeDir-goalie.GetPhi().Get();
                                if(sin(diff)<0)
                                     {goalie.MoveMs(250,125,1000);
                                        cout<<"Turn Right"<<endl;
                                        cout<<diff<<endl;

                                }
                                else
                                {goalie.MoveMs(125,250,1000);
                                    cout<<"Turn Left"<<endl;
                                    cout<<diff<<endl;
                                    relativeDir=atan2((robo[0].GetY()-goalie.GetY()),(robo[0].GetX()-goalie.GetX()));
                                }

                            }

                        }

                        else
                        {
                            goalie.GotoXY(pos1.GetX(), pos1.GetY(), 160,true);
                            usleep(1000);
                        }


                    } while (goalie.GetPos().DistanceTo(pos1)> 0.1);

    }

    pthread_exit(NULL);

}

