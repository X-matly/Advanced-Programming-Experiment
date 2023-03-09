#ifndef ENEMY_H
#define ENEMY_H

#include <QWidget>
#include <QVector>
#include <QtMath>
#include <QDebug>
#include <QRandomGenerator>

class Tower;


class Enemy : public QWidget
{
    Q_OBJECT
private:
    int blood;  //即时血量
    int speed;  //移动速度
    int aggressivity; //攻击力
    int type;    //敌人类型
    int index;   //当前位置在enemyPath中的下标位置
    int deadTime; //确定收尸时间
    int range;    //敌人的攻击范围
    int action;   //敌人的动画
    int flash;    //闪现冷却
    int frozenTime;
    int maimedTime;
    double sumBlood; //初始血量
    bool alive;   //是否存活
    bool frozen;  //是否冻结
    bool maimed;  //是否受伤
    bool slowDown;//是否减速
    bool get;    //是否完成了任务
    QPoint position; //当前位置，以像素点格式表示
    Tower* target;

    QVector<QPoint>* enemyPath;

public:
    explicit Enemy(QWidget *parent = nullptr);
    void setPath(QVector<QPoint>* path);
    void move(int towerMap[18][32]);
    void vo();
    void hitted(int aggress);
    void deadTimeAdd();
    void findTarget(QVector<Tower*> towerVec);
    void resetTarget();
    void setFrozen(bool is);
    void setMaimed();
    void setSlowDown();
    void setFrozenTime(int time);
    void setMaimedTime(int time);
    void subFrozenTime();
    void subMaimedTime();
    void resetFrozenTime();
    void resetGet();
    double getSumBlood();
    int getFrozenTime();
    int getBlood();
    int getAction();
    int getDeadTime();
    int getType();
    int getAggressivity();
    bool isGet();
    bool isAlive();
    bool isFrozen();
    bool isMaimed();
    bool isSlowDown();

    QPoint getPosition();
    Tower* getTarget();


signals:

};

#endif // ENEMY_H
