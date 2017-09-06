#include "coach.h"
#include"player.h"
//written by Ren Jiawei, Su Junxin
coach::coach(package *info_receive)
{
    info=info_receive;
    for(int i=0;i<6;i++)
        robo_role[i]=stay;
    coach_tac=idle_tac;
    gameState=idle;
}

/*
 * coach decide for strategy
 * NOT FUTHERLY DEVELOPED
 */
strategy coach::get_strategy(){
    if(info->myscore<info->oppscore-1)
            return offense;
    if(info->myscore>info->oppscore+1)
        return defense;
    return normal;
}


/*
 * coach decide for tactic
 */
void coach::get_tactic(){

    check_basic_tac();
    sendRole();

}

/*
 * update the array -- play_role[robonumber]=role
 * for parsing roles to robots
 */
void coach::sendRole() {
    for(int i=0;i<6;i++)
        player_role[robo_role[i]]=i;
}

/*
 * Basic tactic
 * Control switching between tactics
 */
int coach::check_basic_tac(){
    //initializy robo role
    if(robo_role[STRIKER_NUMBER]!=solo_striker&&robo_role[STRIKER_NUMBER]!=solo_guard)//if have not done basic tac before
    {
        robo_role[STRIKER_NUMBER]=solo_striker;
        robo_role[GUARD_NUMBER]=solo_guard;
        sendRole();
    }
    //tactic
    if(info->isBallMyHalf)//defend when all is on own half
    {
        coach_tac=defend_t;
        return 1;
    }
    else
    {
        //count how many robots are behind the ball
        int count=0;
        if(isBehindBall(player_role[solo_striker]))
            count++;
        if(isBehindBall(player_role[solo_guard]))
            count++;
        if(coach_tac==backSave_t)
        {
            if(count==2)//if both behind, switch back to normal
                coach_tac=normal_t;
            else// else wait until both behind
                coach_tac=backSave_t;
        }
        else
        {
            switch(count)
            {
            case 0:
            {
                //no one behind the ball
                //need to go back to defend
                //assign the one nearest to own goal to be defender
                if(chooseGuard(player_role[solo_striker],player_role[solo_guard]))
                {
                    robo_role[player_role[solo_guard]]=solo_striker;
                    robo_role[player_role[solo_striker]]=solo_guard;
                }
                coach_tac=backSave_t;
                break;
            }
            case 1:
            {
                //one in front of the ball and one behind the ball
                //switch gaolkeeper and striker
                if(info->ballPosition.GetX()<=1.3)
                {
                    robo_role[player_role[solo_guard]]=solo_striker;
                    robo_role[player_role[solo_striker]]=solo_guard;
                }
                coach_tac=normal_t;
                break;
            }
            case 2:
            {
                //both behind, do normal tactic
                coach_tac=normal_t;
                break;
            }
            }
        }
        return 1;
    }
    return 0;
}

/*
 * check if a robot is behind the ball
 */
bool coach::isBehindBall(int i){
    if(info->mySide == LEFT_SIDE)
    {
        if(info->pos[i].GetX()>info->ballPosition.GetX())
            return 0;
        return 1;
    }

    else if (info->mySide == RIGHT_SIDE)
    {
        if(info->pos[i].GetX()<info->ballPosition.GetX())
            return 0;
        return 1;
    }
    return 0;
}

/*
 * choose guard when doing tactic back_to_save
 * check who is nearer to own goal
 */
bool coach::chooseGuard(int i, int j)
{

    if(info->mySide == LEFT_SIDE)
    {
        if(info->pos[i].GetX()<info->pos[j].GetX())
            return 1;
        return 0;
    }

    if(info->mySide == RIGHT_SIDE)
    {
        if(info->pos[i].GetX()>info->pos[j].GetX())
            return 1;
        return 0;
    }
    return 0;
}

/*
 * coach decide for game state
 */
eGameState coach::getState()
{
     switch(info->playMode)
     {
     case REFEREE_INIT:
     {
          return idle;
     }
     case BEFORE_KICK_OFF:
     {
         if(info->isMeAttack)
             return kickoffPosition_offense;
         else return kickoffPosition_defense;
     }
     case KICK_OFF:
     {
         if(info->isMeAttack)
             return inGame;
         else
             return kickoffPosition_defense;
     }
     case PLAY_ON:
     {
         return inGame;
     }
     case BEFORE_PENALTY:
     {
         if(info->isMePenaltyShoot)
             return penaltyPosition_offense;
         else return penaltyPosition_defense;
     }
     case PENALTY:
     {
         if(info->isMePenaltyShoot)
             return penaltyShoot_offense;
         else return penaltyShoot_defense;
     }
     case PAUSE:
     {
         return idle;
     }
     case TIME_OVER:
         return idle;

     default:
         return gameState;
     }
}

