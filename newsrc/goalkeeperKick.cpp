//============================================================================
// Name        : soccer_client.cpp
// Author      : Chen Yuling
// Version     : Final Chanmpionship Version
// Copyright   : Your copyright notice
// Description : Client for RTDB which controls team 1, Ansi-style
//============================================================================

#include"player.h"

bool BallNoCollision (double a, double b, double c, double d, double x, double y)
{
    
    
    // a:= opprobot x, b:= opprobot y, the opposite robot position
    // c:= ball x    , d:= ball y, the ball position
    // x:= goalkeeper x, y:= goalkeeper y, the goalkeeper position
    
    double k, p;
    k= (d-y)/(c-x); // the slope factor of the quadratic equation function whcih is based on the ball and goalkeeper
    p= k*(a-x)+y; //  we set the point base on the same x axis as the opposite robot, and we can get the y axis p according to the equation above
    
    if ((p>b+0.12)or(p<b-0.12)) // if the set point y axis p is more than opposite robot y position by 0.12 or less than opposite robot y position by 0.12, no collision
    return true;
    else
    return false;
    
}



double adjust1(double a, double b, double c, double d, double x, double y){
    
    // a:= opprobot x, b:= opprobot y
    // c:= ball x    , d:= ball y
    // x:= goalkeeper x, y:= goalkeeper y
    
    double f,k;
    k= (b-d)/(a-c);
    f= k*(x-a)+b+0.15;  // displacement of goalkeeper along y axis by 0.15
    
    return f;
    
}

double adjust2(double a, double b, double c, double d, double x, double y){
    
    // a:= opprobot x, b:= opprobot y
    // c:= ball x    , d:= ball y
    // x:= goalkeeper x, y:= goalkeeper y
    
    double f,k;
    k= (b-d)/(a-c);
    f= k*(x-a)+b+0.25;  // displacement of goalkeeper along y axis by 0.25
    
    return f;
    
}

double adjust3(double a, double b, double c, double d, double x, double y){
    
    // a:= opprobot x, b:= opprobot y
    // c:= ball x    , d:= ball y
    // x:= goalkeeper x, y:= goalkeeper y
    
    double f,k;
    k= (b-d)/(a-c);
    f=k*(x-a)+b+0.35;  // displacemebt of goalkeeper along y axis by 0.35
    
    return f;
    
}

