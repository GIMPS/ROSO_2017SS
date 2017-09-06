//Written by Ren Jiawei
#include "player.h"
#include <pthread.h>
#include"coach.h"
#include"referee.h"
using namespace std;

#define NUM_THREADS     5

double shootingRange=0.5;
double stealRange=0.5;

//Used to store all robot position
package info;

void *guard_thread(void *);
void *striker_thread(void *);
void *goalie_thread(void *);
void *info_updater(void *);
void *coach_runner(void *);

//Use ballpointer to access ball info
RawBall* ballpointer;
Referee* refpointer;
coach* coachpointer;

void debug() {
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
         static player robo[6]={player(DBC,0,&info),player(DBC,1,&info),player(DBC,2,&info),player(DBC,3,&info),player(DBC,4,&info),player(DBC,5,&info)};
         coach team_coach(&info);
         coachpointer=&team_coach;
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
        Referee ref(DBC);
        ref.Init();
        refpointer=&ref;
        ballpointer = &ball,
        /** lets print this information: */
        cout << "Ball informations:" << endl;
        cout << "\t initial position: " << ball.GetPos() << endl;
        /** Notice that the rotation here refers to the moving direction of the ball.
         *  Therefore if the ball does not move the rotation is not defined.
         */
        cout << "\t initial direction: " << ball.GetPhi() << endl;
        cout << "\t initial velocity: " << ball.GetVelocity() << endl;

        //-------------------------------------- Ende Init ---------------------------------


        //Create thread here
       pthread_create(&threads[0], NULL,striker_thread, (void *)&robo[STRIKER_NUMBER]);
       pthread_create(&threads[1], NULL,guard_thread, (void *)&robo[GUARD_NUMBER]);
       pthread_create(&threads[2], NULL,goalie_thread,(void *)&robo[GOALIE_NUMBER]);
       pthread_create(&threads[3], NULL,info_updater,(void *)&robo);// a thread update robot position
       pthread_create(&threads[4], NULL,coach_runner,NULL);

       for(int i=0;i<=3;i++)
       {
           pthread_join(threads[i],NULL);
       }


    } catch (DBError err) {
        cout << "Client died on Error: " << err.what() << endl;
    }
    cout << "ende" << endl;
}



void *striker_thread(void *robo_pointer)
{
    //player *striker=(player *)robo_pointer;

    while(1)
    {

        //striker->doReact(coachpointer);

    }
    pthread_exit(NULL);


}

void *guard_thread(void *robo_pointer)
{


    //player *guard=(player *)robo_pointer;

    while(1)
    {

        //guard->doReact(coachpointer);

    }

    pthread_exit(NULL);
}



void *goalie_thread(void *robo_pointer)
{
    player *goalie=(player *)robo_pointer;

    while(1)
    {
        goalie->moveTo_ca(Position(-1.2,-0.7));
        goalie->moveTo_ca(Position(1.2,0.7));
        //goalie->doReact(coachpointer);
    }

    pthread_exit(NULL);

}

void *info_updater(void *robo_pointer)
{
    while(1)
    {
        info.gametime=clock();
        info.ballPosition=ballpointer->GetPos();
        info.ballVelocity=ballpointer->GetVelocity();
        info.myscore=refpointer->GetRightSideGoals();
        info.oppscore=refpointer->GetLeftSideGoals();
        if(STRIKER_NUMBER>2)
        {
                    info.mySide=(eSide)(1-(int)refpointer->GetBlueSide());
        }
        else
        {
                    info.mySide=refpointer->GetBlueSide();
        }
        for(int i=0;i<6;i++)
        {

             info.pos[i]=((player *)(robo_pointer)+i)->GetPos();

        }
        for(int i=0;i<6;i++)
        {
            info.distToBall[i]=info.pos[i].DistanceTo(info.ballPosition);
        }
        int temp;
        temp=INF;
        for(int i=0;i<6;i++)
        {
            if(i==STRIKER_NUMBER||i==GUARD_NUMBER||i==GOALIE_NUMBER)
                if(temp>info.distToBall[i])
                {
                    temp=info.distToBall[i];
                    info.myNearest=i;
                }
        }

        temp=INF;
        for(int i=0;i<6;i++)
        {
            if(!(i==STRIKER_NUMBER||i==GUARD_NUMBER||i==GOALIE_NUMBER))
                if(temp>info.distToBall[i])
                {
                    temp=info.distToBall[i];
                    info.oppNearest=i;
                }
        }

        if(info.distToBall[info.myNearest]<info.distToBall[info.oppNearest])
        {
            info.allNearest=info.myNearest;
            info.isMeNearest=true;
        }
        else
        {
            info.allNearest=info.oppNearest;
            info.isMeNearest=false;
        }
        //cout<<(int) refpointer->GetSide()<<endl;
        //cout<<(int) refpointer->GetBlueSide()<<endl;
        info.isMeAttack=(refpointer->GetSide()==info.mySide);
        info.isMePenaltyShoot=(info.mySide==LEFT_SIDE);
        info.playMode=refpointer->GetPlayMode();
        //cout<<"play mode is "<<playMode<<endl;
        usleep(500);
    }

    pthread_exit(NULL);
}
void *coach_runner(void *)
{
    strategy game_strat=normal;
    while(1)
    {
        coachpointer->gameState=coachpointer->getState();
        //cout << "game state is "<< coachpointer->gameState<<endl;
        coachpointer->get_tactic();
        game_strat=coachpointer->get_strategy();
        switch(game_strat)
        {
        case normal:
        {
            shootingRange=0.5;
            stealRange=0.5;
            break;
        }
        case offense:
        {
            shootingRange=0.8;
            stealRange=0.8;
            break;
        }
        case defense:
        {
            shootingRange=0.3;
            stealRange=0.3;
            break;
        }
        }
        usleep(500);
    }
     pthread_exit(NULL);
}
