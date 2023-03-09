#ifndef BULLET_H
#define BULLET_H

#include <QWidget>
#include <QPoint>
#include <QtMath>
#include <QDebug>
#include "enemy.h"
#include "tower.h"

#define BULLETSPEED 25

class TowerBullet : public QWidget
{
    Q_OBJECT
private:
    int speed;       //子弹飞行速度
    bool alive;      //子弹是否到达
    bool frozen;     //是否为冰冻子弹
    bool bloodlet;   //是否为放血子弹
    bool slowDown;   //是否为减速子弹
    QPoint position; //子弹当前的位置
    Tower* source;   //子弹源头塔
    Enemy* target;  //目标类

public:
    explicit TowerBullet(Tower* sou,Enemy* tar,QWidget *parent = nullptr);
    bool isAlive();
    bool isFrozen();
    bool isBloodlet();
    bool isSlowDown();
    Tower* getSou();
    QPoint getPosition();
    void move();
    void vo();

signals:

};

class EnemyBullet : public QWidget    //怪物子弹类
{
    Q_OBJECT
private:
    int speed;       //子弹飞行速度
    bool alive;      //子弹是否到达
    QPoint position; //子弹当前的位置
    Enemy* source;   //子弹源头怪物
    Tower* target;  //目标类

public:
    explicit EnemyBullet(Enemy* sou,Tower* tar,QWidget *parent = nullptr);
    bool isAlive();
    QPoint getPosition();
    void move();
    void vo();

signals:

};

#endif // BULLET_H
