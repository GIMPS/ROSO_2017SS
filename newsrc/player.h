//Written by Ren Jiawei, Su Junxin
/*----------------------------------------------
 * This is a player class. Subclass of Robocontrol.
 *
 * We are going to create subclass like striker,
 * goalie adnd guard under this player class.
 *
 * Please add your function here.
 * But remember to do unit test on your function before
 * adding it into the class.
 *
 * Jiawei
 ----------------------------------------------*/


#ifndef player_H
#define player_H

#include <time.h>
#include <iostream>
#include "kogmo_rtdb.hxx"
#include "robo_control.h"
#include <math.h>
#include "info.h"
#include"infoPack.h"
#include "coach.h"



class player:public RoboControl{

    int robonumber; //index of the robot
    package *info;
protected:
    bool isAvoiding;
    double strat_para;

public:

    //Constructor. recieved_pos is the shared position information
    player(RTDBConn& DBC,const int allocated_number,package *info_receive);

    //An improved GotoXY function with COLLISION AVOIDANCE
    void GotoXY_ca(const double x, const double y, int32_t speed = default_speed);

    //An improved GotoPos function with COLLISION AVOIDANCE
    void GotoPos_ca(Position received_dest, int32_t speed=default_speed);
    void doTactic(coach *team_coach);
    void defense_tac_left(role myrole);
    void defense_tac_right(role myrole);
    void normal_tac_left(role myrole);
    void normal_tac_right(role myrole);
    void moveTo(Position p);
    void dopenaltyShoot_defense();
    void dopenaltyShoot_offense();
    void gotopenaltyPosition_defense();
    void gotopenaltyPosition_offense();
    void gotokickoffPosition_defense();
    void gotokickoffPosition_offense();
    void doReact(coach *team_coach);
    void gotokickoffPosition();
    void goalkeep();
    void GotoXYSide(const double x, const double y, int32_t speed, const bool precise);
    void GotoPosSide(const Position dest);
    void moveToSide(Position p);
    void backSave_tac_left(role myrole);
    void backSave_tac_right(role myrole);
    void moveTo_ca(Position p);
    void shoot(double x, double y);
    Position pChangeSide(Position p);
    void penalty_goalkeep();
    bool atkAngleCheck();
    void pGotoXY(const double x,const double y,const int speed=default_speed,const bool precise=true);
    void pGotoPos(Position p);
    bool isInPenalty(Position p);
    void Goto_Pre_Attack_XY(double x, double y);
    Position Pre_Attack_XY(double xa, double ya);
    void goalkeeperKick();
    void pGotoPos_side(Position p);
    void goaliekick();
};


#endif
