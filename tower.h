#ifndef TOWER_H
#define TOWER_H

#include <QWidget>
#include <QPoint>
#include <QtMath>

class Enemy;

class Tower : public QWidget
{
    Q_OBJECT
private:
    QPoint index;   //防御塔所在的地图格编号
    int tempInterval;   //防御塔攻击间隔计数
    int interval;   //防御塔攻击间隔
    int type;       //防御塔类型 1：远程 2：近战
    int blood;      //防御塔血量
    int range;      //防御塔攻击范围
    int aggressivity; //防御塔攻击力
    int deadTime; //确定收尸时间
    int updateType; //防御塔强化类型
    int recoverdNumber; //回复血量数
    int updateType_1;  //强化类型储存1
    int updateType_2;  //强化类型储存2
    double sumBlood;  //防御塔初始血量
    bool group; //防御塔攻击数量
    bool eyeRange; //是否显示攻击范围
    bool recoverd; //是否为恢复型
    bool frozen;//是否为冰冻型
    bool bloodlet;//是否为放血型
    bool slowDown;//是否为减速型
    bool alive;
    QVector<Enemy*> targetGroup;  //目标敌人指针组
    Enemy* target;   //目标敌人指针

public:

    explicit Tower(int typ,QWidget *parent = nullptr);
    void setIndex(QPoint ind);   
    void findTarget(QVector<Enemy*> enemyVec);
    void resetTarget();
    void deadTimeAdd();
    void hitted(int aggress);
    void resetTempInterval();
    void subTempInterval();
    void setUpdateType(int t);
    void setEyeRange(bool is);
    void updateFrenzied();  //升级 近战塔 狂暴的
    void updateGlacial();    //升级 近战塔 冰系的
    void updateRecovered();  //升级 近战塔 恢复的
    void updateMassInjured();//升级 通用塔 群伤的
    void updateBloodletting();//升级 远程塔 放血的
    void updateDecelerating();//升级 远程塔 减速的
    void isRecoverd();
    int getUpdateType();
    int getAggressivity();    
    int getRange();
    int getType();
    int getBlood();
    int getDeadTime();
    int getUpdateType_1();
    int getUpdateType_2();
    double getSumBlood();
    bool isEyeRange();
    bool isAlive();
    bool isGroup();
    bool isFrozen();
    bool isBloodlet();
    bool isSlowDown();
    bool getTempIntervalEqual();
    QPoint getIndex();
    Enemy* getTarget();
    QVector<Enemy*> getTargetGroup();

signals:

};

#endif // TOWER_H
