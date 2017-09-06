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
#include "math.h"
#include <share.h>
#define fconstant1 1
//#define fconstant2 0
#define fconstant2 50
#define fconstant3 1
#define fconstant4 0
//const double roboRadius=0.055;
const double roboRadius=0.044;
const double default_speed=160;
using namespace std;
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
                RoboControl robo(DBC, 0);
                RoboControl robo1(DBC, 1);
                RoboControl robo2(DBC, 2);
                RoboControl robo3(DBC, 3);
                RoboControl robo4(DBC, 4);
                RoboControl robo5(DBC, 5);

        /** Now let's print out some information about the robot... */
                //uint8_t mac[6];
                //robo.GetMac(mac);
                //cout << "Robo @ rfcomm" << rfcomm_nr << " with Mac: ";
                //for (int j = 0; j < 5; j++)
        //		cout << hex << (int) mac[j] << ":";
        //	cout << hex << (int) mac[5] << endl;

                //cout << "\t accuvoltage: " << dec << (int) robo.GetAccuVoltage()
                //		<< "mV" << endl;
        cout << "\t initial position: " << robo.GetPos() << endl;
        cout << "\t initial rotation: " << robo.GetPhi() << endl;

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
                //Position pos1(1.2,-0.4);
                #define pos1 ball.GetPos()
                Position pos2(1, -0.7);
                Position pos3(1, 0.7);
                Position pos4(-1, 0.7);

                Position aforce;
                Position rforce[6];
                Position wforce[4]={Position(0,0),Position(0,0),Position(0,0),Position(0,0)};

                //Need to remasure panalty zone demensions, NOT IMPLEMENTED
                Position penalty_zone[12]={Position(0.98,0.52),
                                            Position(0.98,-0.52),
                                            Position(0.98,0.17),
                                            Position(0.98,-0.17),
                                            Position(1.12,0.52),
                                            Position(1.12,-0.52),

                                           Position(-0.98,0.52),
                                           Position(-0.98,-0.52),
                                           Position(-0.98,0.17),
                                           Position(-0.98,-0.17),
                                           Position(-1.12,0.52),
                                           Position(-1.12,-0.52),


                                           };
                Position penalty_force[12];
                Position total_force;
                Position total_rforce;
                double dist[6];
                Position pos[6];
                Angle diffA(0);
                Angle dirA(0);


                while (1) {
                dist[1]=robo.GetPos().DistanceTo(robo1.GetPos());
                dist[2]=robo.GetPos().DistanceTo(robo2.GetPos());
                dist[3]=robo.GetPos().DistanceTo(robo3.GetPos());
                dist[4]=robo.GetPos().DistanceTo(robo4.GetPos());
                dist[5]=robo.GetPos().DistanceTo(robo5.GetPos());
                pos[1]=robo1.GetPos();
                pos[2]=robo2.GetPos();
                pos[3]=robo3.GetPos();
                pos[4]=robo4.GetPos();
                pos[5]=robo5.GetPos();
                for(int i=1;i<=5;i++)
                {
                    double force=fconstant1/pow(dist[i]-2*roboRadius,2);
                    Angle rangle=robo.GetPos().AngleOfLineToPos(pos[i].GetPos());
                    rforce[i].SetX(-force*cos(rangle));
                    rforce[i].SetY(-force*sin(rangle));
                }

                for(int i=0;i<12;i++)
                {
                    double force=fconstant4/pow(robo.GetPos().DistanceTo(penalty_zone[i].GetPos())-2*roboRadius,2);
                    Angle rangle=robo.GetPos().AngleOfLineToPos(penalty_zone[i].GetPos());
                    penalty_force[i].SetX(-force*cos(rangle));
                    penalty_force[i].SetY(-force*sin(rangle));
                }

                Angle goangle=robo.GetPos().AngleOfLineToPos(pos1);
                aforce.SetX(fconstant2/pow(robo.GetPos().DistanceTo(pos1),2)*cos(goangle));
                aforce.SetY(fconstant2/pow(robo.GetPos().DistanceTo(pos1),2)*sin(goangle));

                wforce[0].SetX(fconstant3/pow((robo.GetX()-(-1.37))-roboRadius,2));
                wforce[1].SetY(fconstant3/pow((robo.GetY()-(-0.88))-roboRadius,2));
                wforce[2].SetX(-fconstant3/pow((robo.GetX()-1.37)+roboRadius,2));
                wforce[3].SetY(-fconstant3/pow((robo.GetY()-0.88)+roboRadius,2));

                total_force.SetX(0);
                total_force.SetY(0);
                for(int i=1;i<=5;i++)
                    total_force+=rforce[i];
                for(int i=0;i<4;i++)
                    total_force+=wforce[i];

                for(int i=0;i<12;i++)
                    total_force+=penalty_force[i];
                total_force+=aforce;

                diffA=Position(0,0).AngleOfLineToPos(total_force)-robo.GetPhi();

                if(cos(diffA)>0)
                {
                    robo.MoveMs(160,160,1000);
                }
                else{
                    if(fabs(sin(diffA))<0.7)
                    {
                        robo.GotoXY(robo.GetX()+cos(robo.GetPhi()+0.5*diffA)*0.3,robo.GetY()+sin(robo.GetPhi()+0.5*diffA)*0.3);
                        usleep(1000);
                        cout<<"diffA is"<<diffA<<endl;
                    }
                    else{
                        robo.GotoXY(robo.GetX()+cos(Position(0,0).AngleOfLineToPos(total_force))*0.3,robo.GetY()+sin(Position(0,0).AngleOfLineToPos(total_force))*0.3);
                        usleep(1000);
                    }

                }

        }












