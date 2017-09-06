
// by Bi Jianxin ga62zuf
#include"player.h"




int strat_para=0.5; // parameter used to adjust the relative location of robot_guard: 
//when ball is on our half, guard stay closer to our gate, so as to prevent the opponents to shoot
// when ball is on opponents' half, guard move forward, and relatively closer to the opponents' gate, so as to participate in attack

Position goalAimPoint_left(1.52,-0.195); //center of right gate, the position striker should shoot to, when playing left
Position goalAimPoint_right(-1.3,-0.2);  //center of left gate, the position striker should shoot to, when playing right

//defense position(x coordinate) of robot_guard, stay on the route between ball and our own gate, prevent the opponents shoot
double xdef_l(double a,double strat_para){

    //(a,b): position of ball
    // c : x  coordinate of robo1

    double xdef_l = a*strat_para- 1.4*(1-strat_para); // if strat_para ==0,5, guard stay at the middle between our gate and ball 

    if (xdef_l> -1.35)
        xdef_l = xdef_l;
    else
        xdef_l =-1.35;

// the defense baseline is x=-1.35, when play left
    return xdef_l;
}

// y coordinate for guard, calcualte by find the line pass through our gate and the ball
double ydef_l(double a, double b,double c){
    //(a,b): position of ball
    // c : x  coordinate of robo1

    double k,ydef_l;
    k= (b+0.2)/(a+1.35);
    ydef_l = k*(c + 1.35)-0.2;

    return ydef_l;


}

// x coordinate for striker, 0.15 behind the ball
double xatk_l(double a){

    //(a,b): position of ball

    double xatk_l;

    xatk_l = a-0.15;
    return xatk_l;

}

