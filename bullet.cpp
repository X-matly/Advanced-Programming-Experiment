#include "bullet.h"
#include <QDebug>

TowerBullet::TowerBullet(Tower *sou, Enemy* tar, QWidget *parent)
    : QWidget{parent}
{
    source=sou;
    target=tar;    
    speed=BULLETSPEED;
    alive=true;
    position.setX(source->getIndex().x()*50+25);
    position.setY(source->getIndex().y()*50+25);
    (!sou->isFrozen())?(frozen=false):(frozen=true);
    (!sou->isBloodlet())?(bloodlet=false):(bloodlet=true);
    (!sou->isSlowDown())?(slowDown=false):(slowDown=true);
}

Tower* TowerBullet::getSou()
{
    return source;
}

void TowerBullet::move()
{
    if(!target->isAlive())
    {
        alive=false;
        return;
    }
    QPoint tarPos=target->getPosition();
    double tempx=abs(position.x()-tarPos.x());
    double tempy=abs(position.y()-tarPos.y());
    if(tempy==0)
    {
        tempx=speed;
    }
    else
    {
        double ad=tempx/tempy;
        tempy=sqrt((speed*speed)/(1+ad*ad));
        tempx=ad*tempy;
    }

    (position.x()<target->getPosition().x())?(position.setX(position.x()+(int)tempx)):((position.x()>target->getPosition().x())?(position.setX(position.x()-(int)tempx)):(vo()));
    (position.y()<target->getPosition().y())?(position.setY(position.y()+(int)tempy)):((position.y()>target->getPosition().y())?(position.setY(position.y()-(int)tempy)):(vo()));

    if(sqrt(pow(position.x()-target->getPosition().x(),2)+pow(position.y()-target->getPosition().y(),2))<speed)
    {
        alive=false;
        target->hitted(source->getAggressivity());
        if(frozen)
        {
            target->setFrozen(true);
            target->setFrozenTime(10);
        }
        if(bloodlet)
        {
            target->setMaimedTime(30);
            target->setMaimed();
        }
        if(slowDown)
        {
            target->setSlowDown();
        }
        //qDebug()<<target->getBlood();
        if(!target->isAlive())
        {
            source->resetTarget();
        }
    }
    return ;
}

QPoint TowerBullet::getPosition()
{
    return position;
}

bool TowerBullet::isAlive()
{
    return alive;
}

bool TowerBullet::isFrozen()
{
    return frozen;
}

bool TowerBullet::isSlowDown()
{
    return slowDown;
}

void TowerBullet::vo()
{
    return;
}

EnemyBullet::EnemyBullet( Enemy*sou, Tower* tar, QWidget *parent)
    : QWidget{parent}
{
    source=sou;
    target=tar;
    speed=BULLETSPEED;
    alive=true;
    position.setX(source->getPosition().x());
    position.setY(source->getPosition().y());
}

void EnemyBullet::move()
{
    if(!target->isAlive())
    {
        alive=false;
        return;
    }
    QPoint tarPos;
    tarPos.setX(target->getIndex().x()*50+25);
    tarPos.setY(target->getIndex().y()*50+25);
    double tempx=abs(position.x()-tarPos.x());
    double tempy=abs(position.y()-tarPos.y());
    if(tempy==0)
    {
        tempx=speed;
    }
    else
    {
        double ad=tempx/tempy;
        tempy=sqrt((speed*speed)/(1+ad*ad));
        tempx=ad*tempy;
    }

    (position.x()<tarPos.x())?(position.setX(position.x()+(int)tempx)):((position.x()>tarPos.x())?(position.setX(position.x()-(int)tempx)):(vo()));
    (position.y()<tarPos.y())?(position.setY(position.y()+(int)tempy)):((position.y()>tarPos.y())?(position.setY(position.y()-(int)tempy)):(vo()));

    if(sqrt(pow(position.x()-tarPos.x(),2)+pow(position.y()-tarPos.y(),2))<speed)
    {

        alive=false;
        target->hitted(source->getAggressivity());
        //qDebug()<<target->getBlood();
        if(!target->isAlive())
        {
            source->resetTarget();
        }
    }
    return ;
}

QPoint EnemyBullet::getPosition()
{
    return position;
}

bool EnemyBullet::isAlive()
{
    return alive;
}

void EnemyBullet::vo()
{
    return;
}
