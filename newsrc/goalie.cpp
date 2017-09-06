//============================================================================
// Name        : soccer_client_1.cpp
// Author      : Minkai Hu
// Version     :
// Copyright   : Your copyright notice
// Description : Client for RTDB which controls team 1, Ansi-style
//============================================================================



#include "player.h"
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <bits/stdc++.h>
#include "kogmo_rtdb.hxx"
#include <math.h>
//Main contributor: Minkai 
//This version is used for the championship and is a skimmed down version of the simulation code
//tactic has been removed due to noise issues, the camera detection has too much of a delay which varies a lot, the wheels don't perform perfectly when telling them
//to use the same speed on both wheels.
const Position goalpatrol1_L(-1.3, 0); //left side has negative x value
Position goalpatrol1;
double degree_delay = 0; // delay for tactic on the real robots
const Position Goal_R(1.35, 0);  //starting position for tactic right
const Position Goal_L(-1.35, 0);//starting position for tactic right
Position OwnGoal;


void player::goalkeep() {
	Position goalpatrol1 = pChangeSide(goalpatrol1_L); //set side of the goalpatrol spot


													   // determine which side we play on

	if (info->mySide == 1) {
		OwnGoal.SetX(Goal_R.GetX());

	}
	if (info->mySide == 0) {
		OwnGoal.SetX(Goal_L.GetX());
	}


	// Ball prediction calculations start here

	Position Ball_spot_1 = info->ballPosition.GetPos(); // get position p1 of ball
	usleep(100000);
	Position Ball_spot_2 = info->ballPosition.GetPos(); // get position p2 of ball
	int factor = 0; // init and reset factor
					// Factor is a support variable used to determine the expected ball trajectory on the goalkeeper line
	factor = (OwnGoal.GetX() - Ball_spot_2.GetX()) / (Ball_spot_1.GetX() - Ball_spot_2.GetX());

	double goal_keeper_spot_to_go;

	//Determine the spot the goal keeper has to go to stop the ball from scoring
	goal_keeper_spot_to_go = Ball_spot_2.GetY() + factor*(Ball_spot_1.GetY() - Ball_spot_2.GetY());


	//Conditions on when the goalkeeper does what
	//Do nothing if the camera detects minor ball movement (to avoid the image processing noise)
	if (Ball_spot_1.DistanceTo(Ball_spot_2)< 0.02)
	{
		usleep(1000);
	}

	//if the ball is expected to hit the goal borders instead of scoring the goalkeeper doesn't move
	else if (Ball_spot_2.GetY() >0.3 || Ball_spot_2.GetY() < -0.3)
	{
		usleep(1000);
	}

	//Condition to move the goalkeeper only if the ball is traveling to the goal otherwise it moves when it rolls to the enemy goal
	//If the ball is unlikely to score a goal but might land close to the sides of the goal the goalkeeper moves to the corner of the penalty zone to block the direct path 
	//between the goal and ball
	//If the ball is expected to hit the goal, the goalkeeper goes to the expected path of the ball and blocks it
	else if (factor <= 0) {
		if (abs(this->GetY() - goal_keeper_spot_to_go)>0.05 && abs(goal_keeper_spot_to_go)<0.4)
		{

			if (info->ballPosition.GetY()<0.207 && info->ballPosition.GetY()>-0.207)
			{
				this->pGotoXY(OwnGoal.GetX(), goal_keeper_spot_to_go, 80, false);
			}

			if (info->ballPosition.GetY()>0.207)
			{
				this->pGotoXY(OwnGoal.GetX(), 0.2);
			}
			if (info->ballPosition.GetY()<-0.207)
			{
				this->pGotoXY(OwnGoal.GetX(), -0.2);
			}
		}
	}
	else {
		this->pGotoXY(OwnGoal.GetX(), info->ballPosition.GetY(), 80, true);
		//cout <<"ballYgoto"<<endl;
	}

	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	// Simulation part for the goalkeeper tactic; DOESN'T WORK WELL ON REAL ROBOTS; COMMENT IT OUT IF USING REAL ROBOTS

	while (abs(this->GetPos().GetX() - goalpatrol1.GetX()) >0.005 && abs(this->GetPos().GetY() - goalpatrol1.GetY()) >0.005) //initialize tactic by going to patrol spot
	{
		this->GotoPos(goalpatrol1);
		usleep(100000);
	}
	// Turn the robot till it faces up or down (perpendicular to the goalline)
	while ((this->GetPhi().Deg()>(95 + degree_delay) || (this->GetPhi().Deg()>(-85 + degree_delay) && this->GetPhi().Deg()<(85 + degree_delay)) || this->GetPhi().Deg()<(-95 + degree_delay)))
	{
		this->MoveMs(40, -40, 500, 0);
	}

	clock_t iteration_timer;
	iteration_timer = info->gametime; //Timer to reset the patrolling method, in case it moves out
	while (info->gametime - iteration_timer< 400000) //Do the patrolling for 4 seconds the check again on conditions if tactic or prediction is used
	{
		// When looking down, move downwards and upwards to cover most of the goal
		if (this->GetPhi().Deg()<140 && this->GetPhi().Deg()>40 && info->gametime - iteration_timer< 400000) //perform patrol if robot is facing up/down and timer is under 4 sec
		{
			this->MoveMs(80, 80, 50000, 0);           //looking down
			usleep(400000);
			this->MoveMs(-80, -80, 50000, 0);          //1000 is 0.1 sec
			usleep(400000);
		}

		// When looking up, move upwards and downwards to cover most of the goal

		if (this->GetPhi().Deg()>-140 && this->GetPhi().Deg()<-40 && info->gametime - iteration_timer< 400000)  //perform patrol if robot is facing up/down and timer is under 4 sec)
		{
			cout << info->gametime - iteration_timer << endl;
			this->MoveMs(80, 80, 50000, 0);
			usleep(400000);
			this->MoveMs(-80, -80, 50000, 0);
			usleep(400000);
		}
		this->GotoPos(goalpatrol1);
	}
	// END OF SIMULATION GOALKEEPER TACTIC
	//  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
}