// y coordinate for striker, calculate by finding the line passing through the opponents gate center and the ball
double yatk_l(double a, double b, double c){
    //(a,b): position of ball
    // c : x coordinate of robo2, xatk_l

    double k,yatk_l;
    k= (b+0.195)/(a-1.52);
    yatk_l = k*(c-1.52)-0.195;
    if (yatk_l >1.1)
        yatk_l = 1.1;
    else if (yatk_l<-0.9)
        yatk_l = -0.9;
// if y coordinate >1.1 or <0.9, then stay at 1.1 or 0.9 respectively, otherwise, the striker will try to go out of the field
    return yatk_l;
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
// functions for playing on the right side, similar to left side, only change parameters
double xdef_r(double a,double strat_para){

    //(a,b): position of ball
    // c : x  coordinate of robo1

    double xdef_r = a*strat_para+ 1.52*(1-strat_para);

    if (xdef_r < 1.25)
        xdef_r = xdef_r;
    else
        xdef_r = 1.25;


    return xdef_r;
}

double ydef_r(double a, double b,double c){
    //(a,b): position of ball
    // c : x  coordinate of robo1

    double k,ydef_r;
    k= (b+0.2)/(a-1.52);
    ydef_r = k*(c-1.52)-0.2;

    return ydef_r;


}


double xatk_r(double a){

    //(a,b): position of ball

    double xatk_r;

    xatk_r = a+0.15;
    return xatk_r;

}

double yatk_r(double a, double b, double c){
    //(a,b): position of ball
    // c : x coordinate of robo2, xatk_r

    double k,yatk_r;
    k= (b+0.195)/(a+1.3);
    yatk_r = k*(c+1.3)-0.195;
    if (yatk_r >1.2)
        yatk_r = 1.2;
    else if (yatk_r<-1.0)
        yatk_r = -1.0;

    return yatk_r;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// normally, the robot can not move to a dynamic position without error, hence, the angle between opponents' gate, ball, striker is checked
// if the angle is too large, atkAngleCheck return false, striker will move to attack position once more
bool player::atkAngleCheck(){

    if (info->mySide == LEFT_SIDE) //when play left
    {
        Angle angleDifference=info->ballPosition.AngleOfLineToPos(goalAimPoint_left)-this->GetPos().AngleOfLineToPos(info->ballPosition);
        cout<<angleDifference.Deg()<<endl;

        double tolerance=36-(this->GetPos().DistanceTo(goalAimPoint_left))*12; // the angle limit is relative to the distance from ball to gate
                                                                               // as the ball get closer to the gate, the tolerance is larger

        if (info->ballPosition.GetY()<= this->GetY())
        {
            if(0.6*tolerance>angleDifference.Deg()&&angleDifference.Deg()>-1*tolerance)// right side limit and left side limit is different, based on their relative positions
                return true;                                                           // if ball is on the left of gate, striker is on the left of ball, then the right angle
            return false;                                                              // tolerance is strict, otherwise, the striker is easier to take the ball to the baseline
        }
        else if (info->ballPosition.GetY()> this->GetY())
        {
            if(1*tolerance>angleDifference.Deg()&&angleDifference.Deg()>-0.6*tolerance)
                return true;
            return false;
        }

    }

    else if (info->mySide == RIGHT_SIDE) // when play right
    {
        Angle angleDifference=info->ballPosition.AngleOfLineToPos(goalAimPoint_right)-this->GetPos().AngleOfLineToPos(info->ballPosition);
                double tolerance=36-(this->GetPos().DistanceTo(goalAimPoint_right))*12;

        cout<<angleDifference.Deg()<<","<<tolerance<<","<<this->GetPos().DistanceTo(info->ballPosition)<<endl;



        if (info->ballPosition.GetY()<= this->GetY())
        {
            if(1*tolerance>angleDifference.Deg()&&angleDifference.Deg()>-0.6*tolerance)
                return true;
            return false;
        }
        else if (info->ballPosition.GetY()> this->GetY())
        {
            if(0.6*tolerance>angleDifference.Deg()&&angleDifference.Deg()>-1*tolerance)
                return true;
            return false;
        }

    }
    return 0;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// when play left
//in defense mode (the mode is decided by the position of the ball(on our half or the opponents' half), the relative position of the ball and our robots)
//the ball is on our half
void player::defense_tac_left(role myrole)
{
    double strat_para = 0.4; // the ratio of (distance from our gate to guard) to (distance from guard to ball) is  4:6
    //cout<<strat_para<<endl;
    // set these positions, so that when the guard needs to move back, it will not collide with the ball 
    Position dst0 (info->ballPosition.GetX()-0.3,info->ballPosition.GetY()-0.3);
    Position dst1 (info->ballPosition.GetX()-0.3,info->ballPosition.GetY()-0.2);
    Position dst2 (info->ballPosition.GetX()-0.3,info->ballPosition.GetY()+0.2);
    //defense
    switch(myrole)
    {
        
        
        

    // the same robot may have different role in different situation
    // for example, in normal play mode, if the striker is in front of the ball, to attack faster, the guard and striker will exchange
    // the previous guard will become the striker and go to the collimation position while the previous striker will go back to defense
    // details can be found in "coach.cpp"
    
    // the role guard in defennse mode, keep going to defense position
    case solo_guard:
    {
        
       // cout<<"guard to defense"<<endl;
       
        if (this->GetX() <= info->ballPosition.GetX()){
            // stay on the defense position
            //cout << "guard is behind the ball"<<endl;
            this->pGotoXY(xdef_l(info->ballPosition.GetX(),strat_para),ydef_l(info->ballPosition.GetX(),info->ballPosition.GetY(),xdef_l(info->ballPosition.GetX(),strat_para)), 160, false);
        }
        // if guard is in front of the ball, first move to defined position and then go to defense position
        else {
            //cout << "guard is in front of the ball"<<endl;
              
            if (info->ballPosition.GetX()-0.3>=-1.3){

                if (this->GetY()< info->ballPosition.GetY()-0.1){
                        this->pGotoXY(dst1.GetX(),dst1.GetY(),160,false);
                }

                else if (this->GetY()> info->ballPosition.GetY()+0.1){
                        this->pGotoXY(dst2.GetX(),dst2.GetY(),160,false);

                }

                else{
                        this->pGotoXY(dst0.GetX(),dst0.GetY(),160,false);

                }
            }


            else {
                this->pGotoXY(-1.3,this->GetY());
            }

        }

        break;
     }
     
     
     
    // the role striker in defense mode, keep going to collimation position, wait for normal play mode
    case solo_striker:
    {
        // if the distance from ball to striker is smaller than 0.5, the striker will kick it out
        if(this->GetPos().DistanceTo(info->ballPosition.GetPos())<0.5&&this->GetX()<info->ballPosition.GetX())
        {
           this->pGotoXY(info->ballPosition.GetX(),info->ballPosition.GetY(),160,false);
            usleep(50000);
            cout<< " distance from ball to striker:"<<this->GetPos().DistanceTo(info->ballPosition)<<endl;
        }

    // if the striker is behind the ball, it will move to collimation position
        if (this->GetX() <= info->ballPosition.GetX()){
            // stay on the defense position
            //cout << "guard is behind the ball"<<endl;
            this->pGotoXY(xatk_l(info->ballPosition.GetX()),yatk_l(info->ballPosition.GetX(),info->ballPosition.GetY(),xatk_l(info->ballPosition.GetX())), 160, false);

        }
    // otherwise, move back until the ball is in front
        else {
            //cout << "guard is in front of the ball"<<endl;

            if (info->ballPosition.GetX()-0.3>=-1.3){

                if (this->GetY()< info->ballPosition.GetY()-0.1){
                        this->pGotoXY(dst1.GetX(),dst1.GetY(),160,false);

                }

                else if (this->GetY()> info->ballPosition.GetY()+0.1){
                        this->pGotoXY(dst2.GetX(),dst2.GetY(),160,false);

                }

                else{
                        this->pGotoXY(dst0.GetX(),dst0.GetY(),160,false);

                }
            }


            else {
                this->pGotoXY(-1.3,this->GetY());
            }

        }
            break;
        }
    default: break;
    }

}

//in backsave mode, it means both stirker and guard is in front of the ball, need to move back 
void player::backSave_tac_left(role myrole)
{
    double strat_para = 0.5;
    cout<<"tactic back save"<<endl<<endl;
    Position dst0 (info->ballPosition.GetX()-0.3,info->ballPosition.GetY()-0.3);
    Position dst1 (info->ballPosition.GetX()-0.3,info->ballPosition.GetY()-0.2);
    Position dst2 (info->ballPosition.GetX()-0.3,info->ballPosition.GetY()+0.2);
    //defense
    switch(myrole)
    {
    
    
    // in backsave mode, the robot which is closer to the ball will be selected as guard, 
    // the guard will move back and go to the defnese position 
    case solo_guard:
    { 
        cout<<"guard back save"<<endl;
        if (this->GetX() <= info->ballPosition.GetX()){
            // stay on the defense position
            //cout << "guard is behind the ball"<<endl;
            this->pGotoXY(xdef_l(info->ballPosition.GetX(),strat_para),ydef_l(info->ballPosition.GetX(),info->ballPosition.GetY(),xdef_l(info->ballPosition.GetX(),strat_para)), 160, false);
        }
        else {
            //cout << "guard is in front of the ball"<<endl;

            if (info->ballPosition.GetX()-0.3>=-1.3){

                if (this->GetY()< info->ballPosition.GetY()-0.1){
                        this->pGotoXY(dst1.GetX(),dst1.GetY(),160,false);

                }

                else if (this->GetY()> info->ballPosition.GetY()+0.1){
                        this->pGotoXY(dst2.GetX(),dst2.GetY(),160,false);

                }

                else{
                        this->pGotoXY(dst0.GetX(),dst0.GetY(),160,false);

                }
            }


            else {
                this->pGotoXY(-1.3,this->GetY());
            }

        }

        break;
     }
     // striker in backsave mode, move back
    case solo_striker:
    {
        cout<<"str back save"<<endl;
        if(info->ballPosition.GetX()>=-0.9)
        {
           this->pGotoXY(info->ballPosition.GetX()-0.2,this->GetY());
        }
        else
           this->pGotoXY(-1.15,this->GetY());
        }
    default: break;
    }

}
// the mode will change to normal play or defense once both robots have moved to the back of the ball

void player::normal_tac_left(role myrole){

    //normal mode. the ball is on opponents' half, both robots are at the back of ball
    //cout<<"normal play123"<<endl<<endl;
    double strat_para = 0.7; // the ratio of (distance from our gate to guard) to (distance from guard to ball) is  7:3, guard moves forward
    Position dst0 (info->ballPosition.GetX()-0.3,info->ballPosition.GetY()-0.3);
    Position dst1 (info->ballPosition.GetX()-0.3,info->ballPosition.GetY()-0.2);
    Position dst2 (info->ballPosition.GetX()-0.3,info->ballPosition.GetY()+0.2);

    switch(myrole)
    {
    case solo_guard:
    {
       if (this->GetX() <= info->ballPosition.GetX()){
            // stay on the defense position
            //cout << "guard is behind the ball"<<endl;
            this->pGotoXY(xdef_l(info->ballPosition.GetX(),strat_para),ydef_l(info->ballPosition.GetX(),info->ballPosition.GetY(),xdef_l(info->ballPosition.GetX(),strat_para)), 160, false);
        }
        else {
            //cout << "guard is in front of the ball"<<endl;


            if (this->GetY()< info->ballPosition.GetY()-0.1){
                    this->pGotoXY(dst1.GetX(),dst1.GetY(),160,false);
                    //usleep(5000);

            }

            else if (this->GetY()> info->ballPosition.GetY()+0.1){
                    this->pGotoXY(dst2.GetX(),dst2.GetY(),160,false);
                    //usleep(5000);

            }

            else{
                    this->pGotoXY(dst0.GetX(),dst0.GetY(),160,false);
                    //usleep(5000);

            }

        }

        break;
    }
    // striker in normal play (arttack) mode
    case solo_striker:
    {
            double destx=xatk_l(info->ballPosition.GetX());
            double desty=yatk_l(info->ballPosition.GetX(),info->ballPosition.GetY(),destx);
            Position dstAtk (destx,desty);

            //move to attack position
            // check atkAngle, if false, go to collimation position
            if(!atkAngleCheck()){
                this->GotoXY(destx,desty, 10, true);

                    //usleep(50000);
                cout<<"move to attack position"<<endl;
                break;
            }


            //else check whether dribbling or shoot, depedents on the distance from the gate to the ball
            if (info->ballPosition.DistanceTo(Position(1.52,-0.195))>0.9){
                if(this->GetPos().DistanceTo(info->ballPosition)>0.1)
                {
                    this->GotoXY(info->ballPosition.GetX(),info->ballPosition.GetY(), 120, true);
                    cout<<"kick"<<endl;
                      usleep(50000);
                    break;
                }
                else
                {
                    this->MoveMs(120,120,5000);
                    break;
                }

            }

            //shoot
            if (info->ballPosition.DistanceTo(Position(1.52,-0.195))<=0.9){
                if(this->GetPos().DistanceTo(info->ballPosition)>0.1)
                {
                this->GotoXY(info->ballPosition.GetX(),info->ballPosition.GetY(), 160, true);
                cout<<"shoooooooooooot"<<endl;
                        usleep(1000000);
                break;
                }
                else
                {
                    this->MoveMs(160,160,5000);
                       usleep(1000000);
                    break;
                }

            }


     }

    default: break;
    }


    }
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// when play on the right, same as on the left, only change field parameters
void player::defense_tac_right(role myrole)
{
    double strat_para = 0.4;
    //cout<<strat_para<<endl;

    Position dst0 (info->ballPosition.GetX()+0.3,info->ballPosition.GetY()-0.3);
    Position dst1 (info->ballPosition.GetX()+0.3,info->ballPosition.GetY()-0.2);
    Position dst2 (info->ballPosition.GetX()+0.3,info->ballPosition.GetY()+0.2);
    //defense
    switch(myrole)
    {

    case solo_guard:
    {
        cout<<"guard to defense"<<endl;
        if (this->GetX() >= info->ballPosition.GetX()){
            // stay on the defense position
            //cout << "guard is behind the ball"<<endl;
            this->pGotoXY(xdef_r(info->ballPosition.GetX(),strat_para),ydef_r(info->ballPosition.GetX(),info->ballPosition.GetY(),xdef_r(info->ballPosition.GetX(),strat_para)), 160, false);
        }
        else {
            //cout << "guard is in front of the ball"<<endl;

            if (info->ballPosition.GetX()+0.3<=1.25){

                if (this->GetY()< info->ballPosition.GetY()-0.1){
                        this->pGotoXY(dst1.GetX(),dst1.GetY(),160,false);

                }

                else if (this->GetY()> info->ballPosition.GetY()+0.1){
                        this->pGotoXY(dst2.GetX(),dst2.GetY(),160,false);

                }

                else{
                        this->pGotoXY(dst0.GetX(),dst0.GetY(),160,false);

                }
            }


            else {
                this->pGotoXY(1.25,this->GetY());
            }

        }

        break;
     }
    case solo_striker:
    {

        if(this->GetPos().DistanceTo(info->ballPosition.GetPos())<0.5&&this->GetX()>=info->ballPosition.GetX())
        {
            this->pGotoXY(info->ballPosition.GetX(),info->ballPosition.GetY(),160,false);
            cout<<"distance from ball to striker2 "<<this->GetPos().DistanceTo(info->ballPosition)<<endl;
        }


        if (this->GetX() >= info->ballPosition.GetX()){
            // stay on the defense position
            //cout << "guard is behind the ball"<<endl;
            this->pGotoXY(xatk_r(info->ballPosition.GetX()),yatk_r(info->ballPosition.GetX(),info->ballPosition.GetY(),xatk_r(info->ballPosition.GetX())), 160, false);

        }
        else {
            //cout << "guard is in front of the ball"<<endl;

            if (info->ballPosition.GetX()+0.3<=1.3){

                if (this->GetY()< info->ballPosition.GetY()-0.1){
                        this->pGotoXY(dst1.GetX(),dst1.GetY(),160,false);

                }

                else if (this->GetY()> info->ballPosition.GetY()+0.1){
                        this->pGotoXY(dst2.GetX(),dst2.GetY(),160,false);

                }

                else{
                        this->pGotoXY(dst0.GetX(),dst0.GetY(),160,false);

                }
            }


            else {
                this->pGotoXY(1.3,this->GetY());
            }

        }
            break;
        }
    default: break;
    }

}


void player::backSave_tac_right(role myrole)
{
    double strat_para = 0.5;
    cout<<"tactic back save"<<endl<<endl;
    Position dst0 (info->ballPosition.GetX()+0.3,info->ballPosition.GetY()-0.3);
    Position dst1 (info->ballPosition.GetX()+0.3,info->ballPosition.GetY()-0.2);
    Position dst2 (info->ballPosition.GetX()+0.3,info->ballPosition.GetY()+0.2);
    //defense
    switch(myrole)
    {

    case solo_guard:
    {
        cout<<"guard back save"<<endl;
        if (this->GetX() >= info->ballPosition.GetX()){
            // stay on the defense position
            //cout << "guard is behind the ball"<<endl;
            this->pGotoXY(xdef_r(info->ballPosition.GetX(),strat_para),ydef_r(info->ballPosition.GetX(),info->ballPosition.GetY(),xdef_r(info->ballPosition.GetX(),strat_para)), 160, false);
        }
        else {
            //cout << "guard is in front of the ball"<<endl;

            if (info->ballPosition.GetX()+0.3<=1.3){

                if (this->GetY()< info->ballPosition.GetY()-0.1){
                        this->pGotoXY(dst1.GetX(),dst1.GetY(),160,false);

                }

                else if (this->GetY()> info->ballPosition.GetY()+0.1){
                        this->pGotoXY(dst2.GetX(),dst2.GetY(),160,false);

                }

                else{
                        this->pGotoXY(dst0.GetX(),dst0.GetY(),160,false);

                }
            }


            else {
                this->pGotoXY(1.3,this->GetY());
            }

        }

        break;
     }
    case solo_striker:
    {
        cout<<"str back save"<<endl;
        if(info->ballPosition.GetX()<=1.1)
        {
           this->pGotoXY(info->ballPosition.GetX()+0.2,this->GetY());
        }
        else
           this->pGotoXY(1.3,this->GetY());
        }
    default: break;
    }

}


void player::normal_tac_right(role myrole){

    //normal mode
    //cout<<"normal play123"<<endl<<endl;
    double strat_para = 0.7;
    Position dst0 (info->ballPosition.GetX()+0.3,info->ballPosition.GetY()-0.3);
    Position dst1 (info->ballPosition.GetX()+0.3,info->ballPosition.GetY()-0.2);
    Position dst2 (info->ballPosition.GetX()+0.3,info->ballPosition.GetY()+0.2);

    switch(myrole)
    {
    case solo_guard:
    {
       if (this->GetX() >= info->ballPosition.GetX()){
            // stay on the defense position
            //cout << "guard is behind the ball"<<endl;
            this->pGotoXY(xdef_r(info->ballPosition.GetX(),strat_para),ydef_r(info->ballPosition.GetX(),info->ballPosition.GetY(),xdef_r(info->ballPosition.GetX(),strat_para)), 160, false);
        }
        else {
            //cout << "guard is in front of the ball"<<endl;


            if (this->GetY()< info->ballPosition.GetY()-0.1){
                    this->pGotoXY(dst1.GetX(),dst1.GetY(),160,false);
                    //usleep(5000);

            }

            else if (this->GetY()> info->ballPosition.GetY()+0.1){
                    this->pGotoXY(dst2.GetX(),dst2.GetY(),160,false);
                    //usleep(5000);

            }

            else{
                    this->pGotoXY(dst0.GetX(),dst0.GetY(),160,false);
                    //usleep(5000);

            }

        }

        break;
    }
    case solo_striker:
    {
            double destx=xatk_r(info->ballPosition.GetX());
            double desty=yatk_r(info->ballPosition.GetX(),info->ballPosition.GetY(),destx);
            Position dstAtk (destx,desty);

            //move to attack position

            if(!atkAngleCheck())
            {
                if (this->GetPos().DistanceTo(dstAtk)>0.1)
                {


                this->pGotoXY(destx,desty, 160, true);

                    usleep(50000);



                    cout<<"move to attack position"<<endl;
                    break;
                }
            }


            //kick the ball
            if (info->ballPosition.DistanceTo(Position(-1.3,-0.195))>0.9)
            {
                this->GotoXY(info->ballPosition.GetX(),info->ballPosition.GetY(), 120, true);
                usleep(50000);

                break;
            }

            //shoot
            if (info->ballPosition.DistanceTo(Position(-1.3,-0.195))<=0.9)
            {
                this->GotoXY(info->ballPosition.GetX(),info->ballPosition.GetY(), 160, true);
                cout<<"shoooooooooooot"<<endl;
                usleep(1000000);

                break;

            }



     }

    default: break;
    }


    }




