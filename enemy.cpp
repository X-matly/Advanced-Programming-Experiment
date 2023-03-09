#include "enemy.h"
#include "tower.h"

Enemy::Enemy(QWidget *parent)
    : QWidget{parent}
{
    int chance=QRandomGenerator::global()->bounded(100);
    frozen=false;
    maimed=false;
    slowDown=false;
    alive=true;
    get=false;
    position.setX(0);
    position.setY(0);
    flash=0;
    index=0;
    action=0;
    deadTime=0;
    frozenTime=0;
    maimedTime=0;
    range=80;
    enemyPath=NULL;
    target=NULL;
    if(chance>=0&&chance<=60)
    {
        blood=100;
        sumBlood=100.0;
        speed=10;
        aggressivity=40;
        type=1;
    }
    else if(chance>60&&chance<=80)
    {
        blood=60;
        sumBlood=60.0;
        speed=25;
        aggressivity=30;
        type=2;
    }
    else if(chance>80&&chance<=100)
    {
        blood=150;
        sumBlood=150.0;
        speed=10;
        aggressivity=20;
        type=3;
    }
}

void Enemy::setPath(QVector<QPoint>* path)
{
    enemyPath=path;
    if(enemyPath->at(0).x()==2)
    {
        position.setX(50);
        position.setY(enemyPath->at(0).y()*50);
    }
    else if(enemyPath->at(0).x()==17)
    {
        position.setX(900);
        position.setY(enemyPath->at(0).y()*50);
    }
    else if(enemyPath->at(0).y()==31)
    {
        position.setX(enemyPath->at(0).x()*50);
        position.setY(1600);
    }
    else if(enemyPath->at(0).y()==0)
    {
        position.setX(enemyPath->at(0).x()*50);
        position.setY(0);
    }
    return ;
}

void Enemy::findTarget(QVector<Tower*> towerVec)
{
    int min=100000;
    if(target!=NULL&&!target->isAlive())
    {
        target=NULL;
    }
    for(auto iter:towerVec)
    {
        if(iter->isAlive()&&iter->getType()==2)
        {
            if(sqrt(pow((iter->getIndex().x()*50+25-getPosition().x()-25),2)+pow((iter->getIndex().y()*50+25-getPosition().y()-25),2))<=range)
            {
                if(target!=NULL)
                {
                    if(target->getBlood()<min)
                    {
                        min=iter->getBlood();
                        target=iter;
                    }
                }
                else
                {
                    target=iter;
                    min=target->getBlood();
                }

            }
        }

    }
}

void Enemy::vo()
{
    return;
}

void Enemy::setFrozenTime(int time)
{
    frozenTime=time;
    return;
}

void Enemy::subFrozenTime()
{
    frozenTime--;
    if(frozenTime==0)
    {
        frozen=false;
    }
    return;
}

void Enemy::resetFrozenTime()
{
    frozenTime=0;
    return;
}

void Enemy::deadTimeAdd()
{
    deadTime++;
    return;
}

void Enemy::resetTarget()
{
    target=NULL;
    return;
}

void Enemy::move(int towerMap[18][32])
{
    if(type==3&&flash>0)
    {
        flash--;
    }
    if(frozen)   //被冻结
    {
        return;
    }
    else if(towerMap[enemyPath->at(index).x()][enemyPath->at(index).y()]==2)//前方近战防御塔
    {
        if(type==3&&flash==0)   //闪现的
        {
            (position.x()<enemyPath->at(index).x()*50)?(position.setX(position.x()+50)):((position.x()>enemyPath->at(index).x()*50)?(position.setX(position.x()-50)):(vo()));
            (position.y()<enemyPath->at(index).y()*50)?(position.setY(position.y()+50)):((position.y()>enemyPath->at(index).y()*50)?(position.setY(position.y()-50)):(vo()));
            index++;
            (position.x()<enemyPath->at(index).x()*50)?(position.setX(position.x()+50)):((position.x()>enemyPath->at(index).x()*50)?(position.setX(position.x()-50)):(vo()));
            (position.y()<enemyPath->at(index).y()*50)?(position.setY(position.y()+50)):((position.y()>enemyPath->at(index).y()*50)?(position.setY(position.y()-50)):(vo()));
            if(index>enemyPath->size()-1)
            {
                alive=false;
            }
            flash=50;
        }
        return;
    }
    (position.x()<enemyPath->at(index).x()*50)?(position.setX(position.x()+speed)):((position.x()>enemyPath->at(index).x()*50)?(position.setX(position.x()-speed)):(vo()));
    (position.y()<enemyPath->at(index).y()*50)?(position.setY(position.y()+speed)):((position.y()>enemyPath->at(index).y()*50)?(position.setY(position.y()-speed)):(vo()));

    if(position.x()==enemyPath->at(index).x()*50&&position.y()==enemyPath->at(index).y()*50)
    {
        index++;
        if(index>enemyPath->size()-1)
        {
            alive=false;
            get=true;
        }
    }
    return ;
}

void Enemy::setFrozen(bool is)
{
    frozen=is;
    return;
}

void Enemy::resetGet()
{
    get=false;
    return;
}

void Enemy::setMaimed()
{
    maimed=true;
    return;
}

void Enemy::setSlowDown()
{
    slowDown=true;
    speed=5;
    return;
}

void Enemy::setMaimedTime(int time)
{
    maimedTime=time;
    return;
}

void Enemy::subMaimedTime()
{
    maimedTime--;
    //qDebug()<<maimedTime;
    if(maimedTime==0)
    {
        maimed=false;
    }
    return;
}

void Enemy::hitted(int aggress)
{
    blood-=aggress;
    if(blood<=0)
    {
        alive=false;
    }
    return ;
}

bool Enemy::isGet()
{
    return get;
}

bool Enemy::isAlive()
{
    return alive;
}

bool Enemy::isFrozen()
{
    return frozen;
}

bool Enemy::isSlowDown()
{
    return slowDown;
}

bool Enemy::isMaimed()
{
    if(maimed)
    {
        subMaimedTime();
        blood-=1;
        if(blood<=0)
        {
            alive=false;
        }
    }
    return maimed;
}

QPoint Enemy::getPosition()
{
    return position;
}

double Enemy::getSumBlood()
{
    return sumBlood;
}

int Enemy::getBlood()
{
    return blood;
}

int Enemy::getFrozenTime()
{
    return frozenTime;
}

int Enemy::getAggressivity()
{
    return aggressivity;
}

int Enemy::getType()
{
    return type;
}

int Enemy::getAction()
{
    if(!frozen)
    {
        action++;
        if(action>2)
        {
            action=1;
        }
    }
    return action;
}

int Enemy::getDeadTime()
{
    return deadTime;
}

Tower* Enemy::getTarget()
{
    return target;
}

