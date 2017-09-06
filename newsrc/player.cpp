//Written by Ren Jiawei, Su Junxin
#include"player.h"
#include "basic_tac.cpp"
#include "kickoff_position.cpp"
#include "penalty.cpp"
#include "goalie.cpp"
#include"goalkeeperKick.cpp"

Position Prev;
Position Curr;

player::player (RTDBConn& DBC,const int allocated_number,package *info_receive): RoboControl(DBC,allocated_number)
{
        robonumber=allocated_number;
        info=info_receive;
        isAvoiding=false;
}
/*
 * side switching gotoxy
 */
void player::GotoXYSide(const double x, const double y, int32_t speed = 160, const bool precise = true)
{
    if(info->mySide== LEFT_SIDE)
        this->GotoXY(x*-1,y,speed,precise);
    else if(info->mySide == RIGHT_SIDE)
        this->GotoyXY(x,y,speed,precise);
}
/*
 * side switching gotopos
 */
void player::GotoPosSide(const Position dest)
{
    if(info->mySide== LEFT_SIDE)
        this->GotoXY(dest.GetX()*-1,dest.GetY());
    else if(info->mySide == RIGHT_SIDE)
        this->GotoXY(dest.GetX(),dest.GetY());
}
/*
 * Do coach's tactic
 */
void player:: doTactic(coach *team_coach)
{
     role myrole=team_coach->robo_role[this->robonumber];
     switch(team_coach->coach_tac)
     {
     case defend_t:
     {
         if (info->mySide == LEFT_SIDE)
             defense_tac_left(myrole);
         else if (info->mySide == RIGHT_SIDE)
             defense_tac_right(myrole);
         break;
     }
     case normal_t:
     {
         if (info->mySide == LEFT_SIDE)
             normal_tac_left(myrole);
         else if (info->mySide == RIGHT_SIDE)
             normal_tac_right(myrole);
         break;
     }
     case backSave_t:
     {
         if (info->mySide == LEFT_SIDE)
             backSave_tac_left(myrole);
         else if (info->mySide == RIGHT_SIDE)
             backSave_tac_right(myrole);
         break;
     }
     default:this->AbortGotoXY();
     }
}

/*
 * Do react to coach's state
 */
void player:: doReact(coach *team_coach)
{
     switch(team_coach->gameState)
     {
     case idle:
     {
         this->AbortGotoXY();
         break;
     }
     case kickoffPosition_offense:
     {
         this->gotokickoffPosition();
         break;
     }
     case kickoffPosition_defense:
     {
         this->gotokickoffPosition();
         break;
     }
     case inGame:
     {
         if(isInPenalty(info->ballPosition)&&info->isBallMyHalf)//goalkeeper kick
         {
             this->goaliekick();
             break;
         }
         if(this->robonumber==GOALIE_NUMBER)//goalkeeper
         {
             this->goalkeep();
             break;
         }
         this->doTactic(team_coach);//striker and defender
         break;
     }
     case penaltyPosition_offense:
     {
         this->gotopenaltyPosition_offense();
         break;
     }
     case penaltyPosition_defense:
     {
         this->gotopenaltyPosition_defense();
         break;
     }
     case penaltyShoot_defense:
     {
         this->dopenaltyShoot_defense();
         break;
     }
     case penaltyShoot_offense:
     {
         this->dopenaltyShoot_offense();
         break;
     }
     default:
         this->AbortGotoXY();
     }
}

/*
 * moveto function with side switching
 */
void player::moveToSide(Position p)
{
    int side=1;
    if(info->mySide==RIGHT_SIDE)
        side=-1;
    if(this->GetPos().DistanceTo(Position(p.GetX()*side,p.GetY()))>0.1)
    {
        this->GotoXY(p.GetX()*side,p.GetY(),80,true);
        usleep(2000);
    }
}

/*
 * improved goto method
 * usleep when moving correctly
 * send goto command when not moving or destination changes
 * mainly to save commands, keep sending goto may overload the server
 */
void player::pGotoXY(const double x, const double y, const int32_t speed, const bool precise)
{
    Curr.SetX(x);
    Curr.SetY(y);

    if(this->GetPos().DistanceTo(Position(x,y))>0.05 )
        if(this->GetModuleStatus()==STATUS_INT_GOTOXY_RUNNING && Curr.GetPos().DistanceTo(Prev)==0 )
        {
            usleep(1000);
        }
        else
            this->GotoXY(x,y,speed,precise);
        else
            this->AbortGotoXY();

    Prev.SetX(x);
    Prev.SetY(y);
}
/*
 *pGoto with side switching
 */
void player::pGotoPos_side(Position p)
{
    int side=1;
    if(info->mySide==RIGHT_SIDE)
        side=-1;
    pGotoXY(p.GetX()*side,p.GetY());
}
/*
 * pGoToPos based on pGotoXY
 */
void player::pGotoPos(Position p)
{
    pGotoXY(p.GetX(),p.GetY());
}


/*
 *modify x coord of a position to change side
 */
Position player::pChangeSide(Position p)
{
    double x=p.GetX();
    double y=p.GetY();
    if(info->mySide==RIGHT_SIDE)
        x*=-1;
    return Position(x,y);
}
//bool player::isOnField(){

//}
