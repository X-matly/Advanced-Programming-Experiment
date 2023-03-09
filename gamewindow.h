#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QRandomGenerator>
#include <QDebug>
#include <QVector>
#include <QTimer>
#include <QMessageBox>
#include <QMediaPlayer>
#include <QSound>
#include <QMouseEvent>
#include "map.h"
#include "enemy.h"
#include "tower.h"
#include "bullet.h"
#include "affixlibrary.h"

class GameWindow : public QWidget
{
    Q_OBJECT
private:
    int towerPlace; //放置防御塔参数
    Map* gameMap;
    AffixLibrary* affixLibrary;//词缀库
    int towerMap[18][32]; //当前防御塔地图
    int aff;       //当前帧获得的词缀
    int affTime;   //词缀展示时间
    int money;     //金钱数量
    int heart;     //血量
    QVector<Enemy*> enemyVector;
    QVector<Tower*> towerVector;
    QVector<TowerBullet*>towerBulletVector;
    QVector<EnemyBullet*>enemyBulletVector;
public:
    explicit GameWindow(QString mP,QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent* event);

    void paintEvent(QPaintEvent*); //重写绘图事件
    void paintMenuBar(QPainter& painter);//绘制菜单栏
    void paintTower(QPainter& painter);//绘制防御塔
    void paintEnemy(QPainter& painter);//绘制敌人
    void paintBullet(QPainter& painter);//绘制子弹
    void paintMap(QPainter& painter);//绘制地图
    void setMap(QString mP); //设置地图

signals:

};

#endif // GAMEWINDOW_H