/*
        while (1) {
                dist[1]=robo.GetPos().DistanceTo(robo1.GetPos());
                dist[2]=robo.GetPos().DistanceTo(robo2.GetPos());
                dist[3]=robo.GetPos().DistanceTo(robo3.GetPos());
                dist[4]=robo.GetPos().DistanceTo(robo4.GetPos());
                dist[5]=robo.GetPos().DistanceTo(robo5.GetPos());
                pos[1]=robo1.GetPos();
                pos[2]=robo2.GetPos();
                pos[3]=robo3.GetPos();
                pos[4]=robo4.GetPos();
                pos[5]=robo5.GetPos();
                for(int i=1;i<=5;i++)
                {
                    double force0=fconstant1/pow(dist[i]-2*roboRadius,2);
                    Angle rangle=robo.GetPos().AngleOfLineToPos(pos[i].GetPos());
                    rforce[i].SetX(-force0*cos(rangle));
                    rforce[i].SetY(-force0*sin(rangle));
                }
                Angle goangle=robo.GetPos().AngleOfLineToPos(pos1);
                aforce.SetX(fconstant2/pow(robo.GetPos().DistanceTo(pos1),2)*cos(goangle));
                aforce.SetY(fconstant2/pow(robo.GetPos().DistanceTo(pos1),2)*sin(goangle));
                wforce[0].SetX(fconstant3/pow((robo.GetX()-(-1.38))-roboRadius,2));
                wforce[1].SetY(fconstant3/pow((robo.GetY()-(-0.89))-roboRadius,2));
                wforce[2].SetX(-fconstant3/pow((robo.GetX()-1.38)+roboRadius,2));
                wforce[3].SetY(-fconstant3/pow((robo.GetY()-0.89)+roboRadius,2));

                total_force.SetX(0);
                total_force.SetY(0);
                for(int i=1;i<=5;i++)
                    total_force+=rforce[i];
                for(int i=0;i<4;i++)
                    total_force+=wforce[i];
                total_force+=aforce;

                diffA=Position(0,0).AngleOfLineToPos(total_force)-robo.GetPhi();
                fr=total_force.DistanceTo(Position(0,0))*cos(diffA);


                if(fabs(total_force.DistanceTo(Position(0,0)))>500){

                if(fabs(sin(diffA))<0.6&&cos(diffA)>0)
                {
                    usleep(1000);
                }
                else{
                newRadius=8/fr;
                if(newRadius>0)
                {
                    vr=160;
                    vl=-vr*(roboRadius-newRadius)/(roboRadius+newRadius);
                }
                else
                {
                    vl=160;
                    vr=-vl*(roboRadius+newRadius)/(roboRadius-newRadius);
                }

                cout<<"---------------------vl"<<vl<<endl;
                cout<<vr<<endl;
                cout<<total_force.GetPos()<<endl;
                cout<<newRadius<<endl;
                while(robo.IsMoving());
                robo.MoveMs(vl,vr,200);
                usleep(1000);
                }
                }
                else{
                    robo.GotoPos(pos1);
                    usleep(1000);
                }

        }

*/
    } catch (DBError err) {
        cout << "Client died on Error: " << err.what() << endl;
    }
    cout << "ende" << endl;
    return 0;
}

