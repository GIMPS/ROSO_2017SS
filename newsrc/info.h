/*---------------------------------------
 *
 * Put invariable value or infomation in the file
 *
 *
 * Jiawei
 * ---------------------------------------------*/
 //Written by Ren Jiawei
#ifndef info_H
#define info_H
#include "robo_control.h"
#include <share.h>
#include<ctime>


//Coeficcient of collision avoidance algo. Keep fconstant4 as 0 before penalty avoidance added.
#define fconstant1 1    //repelling force by robot
#define fconstant2 50   // attracting force by destination point
#define fconstant3 1    //repelling force by walls
#define fconstant4 0    //repelling force by penalty zone


#define roboRadius 0.044
#define default_speed 80


//Change robot number here
#define STRIKER_NUMBER  0
#define GUARD_NUMBER    1
#define GOALIE_NUMBER   2
#define INF 10000000

enum strategy {
    normal,
    offense,
    defense,

};


enum tactic {
    idle_tac,
    defend_t,
    normal_t,
    backSave_t
};

enum role{
    stay,
    solo_striker,
    solo_guard
};

enum eGameState{
    idle,//0
    kickoffPosition_offense,//1
    kickoffPosition_defense,//2
    inGame,//3
    penaltyPosition_offense,//4
    penaltyPosition_defense,//5
    penaltyShoot_offense,//6
    penaltyShoot_defense//7
};
#endif
