//============================================================================
// Author      : Francesco Vigni
// Version     :
// Description : File for offensive and defensive penalty
//============================================================================

#include"player.h"
//definition of useful points
Position rest1 (0.3,-0.5);
Position rest2 (1,0.5);
Position rest3 (1,-0.3);
Position center (0,0);
Position penalty (-1.3,0);

//shifts x coordinate
double compute_x_attack(double a){
    //(a): x coordinate of the ball
    //shifts the x coordinate of a certain small value
    double x_attack;
    x_attack = a+0.08;
    return x_attack;

}

//calculate line btw ball and goal 
//return the y coordinate of the robot in order
//to be on the same line
double compute_y_attack(double a, double b, double c){
    //(a,b): position of ball
    // c : x coordinate of robo2

    double k,y_attack;
    k= b/(a+1.42);
    y_attack = k*(c+1.42);
    if (y_attack >0.8)
        y_attack = 0.8;
    else if (y_attack<-0.8)
        y_attack = -0.8;

    return y_attack;
}

//takes as inputs the coordinates of the ball and
//compute the target position for the robot
//in order have all the points on the same line (goal_ball_robot)
//
Position player::Pre_Attack_XY(double xa, double ya){
    Position before_penalty_xy;
    before_penalty_xy.SetX(compute_x_attack(xa));
    before_penalty_xy.SetY(compute_y_attack(xa,ya,compute_x_attack(xa)));
    return before_penalty_xy;
}

//added command GoToPos
void player::Goto_Pre_Attack_XY(double x, double y){
    // x,y position of the ball
    // robo go to the pre_attack position
    if(this->GetPos().DistanceTo(Pre_Attack_XY(x,y))>0.05){
        this->GotoPos(Pre_Attack_XY(x,y));
    }
}


void tt(){ //taketime 
    usleep(200000);
}

//define what each player should do in state BEFORE_PENALTY when its our turn to kick.
void player::gotopenaltyPosition_offense()
{
    tt();
    //cout << "we have to attack" << endl ;
    //cout << "Penalty_shoot Attack starts" << endl;
    switch(this->robonumber)
    {
    case GOALIE_NUMBER:
    {
            this->pGotoPos(rest1); //pGotoPos is a modified version of GotoPos that allows preemption of commands, so if a new command GotoPos is received the previous one will be interrupted, in order to increase precision in movements. can be found in ./player.cpp

        break;
    }
    case STRIKER_NUMBER:
    {
            this->pGotoPos(center); 
        break;
    }
    case GUARD_NUMBER:
    {
            this->pGotoPos(rest2);
        break;
    }
    }
}

//define movements in PENALTY state when we have to kick.
void player::dopenaltyShoot_offense()
{
    switch(this->robonumber)
    {
    case GOALIE_NUMBER:
    {
        this->AbortGotoXY();
        break;
    }
    case STRIKER_NUMBER:
    {
        double ball_x=info->ballPosition.GetX();
        double ball_y=info->ballPosition.GetY();
        // go to pre penalty position in ball-goal trajectory line
        Goto_Pre_Attack_XY(ball_x,ball_y);
        if(this->GetPos().DistanceTo(Pre_Attack_XY(ball_x,ball_y))<0.05)
            if(info->ballVelocity<0.01 && ball_x<1.3){
                //the x coordinate is scaled in order to have more speed at the penalty kick
                this->GotoXY(ball_x*1.1, ball_y ,200, false);
            }
        tt();
    }
    case GUARD_NUMBER:
    {
        this->AbortGotoXY();
        break;
    }
    }
}
//define what each player should do in state BEFORE_PENALTY when its our turn to defend.
void player::gotopenaltyPosition_defense()
{
    tt();
    switch(this->robonumber)
    {
    case GOALIE_NUMBER:
    {
            this->pGotoXY(penalty.GetX(),penalty.GetY(),100,true);
        break;
    }
    case STRIKER_NUMBER:
    {
            this->pGotoPos(rest3);
        break;
    }
    case GUARD_NUMBER:
    {
            this->pGotoPos(rest2);
        break;
    }
    }
}

//define movements in PENALTY state when we have to defend.
void player::dopenaltyShoot_defense()
{
    
    switch(this->robonumber)
    {
    case GOALIE_NUMBER:
    {
        this->penalty_goalkeep(); //function called from ./goalie.cpp
        break;
    }
    case STRIKER_NUMBER:
    {
        this->AbortGotoXY();
        break;
    }
    case GUARD_NUMBER:
    {
        this->AbortGotoXY();
        break;
    }
    }
}
