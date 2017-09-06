//Written by Ren Jiawei
#include"player.h"
Position goalspot (-1.25,0);
Position kickoff (-0.2, 0);
Position kickoff_strike_de (-0.7,0.4);
Position kickoff_guard_de (-0.7,-0.4);
/*
 * goto kick off position before game starts
 */
void player::gotokickoffPosition()
{
    //use moveToSide to take care of side change
    switch(this->robonumber)
    {
    case GOALIE_NUMBER:
    {
        moveToSide(goalspot);//kickoff position for goalie
        break;
    }
    case STRIKER_NUMBER:
    {
        if (info->isMeAttack){
                        moveToSide(kickoff);//offense kickoff position for striker
        }

        else {
            moveToSide(kickoff_strike_de);//defense kickoff position for striker
        }
        break;
    }
    case GUARD_NUMBER:
        {
            moveToSide(kickoff_guard_de);//kick off position for guard
        }
    }
}



