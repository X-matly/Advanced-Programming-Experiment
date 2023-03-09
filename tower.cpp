#include "tower.h"
#include "enemy.h"

Tower::Tower(int typ,QWidget *parent)
    : QWidget{parent}
{
    type=typ;
    index.setX(0);
    index.setY(0);
    alive=true;
    group=false;
    recoverd=false;
    eyeRange=false;
    bloodlet=false;
    slowDown=false;
    updateType_1=0;
    updateType_2=0;
    recoverdNumber=0;
    target=NULL;
    interval=0;
    tempInterval=0;
    deadTime=0;
    frozen=false;
    updateType=0;
    if(type==1)   //远程攻击塔
    {
        blood=100;
        sumBlood=100.0;
        aggressivity=20;
        range=200;
    }
    else if(type==2)   //近战攻击塔
    {
        blood=100;
        sumBlood=100.0;
        aggressivity=50;
        range=80;
    }
}

void Tower::findTarget(QVector<Enemy*> enemyVec)
{
    int min=100000;
    targetGroup.clear();
    target=NULL;
    /*if(!isGroup)
    {
        if(target!=NULL&&!target->isAlive())
        {
            target=NULL;
        }
    }
    else
    {
        if(targetGroup.size()!=0)
        {
            for(auto iter=targetGroup.begin();iter!=targetGroup.end();)
            {
                if(!(*iter)->isAlive())
                {
                    iter=targetGroup.erase(iter);
                }
                else
                {
                    iter++;
                }
            }
        }
    }*/

    for(auto iter:enemyVec)
    {
        if(iter->isAlive())
        {
            if(sqrt(pow((index.x()*50+25-iter->getPosition().x()-25),2)+pow((index.y()*50+25-iter->getPosition().y()-25),2))<=range)
            {
                if(group==true)
                {
                    targetGroup.push_back(iter);
                }
                else
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
}

void Tower::updateFrenzied()  //升级 近战塔 狂暴的
{
    if(updateType==0)
    {
        updateType=1;
        updateType_1=1;
    }
    else
    {
        updateType=7;
        updateType_2=1;
    }
    aggressivity*=2;
    interval=2;
}
void Tower::updateGlacial()    //升级 近战塔 冰系的
{
    if(updateType==0)
    {
        updateType=2;
        updateType_1=2;
    }
    else
    {
        updateType=7;
        updateType_2=2;
    }
    frozen=true;
    aggressivity+=20;
    interval=1;
}
void Tower::updateRecovered()  //升级 近战塔 恢复的
{
    if(updateType==0)
    {
        updateType=3;
        updateType_1=3;
    }
    else
    {
        updateType=7;
        updateType_2=3;
    }
    recoverd=true;
    recoverdNumber=5;
}
void Tower::updateMassInjured()//升级 通用塔 群伤的
{
    if(updateType==0)
    {
        updateType=4;
        updateType_1=4;
    }
    else
    {
        updateType=7;
        updateType_2=4;
    }
    group=true;

}
void Tower::updateBloodletting()//升级 远程塔 放血的
{
    if(updateType==0)
    {
        updateType=5;
        updateType_1=5;
    }
    else
    {
        updateType=7;
        updateType_2=5;
    }
    bloodlet=true;
}
void Tower::updateDecelerating()//升级 远程塔 减速的
{
    if(updateType==0)
    {
        updateType=6;
        updateType_1=6;
    }
    else
    {
        updateType=7;
        updateType_2=6;
    }
    slowDown=true;
}

void Tower::setEyeRange(bool is)
{
    eyeRange=is;
    return;
}

void Tower::setIndex(QPoint ind)
{
    index=ind;
    return;
}

void Tower::isRecoverd()
{
    if(recoverd)
    {
        if(blood<sumBlood)
        {
            if(blood+recoverdNumber<sumBlood)
            {
                blood+=recoverdNumber;
            }
            else
            {
                blood=sumBlood;
            }
        }
    }
    return;
}

void Tower::deadTimeAdd()
{
    deadTime++;
    return;
}

void Tower::resetTarget()
{
    target=NULL;
    return ;
}

void Tower::hitted(int aggress)
{
    blood-=aggress;
    if(blood<=0)
    {
        alive=false;
    }
    return ;
}

void Tower::resetTempInterval()
{
    tempInterval=interval;
}

void Tower::subTempInterval()
{
    tempInterval--;
    return;
}

QPoint Tower::getIndex()
{
    return index;
}

int Tower::getUpdateType()
{
    return updateType;
}

int Tower::getAggressivity()
{
    return aggressivity;
}

int Tower::getUpdateType_1()
{
    return updateType_1;
}

int Tower::getUpdateType_2()
{
    return updateType_2;
}

int Tower::getRange()
{
    return range;
}

bool Tower::getTempIntervalEqual()
{
    return (tempInterval==0);
}

bool Tower::isEyeRange()
{
    return eyeRange;
}

int Tower::getDeadTime()
{
    return deadTime;
}

int Tower::getType()
{
    return type;
}

int Tower::getBlood()
{
    return blood;
}

double Tower::getSumBlood()
{
    return sumBlood;
}

Enemy* Tower::getTarget()
{
    return target;
}

QVector<Enemy*> Tower::getTargetGroup()
{
    return targetGroup;
}

bool Tower::isAlive()
{
    return alive;
}

bool Tower::isGroup()
{
    return group;
}

bool Tower::isFrozen()
{
    return frozen;
}

bool Tower::isBloodlet()
{
    return bloodlet;
}

bool Tower::isSlowDown()
{
    return slowDown;
}

