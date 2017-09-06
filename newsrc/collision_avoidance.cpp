//Written by Ren Jiawei
#include"player.h"
#define PENALTY_UP 0.4
#define PENALTY_DOWN -0.4
#define PENALTY_LEFT -1.1
#define PENALTY_RIGHT 1.1
#define FIELD_UP 0.88
#define FIELD_DOWN -0.88
#define FIELD_LEFT -1.37
#define FIELD_RIGHT 1.37
/*
 * new GotoXY function with collision avoidance
 */
void player::GotoXY_ca(const double x, const double y, int32_t speed) {
            Position pos[6];//store all robots'positions
            Position dest;//destination position
            Position aforce;//destination's attractive force
            Position rforce[6];//robots' repulsive force
            Position wforce[4];//walls' repulsive force
            Position penalty_force[4];//penalty zone's repulsive force
            Position total_force;//total force calculated
            double dist[6];//distance from other robots
            double distWall[4];//distance from wall projections
            double distPenalty[4];//distance from penalty projections
            Angle diffA;//Angle difference between total force angle and moving direction

            // projections on the wall
            Position posWall[4]={Position(this->GetX(),FIELD_UP),
                                 Position(this->GetX(),FIELD_DOWN),
                                 Position(FIELD_LEFT,this->GetY()),
                                 Position(FIELD_RIGHT,this->GetY())
                                };
            //projections on penalty zones
            Position posPenalty[4]={Position(this->GetX(),PENALTY_UP),
                                    Position(this->GetX(),PENALTY_DOWN),
                                    Position(PENALTY_LEFT,this->GetY()),
                                    Position(PENALTY_RIGHT,this->GetY())
                                   };
            //set destination
            dest.SetX(x);
            dest.SetY(y);
            double minDistance=INF;

            //read shared position
            for(int i=0;i<6;i++)
            {
                pos[i]=info->pos[i];
                usleep(1000);
            }
            //distance to other robots
            for(int i=0;i<=5;i++)
            {
                if (i==robonumber) continue;
                dist[i]=this->GetPos().DistanceTo(pos[i])-2*roboRadius;
                if(minDistance>dist[i])
                    minDistance=dist[i];
            }
            //force by other robot
            for(int i=0;i<=5;i++)
            {
                if (i==robonumber) continue;
                double force=fconstant1/pow(dist[i],2);
                Angle rangle=this->GetPos().AngleOfLineToPos(pos[i].GetPos());
                rforce[i].SetX(-force*cos(rangle));
                rforce[i].SetY(-force*sin(rangle));
            }
            //distance to walls
            for(int i=0;i<=3;i++)
            {
                distWall[i]=this->GetPos().DistanceTo(posWall[i])-roboRadius;
                if(minDistance>distWall[i])
                    minDistance=distWall[i];
            }


            //force by walls
            for(int i=0;i<=3;i++)
            {
                double force=fconstant3/pow(distWall[i]-2*roboRadius,2);
                Angle rangle=this->GetPos().AngleOfLineToPos(posWall[i].GetPos());
                wforce[i].SetX(-force*cos(rangle));
                wforce[i].SetY(-force*sin(rangle));
            }


            //distance to penalty
            for(int i=0;i<=3;i++)
            {
                if(isInPenalty(posPenalty[i]))
                    distPenalty[i]=this->GetPos().DistanceTo(posPenalty[i])-roboRadius;
                else distPenalty[i]=INF;

                if(minDistance>distPenalty[i])
                    minDistance=distPenalty[i];
            }


            //force by penalty
            for(int i=0;i<=3;i++)
            {
                double force=fconstant4/pow(distPenalty[i],2);
                Angle rangle=this->GetPos().AngleOfLineToPos(posPenalty[i].GetPos());
                penalty_force[i].SetX(-force*cos(rangle));
                penalty_force[i].SetY(-force*sin(rangle));
            }

            //if min distance smaller than 0.1 enter CA mode
            //quit CA mode when min distance >0.2
            if(isAvoiding==true&&minDistance>0.2)//quit CA mode check
                isAvoiding=false;

            if(minDistance>0.1&&isAvoiding==false)//normal mode
            {
                this->GotoXY(x,y,speed);
                return;
            }
            else//CA mode
            {
                isAvoiding=true;
                //aforce by destination point
                Angle goangle=this->GetPos().AngleOfLineToPos(dest);
                aforce.SetX(fconstant2/pow(this->GetPos().DistanceTo(dest),2)*cos(goangle));
                aforce.SetY(fconstant2/pow(this->GetPos().DistanceTo(dest),2)*sin(goangle));

                // Init total force
                total_force.SetX(0);
                total_force.SetY(0);

                //calculate total force
                for(int i=0;i<=5;i++)
                {
                    if (i==robonumber) continue;
                    total_force+=rforce[i];
                }

                for(int i=0;i<4;i++)
                    total_force+=wforce[i];

                for(int i=0;i<4;i++)
                    total_force+=penalty_force[i];
                total_force+=aforce;

                //angle between totalforce and robot facing dirction
                diffA=Position(0,0).AngleOfLineToPos(total_force)-this->GetPhi();

                //Go straight when the force is less than 90 degree to the robot
                if(cos(diffA)>0)
                {
                    this->MoveMs(speed*0.67,speed*0.67,1000);
                }
                else{
                    //If angle larger than 145 degree. It d be too large. Split the angle. Otherwise the robot will oscillate.
                    if(fabs(sin(diffA))<0.7)
                    {
                        this->GotoXY(this->GetX()+cos(this->GetPhi()+0.5*diffA)*0.3,this->GetY()+sin(this->GetPhi()+0.5*diffA)*0.3,speed,true);
                        usleep(1000);

                    }
                    else{
                        //All else, go along the force direction
                        this->GotoXY(this->GetX()+cos(Position(0,0).AngleOfLineToPos(total_force))*0.3,this->GetY()+sin(Position(0,0).AngleOfLineToPos(total_force))*0.3,speed,true);
                        usleep(1000);
                    }

                }
            }

}



/*
 * new GotoPos function with collision avoidance
 */
void player::GotoPos_ca(Position received_dest, int32_t speed) {

            GotoXY_ca(received_dest.GetX(), received_dest.GetY(), speed);
}

/*
 * designed to simplify goto while loop
 * HOWEVER FUNCTION WAS NOT FURTHERLY USED
 */
void player::moveTo_ca(Position p)
{
    while(this->GetPos().DistanceTo(p)>0.1)
    {
        this->GotoPos_ca(p,80);
        usleep(2000);
    }
}
/*
 * Check if a position is in penalty zone
 */
bool player::isInPenalty(Position p)
{
    bool inPenalty=false;
    double x=p.GetX();
    double y=p.GetY();
    if(x<=PENALTY_LEFT&&x>=FIELD_LEFT)
        if(y<=PENALTY_UP&&y>=PENALTY_DOWN)
            inPenalty=true;
    if(x>=PENALTY_RIGHT&&x<=FIELD_RIGHT)
        if(y<=PENALTY_UP&&y>=PENALTY_DOWN)
            inPenalty=true;
    return inPenalty;
}


