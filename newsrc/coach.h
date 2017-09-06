//Written by Ren Jiawei Su Junxin
#ifndef COACH_H
#define COACH_H
#include"info.h"
#include"infoPack.h"
class coach
{
    package *info;
public:
    role robo_role[6];
    tactic coach_tac;
    eGameState gameState;
    int player_role[20];
    coach(package *info_receive);

    void get_tactic();
    void send(tactic, int role_num);
    strategy get_strategy();
    int check_basic_tac();
    eGameState getState();
    void sendRole();
private:
    bool isBehindBall(int i);
    bool chooseGuard(int i, int j);
};



#endif // COACH_H
