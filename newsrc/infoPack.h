//Written by Ren Jiawei
#ifndef infoPack_H
#define infoPack_H

#include <time.h>
#include <iostream>
#include "kogmo_rtdb.hxx"
#include "robo_control.h"
#include <math.h>
#include "info.h"
class package
{
public:
    Position pos[6];
    double ballVelocity;
    Position ballPosition;
    int velocity[6];
    int myscore;
    int oppscore;
    int distToBall[6];
    int myNearest;
    int oppNearest;
    int allNearest;
    bool isBallMine;
    bool isBallMyHalf;
    bool isMeNearest;
    ePlayMode playMode;
    bool isMeAttack;
    bool isMePenaltyShoot;
    eSide mySide;
    clock_t gametime;
    double goal_keeper_spot_to_go;
    double factor;
};
#endif