//penalty goalkeeper mode
void player::penalty_goalkeep()
{
	//same tactic as in normal mode was used for the final championship, calculate the projected path of the ball
	Position Ball_spot_1 = info->ballPosition.GetPos();
	usleep(100000);
	Position Ball_spot_2 = info->ballPosition.GetPos();
	int factor = 0;

	factor = (-1.3 - Ball_spot_2.GetX()) / (Ball_spot_1.GetX() - Ball_spot_2.GetX());

	double goal_keeper_spot_to_go;

	goal_keeper_spot_to_go = Ball_spot_2.GetY() + factor*(Ball_spot_1.GetY() - Ball_spot_2.GetY());



	if (abs(this->GetY() - goal_keeper_spot_to_go)>0.05 && abs(goal_keeper_spot_to_go)<0.4) {

		if (info->ballPosition.GetY()<0.207 && info->ballPosition.GetY()>-0.207) {
			this->pGotoXY(OwnGoal.GetX(), goal_keeper_spot_to_go, 80, false);


		}

		if (info->ballPosition.GetY()>0.207) {
			this->pGotoXY(OwnGoal.GetX(), 0.2);
		}
		if (info->ballPosition.GetY()<-0.207) {
			this->pGotoXY(OwnGoal.GetX(), -0.2);
		}
	}
}


void player::goaliekick() {
	switch (this->robonumber)
	{
	case GOALIE_NUMBER: //case of goalkeeper kick: go behind the ball and kick it forwards for a set time (2 seconds), then return back to the penalty zone
	{
		this->pGotoXY(info->ballPosition.GetX() + 0.03, info->ballPosition.GetY(), 160, false);
		usleep(2000000);
		this->pGotoPos(OwnGoal);
		break;
	}
	default:
	{
		if (info->mySide == LEFT_SIDE)
			this->pGotoXY(this->GetX() + 0.2, this->GetY());
		if (info->mySide == RIGHT_SIDE)
			this->pGotoXY(this->GetX() - 0.2, this->GetY());
	}
	}

}