void player::goalkeeperKick(){
    
    
    
    /** Define four positions which form a rectangle...
    *
    */
    
    Position pos0(1.27, this->GetY());
    Position pos1(1.27, info->ballPosition.GetY());
    Position pos2(this->GetX(), this->GetY()-0.3);
    Position pos3(1.27, this->GetY()-0.3);
    Position pos4(1.27, adjust1(info->pos[0].GetX(),info->pos[0].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY())); //1st new kick postion
    Position pos5(1.27, adjust2(info->pos[0].GetX(),info->pos[0].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY())); //2nd new kick postion
    Position pos6(1.27, adjust3(info->pos[0].GetX(),info->pos[0].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY())); //3rd new kick postion
    Position poskick;
    Position posmid;
    
    
    
    
    
        /** Sequentially move to the four different positions.
        *  The while is excited if the position is reached.
        */
        cout <<"working.."<<endl;
        if (((info->pos[0].GetY()< info->pos[1].GetY())&&(info->pos[1].GetY()< info->pos[2].GetY())) or ((info->pos[2].GetY()< info->pos[1].GetY())&&(info->pos[1].GetY()< info->pos[0].GetY()))){
            posmid=info->pos[1].GetPos();
            cout << "midrobot is robo1 " << endl << endl;
        }
        if (((info->pos[1].GetY()< info->pos[0].GetY())&&(info->pos[0].GetY()< info->pos[2].GetY())) or ((info->pos[2].GetY()< info->pos[0].GetY())&&(info->pos[0].GetY()< info->pos[1].GetY()))){
            posmid=info->pos[0].GetPos();
            cout << "midrobot is robo0 " << endl << endl;
        }
        if (((info->pos[0].GetY()< info->pos[2].GetY())&&(info->pos[2].GetY()< info->pos[1].GetY())) or ((info->pos[1].GetY()< info->pos[2].GetY())&&(info->pos[2].GetY()< info->pos[0].GetY()))){
            posmid=info->pos[2].GetPos();
            cout << "midrobot is robo2 " << endl << endl;
        }
    
    // the middle robot according to the y axis, and the displacement of the goalkeeper is based on the middoe robot
        
        
        
        if( (info->ballPosition.GetY()> this->GetY()+0.1) or (info->ballPosition.GetY()< this->GetY()-0.1) )//if the ball is not near the robot according to the y axis
        
        {
            
            
            cout << "Moving to pos0:" << pos0 << endl << endl;
            this->GotoXY(pos0.GetX(), pos0.GetY(), 160, false);
            while (this->GetPos().DistanceTo(pos0.GetPos()) > 0.05) usleep(1000);
            //go back to the bottom line first
            
            cout << "Moving to pos1: " << pos1<< endl << endl;
            this->GotoXY(pos1.GetX(),pos1.GetY(), 160, false);
            while (this->GetPos().DistanceTo(pos1.GetPos()) > 0.05) usleep(1000);
            //go the same y axis position as the ball
            
            cout << "decide to adjust: " << pos1<< endl << endl;
            
            // collision check
            if (BallNoCollision(info->pos[0].GetX(),info->pos[0].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY()) and
            BallNoCollision(info->pos[1].GetX(),info->pos[1].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY()) and
            BallNoCollision(info->pos[2].GetX(),info->pos[2].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY()) )
            
            poskick= pos1;
            
            else if (
                BallNoCollision(info->pos[0].GetX(),info->pos[0].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), adjust1(posmid.GetX(),posmid.GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY())) and
                BallNoCollision(info->pos[1].GetX(),info->pos[1].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), adjust1(posmid.GetX(),posmid.GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY())) and
                BallNoCollision(info->pos[2].GetX(),info->pos[2].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), adjust1(posmid.GetX(),posmid.GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY())) ){
                    
                    poskick= pos4;
                    cout << "Moving to pos4: " << pos4<< endl << endl;
            }
                
            else if (BallNoCollision(info->pos[0].GetX(),info->pos[0].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), adjust2(posmid.GetX(),posmid.GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY())) and
                BallNoCollision(info->pos[1].GetX(),info->pos[1].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), adjust2(posmid.GetX(),posmid.GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY())) and
                BallNoCollision(info->pos[2].GetX(),info->pos[2].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), adjust2(posmid.GetX(),posmid.GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY())) ){
                    
                    poskick= pos5;
                    cout << "Moving to pos5: " << pos4<< endl << endl;
            }
                
                
            else if (BallNoCollision(info->pos[0].GetX(),info->pos[0].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), adjust3(posmid.GetX(),posmid.GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY())) and
                BallNoCollision(info->pos[1].GetX(),info->pos[1].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), adjust3(posmid.GetX(),posmid.GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY())) and
                BallNoCollision(info->pos[2].GetX(),info->pos[2].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), adjust3(posmid.GetX(),posmid.GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY())) ){
                    
                    poskick= pos6;
                    cout << "Moving to pos6: " << pos4<< endl << endl;
            }
            
            
            cout << "go to the kick position " << poskick<< endl << endl;
            this->GotoXY(poskick.GetX(),poskick.GetY(), 160, false);
            while (this->GetPos().DistanceTo(poskick.GetPos()) > 0.01) usleep(1000);
            cout << "kick"  << endl << endl;
            this->GotoXY(info->ballPosition.GetX(),info->ballPosition.GetY(), 250, false);
            while (this->GetPos().DistanceTo(info->ballPosition.GetPos()) > 0.01) usleep(1000);
        }
            
            
            
            
            
            
            
            
        else // if the ball is near the robot according to the y axis
        
        {
            
            this->GotoXY(pos2.GetX(), pos2.GetY(), 160, false);
            while (this->GetPos().DistanceTo(pos2.GetPos()) > 0.01) usleep(1000);
            //move on the y axis first
            
            this->GotoXY(pos3.GetX(), pos3.GetY(), 160, false);
            while (this->GetPos().DistanceTo(pos3.GetPos()) > 0.01) usleep(1000);
            //move back to the bottom line
            
            this->GotoXY(pos0.GetX(), pos0.GetY(), 160, false);
            while (this->GetPos().DistanceTo(pos0.GetPos()) > 0.05) usleep(1000);
            
            
            this->GotoXY(pos1.GetX(),pos1.GetY(), 160, false);
            while (this->GetPos().DistanceTo(pos1.GetPos()) > 0.05) usleep(1000);
            
            // collision check
            if (BallNoCollision(info->pos[0].GetX(),info->pos[0].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY()) and
            BallNoCollision(info->pos[1].GetX(),info->pos[1].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY()) and
            BallNoCollision(info->pos[2].GetX(),info->pos[2].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY()) )
            
            poskick= pos1;
            else if (
                BallNoCollision(info->pos[0].GetX(),info->pos[0].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), adjust1(posmid.GetX(),posmid.GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY())) and
                BallNoCollision(info->pos[1].GetX(),info->pos[1].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), adjust1(posmid.GetX(),posmid.GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY())) and
                BallNoCollision(info->pos[2].GetX(),info->pos[2].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), adjust1(posmid.GetX(),posmid.GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY())) ){
                    
                    poskick= pos4;
                    cout << "Moving to pos4: " << pos4<< endl << endl;
            }
                
            else if (BallNoCollision(info->pos[0].GetX(),info->pos[0].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), adjust2(posmid.GetX(),posmid.GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY())) and
                BallNoCollision(info->pos[1].GetX(),info->pos[1].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), adjust2(posmid.GetX(),posmid.GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY())) and
                BallNoCollision(info->pos[2].GetX(),info->pos[2].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), adjust2(posmid.GetX(),posmid.GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY())) ){
                    
                    poskick= pos5;
                    cout << "Moving to pos5: " << pos4<< endl << endl;
            }
                
                
            else if (BallNoCollision(info->pos[0].GetX(),info->pos[0].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), adjust3(posmid.GetX(),posmid.GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY())) and
                BallNoCollision(info->pos[1].GetX(),info->pos[1].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), adjust3(posmid.GetX(),posmid.GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY())) and
                BallNoCollision(info->pos[2].GetX(),info->pos[2].GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), adjust3(posmid.GetX(),posmid.GetY(),info->ballPosition.GetX(), info->ballPosition.GetY(), this->GetX(), this->GetY())) ){
                    
                    poskick= pos6;
                    cout << "Moving to pos6: " << pos4<< endl << endl;
            }
            
            
            cout << "go to a the kick position " << poskick<< endl << endl;
            this->GotoXY(poskick.GetX(),poskick.GetY(), 160, false);
            while (this->GetPos().DistanceTo(poskick.GetPos()) > 0.01) usleep(1000);
            cout << "kick" << endl << endl;
            this->GotoXY(info->ballPosition.GetX(),info->ballPosition.GetY(), 250, false);
            while (this->GetPos().DistanceTo(info->ballPosition.GetPos()) > 0.01) usleep(1000);
            
        }
        
        
    }


