#include "gamewindow.h"

GameWindow::GameWindow(QString mP,QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(1600,900);//固定窗口为1600*900
    aff=0;
    affTime=0;
    money=300;
    heart=10;
    gameMap=new Map;
    setMap(mP);
    gameMap->readMap();
    affixLibrary=new AffixLibrary;
    towerPlace=0;
    for(int i=0;i<18;i++)
    {
        for(int j=0;j<32;j++)
        {
            towerMap[i][j]=0;
        }
    }


    QMediaPlayer *startSound=new QMediaPlayer;  //创建一个音乐播放器
    startSound->setMedia(QUrl("../TowerDefenseGame/res/game.mp3"));
    startSound->setVolume(80);  //音量
    startSound->play();
    QTimer* musicTimer = new QTimer(this);  //播放音乐的定时器
    musicTimer->start(182000);
    connect(musicTimer,&QTimer::timeout,[=]()
    {
        startSound->play();
    });



    QTimer* enemyCreateTimer = new QTimer(this);  //产生怪物的定时器
    enemyCreateTimer->start(2000);
    connect(enemyCreateTimer,&QTimer::timeout,[=]()
    {
        int test=QRandomGenerator::global()->bounded(100);
        Enemy* temp=new Enemy;
        temp->setPath(&gameMap->myGamePath[test%gameMap->myGamePath.size()]);
        enemyVector.push_back(temp);
    });

    QTimer* enemyRunTimer = new QTimer(this);  //怪物运动的定时器
    enemyRunTimer->start(100);
    connect(enemyRunTimer,&QTimer::timeout,[=]()
    {
        for(auto iter=enemyVector.begin();iter!=enemyVector.end();)
        {
            (*iter)->isMaimed();
            if((*iter)->isAlive()&&!(*iter)->isFrozen())
            {
                (*iter)->move(towerMap);
                iter++;
            }
            else if((*iter)->isAlive()&&(*iter)->isFrozen())
            {
                (*iter)->subFrozenTime();
                iter++;
            }
            else
            {
                (*iter)->deadTimeAdd();
                if((*iter)->getDeadTime()==1)
                {
                    QSound::play("../TowerDefenseGame/res/bombhit.wav");
                    int chance=QRandomGenerator::global()->bounded(100);
                    if(chance<50)
                    {
                        aff=affixLibrary->getAffix();
                        QSound::play("../TowerDefenseGame/res/coins.wav");
                        affTime=20;
                    }
                    if((*iter)->isGet())
                    {
                        heart--;
                        (*iter)->resetGet();
                        if(heart<=0)
                        {
                            QMessageBox::critical(this,"错误信息","死掉啦！");
                            this->close();
                        }

                    }
                }
                if((*iter)->getDeadTime()>=11)
                {
                    delete (*iter);
                    iter=enemyVector.erase(iter);
                }
            }
        }
        update();
    });

    QTimer* towerAliveTimer = new QTimer(this);  //防御塔存活判定的定时器
    towerAliveTimer->start(100);
    connect(towerAliveTimer,&QTimer::timeout,[=]()
    {
        for(auto iter=towerVector.begin();iter!=towerVector.end();)
        {
            if((*iter)->isAlive())
            {
                iter++;
            }
            else
            {
                (*iter)->deadTimeAdd();
                if((*iter)->getDeadTime()==1)
                {
                    QSound::play("../TowerDefenseGame/res/comet.wav");
                    towerMap[(*iter)->getIndex().x()][(*iter)->getIndex().y()]=0;
                    int test=QRandomGenerator::global()->bounded(100);
                    if(test>=0&&test<30)
                    {
                        int t=QRandomGenerator::global()->bounded(100);
                        if(t<=50)
                        {
                            affixLibrary->getRandom((*iter)->getUpdateType_1());
                        }
                        else
                        {
                            affixLibrary->getRandom((*iter)->getUpdateType_2());
                        }
                    }
                    else if(test<40)
                    {
                        affixLibrary->getRandom((*iter)->getUpdateType_1());
                        affixLibrary->getRandom((*iter)->getUpdateType_2());
                    }
                }
                else if((*iter)->getDeadTime()>=11)
                {
                    delete (*iter);
                    iter=towerVector.erase(iter);
                }
            }
        }

    });

    QTimer* towerAttackTimer = new QTimer(this);  //防御塔攻击的定时器
    towerAttackTimer->start(500);
    connect(towerAttackTimer,&QTimer::timeout,[=]()
    {
        if(money<=300)
        {
            money+=5;
        }
        for(auto iter:towerVector)
        {
            iter->isRecoverd();
            if(iter->getTempIntervalEqual())
            {
                iter->findTarget(enemyVector);
                if(!iter->isGroup())
                {
                    if(iter->getTarget()!=NULL)
                    {
                        TowerBullet* temp=new TowerBullet(iter,iter->getTarget());
                        QSound::play("../TowerDefenseGame/res/energy.wav");
                        towerBulletVector.push_back(temp);
                    }
                }
                else
                {
                    for(auto it:iter->getTargetGroup())
                    {
                        TowerBullet* temp=new TowerBullet(iter,it);
                        QSound::play("../TowerDefenseGame/res/energy.wav");
                        towerBulletVector.push_back(temp);
                    }
                }
                iter->resetTempInterval();
            }
            else
            {
                iter->subTempInterval();
            }
        }
        /*qDebug()<<bulletVector.size();
        for(auto iter:bulletVector)
        {
            qDebug()<<iter->getPosition().x()<<" "<<iter->getPosition().y();
        }*/
    });

    QTimer* enemyAttackTimer = new QTimer(this);  //怪物攻击的定时器
    enemyAttackTimer->start(600);
    connect(enemyAttackTimer,&QTimer::timeout,[=]()
    {
        for(auto iter:enemyVector)
        {
            if(!iter->isFrozen())
            {
                iter->findTarget(towerVector);
                if(iter->getTarget()!=NULL)
                {
                    EnemyBullet* temp=new EnemyBullet(iter,iter->getTarget());
                    enemyBulletVector.push_back(temp);
                }
            }
        }
    });

    QTimer* bulletFlyTimer = new QTimer(this);  //子弹飞行的定时器
    bulletFlyTimer->start(30);
    connect(bulletFlyTimer,&QTimer::timeout,[=]()
    {
        for(auto iter=towerBulletVector.begin();iter!=towerBulletVector.end();)
        {
            if((*iter)->isAlive())
            {
                (*iter)->move();
                iter++;
            }
            else
            {
                delete (*iter);
                iter=towerBulletVector.erase(iter);
            }
        }
        for(auto iter=enemyBulletVector.begin();iter!=enemyBulletVector.end();)
        {
            if((*iter)->isAlive())
            {
                (*iter)->move();
                iter++;
            }
            else
            {
                delete (*iter);
                iter=enemyBulletVector.erase(iter);
            }
        }
        update();
    });
}

void GameWindow::mousePressEvent(QMouseEvent* event)//重写鼠标点击事件
{
    if (event->button() == Qt::LeftButton) //鼠标左键点击
    {
        QPoint mousePos=event->pos();
        if(mousePos.x()>45&&mousePos.x()<145&&mousePos.y()>25&&mousePos.y()<95)  //点击1号远程防御塔菜单栏图标
        {
            (towerPlace!=1)?(towerPlace=1):(towerPlace=0);
        }
        else if(mousePos.x()>245&&mousePos.x()<345&&mousePos.y()>25&&mousePos.y()<95)  //点击2号近战防御塔菜单栏图标
        {
            (towerPlace!=2)?(towerPlace=2):(towerPlace=0);
        }
        else if(mousePos.x()>650&&mousePos.x()<785&&mousePos.y()>30&&mousePos.y()<60)   //点击 近战的 狂暴的
        {
            (towerPlace!=4)?(towerPlace=4):(towerPlace=0);
        }
        else if(mousePos.x()>650&&mousePos.x()<785&&mousePos.y()>60&&mousePos.y()<95)   //点击 近战的 冰冻的
        {
            (towerPlace!=5)?(towerPlace=5):(towerPlace=0);
        }
        else if(mousePos.x()>785&&mousePos.x()<910&&mousePos.y()>30&&mousePos.y()<60)   //点击 近战的 恢复的
        {
            //qDebug()<<"1";
            (towerPlace!=6)?(towerPlace=6):(towerPlace=0);
        }
        else if(mousePos.x()>785&&mousePos.x()<910&&mousePos.y()>60&&mousePos.y()<95)   //点击 通用的 群伤的
        {
            //qDebug()<<"1";
            (towerPlace!=7)?(towerPlace=7):(towerPlace=0);
        }
        else if(mousePos.x()>910&&mousePos.x()<1035&&mousePos.y()>30&&mousePos.y()<60)   //点击 远程的 放血的
        {
            //qDebug()<<"1";
            (towerPlace!=8)?(towerPlace=8):(towerPlace=0);
        }
        else if(mousePos.x()>910&&mousePos.x()<1035&&mousePos.y()>60&&mousePos.y()<95)   //点击 远程的 减速的
        {
            //qDebug()<<"1";
            (towerPlace!=9)?(towerPlace=9):(towerPlace=0);
        }
        else if(mousePos.x()>1455&&mousePos.x()<1555&&mousePos.y()>25&&mousePos.y()<95)   //点击 铲子
        {
            //qDebug()<<"1";
            (towerPlace!=10)?(towerPlace=10):(towerPlace=0);
        }
        else if(mousePos.y()>100)    //点击地图区域
        {
            QPoint floorIndex;
            floorIndex.setX(mousePos.y()/50);
            floorIndex.setY(mousePos.x()/50);
            if(towerPlace==0)
            {
                if(towerMap[floorIndex.x()][floorIndex.y()]!=0)
                {
                    for(auto iter:towerVector)
                    {
                        if(iter->getIndex().x()==floorIndex.x()&&iter->getIndex().y()==floorIndex.y())
                        {
                            iter->isEyeRange()?iter->setEyeRange(false):iter->setEyeRange(true);
                            break;
                        }
                    }
                }
            }
            else if(towerPlace==1)   //放置1号远程防御塔
            {
                if(money>=100&&gameMap->myGameMap[floorIndex.x()][floorIndex.y()]==0&&towerMap[floorIndex.x()][floorIndex.y()]==0)
                {
                    Tower* temp=new Tower(1);
                    temp->setIndex(floorIndex);
                    towerVector.push_back(temp);
                    towerMap[floorIndex.x()][floorIndex.y()]=1;
                    money-=100;
                    QSound::play("../TowerDefenseGame/res/build.wav");
                }
                towerPlace=0;
            }
            else if(towerPlace==2)   //放置2号近战防御塔
            {
                if(money>=100&&gameMap->myGameMap[floorIndex.x()][floorIndex.y()]==1&&towerMap[floorIndex.x()][floorIndex.y()]==0)
                {
                    Tower* temp=new Tower(2);
                    temp->setIndex(floorIndex);
                    towerVector.push_back(temp);
                    towerMap[floorIndex.x()][floorIndex.y()]=2;
                    money-=100;
                    QSound::play("../TowerDefenseGame/res/build.wav");
                }
                towerPlace=0;
            }
            else if(towerPlace==4)   //放置词缀 近战的 狂暴的
            {
                if(gameMap->myGameMap[floorIndex.x()][floorIndex.y()]==1&&towerMap[floorIndex.x()][floorIndex.y()]==2)
                {
                    for(auto iter:towerVector)
                    {
                        if(iter->getIndex()==floorIndex&&iter->getUpdateType()!=1&&iter->getUpdateType()!=7)
                        {
                            //qDebug()<<"get";
                            if(affixLibrary->haveFrenziedNumber())
                            {
                                iter->updateFrenzied();
                                affixLibrary->subFrenziedNumber();
                                QSound::play("../TowerDefenseGame/res/diskhit.wav");
                            }
                            break;
                        }
                    }
                }
                towerPlace=0;
            }
            else if(towerPlace==5)   //放置词缀 近战的 冰冻的
            {
                if(gameMap->myGameMap[floorIndex.x()][floorIndex.y()]==1&&towerMap[floorIndex.x()][floorIndex.y()]==2)
                {
                    for(auto iter:towerVector)
                    {
                        if(iter->getIndex()==floorIndex&&iter->getUpdateType()!=2&&iter->getUpdateType()!=7)
                        {
                            //qDebug()<<"get";
                            if(affixLibrary->haveGlacialNumber())
                            {
                                iter->updateGlacial();
                                affixLibrary->subGlacialNumber();
                                QSound::play("../TowerDefenseGame/res/diskhit.wav");
                            }
                            break;
                        }
                    }
                }
                towerPlace=0;
            }
            else if(towerPlace==6)   //放置词缀 近战的 恢复的
            {
                if(gameMap->myGameMap[floorIndex.x()][floorIndex.y()]==1&&towerMap[floorIndex.x()][floorIndex.y()]==2)
                {
                    for(auto iter:towerVector)
                    {
                        if(iter->getIndex()==floorIndex&&iter->getUpdateType()!=3&&iter->getUpdateType()!=7)
                        {
                            //qDebug()<<"get";
                            if(affixLibrary->haveRecoveredNumber())
                            {
                                iter->updateRecovered();
                                affixLibrary->subRecoveredNumber();
                                QSound::play("../TowerDefenseGame/res/diskhit.wav");
                            }
                            break;
                        }
                    }
                }
                towerPlace=0;
            }
            else if(towerPlace==7)   //放置词缀 通用的 群伤的
            {
                if(towerMap[floorIndex.x()][floorIndex.y()]==1||towerMap[floorIndex.x()][floorIndex.y()]==2)
                {
                    for(auto iter:towerVector)
                    {
                        if(iter->getIndex()==floorIndex&&iter->getUpdateType()!=4&&iter->getUpdateType()!=7)
                        {
                            //qDebug()<<"get";
                            if(affixLibrary->haveMassInjuredNumber())
                            {
                                iter->updateMassInjured();
                                affixLibrary->subMassInjuredNumber();
                                QSound::play("../TowerDefenseGame/res/diskhit.wav");
                            }
                            break;
                        }
                    }
                }
                towerPlace=0;
            }
            else if(towerPlace==8)   //放置词缀 远程的 放血的
            {
                if(gameMap->myGameMap[floorIndex.x()][floorIndex.y()]==0&&towerMap[floorIndex.x()][floorIndex.y()]==1)
                {
                    for(auto iter:towerVector)
                    {
                        if(iter->getIndex()==floorIndex&&iter->getUpdateType()!=5&&iter->getUpdateType()!=7)
                        {
                            if(affixLibrary->haveBloodLettingNumber())
                            {
                                iter->updateBloodletting();
                                affixLibrary->subBloodLettingNumber();
                                QSound::play("../TowerDefenseGame/res/diskhit.wav");
                            }
                            break;
                        }
                    }
                }
                towerPlace=0;
            }
            else if(towerPlace==9)   //放置词缀 远程的 减速的
            {
                if(gameMap->myGameMap[floorIndex.x()][floorIndex.y()]==0&&towerMap[floorIndex.x()][floorIndex.y()]==1)
                {
                    for(auto iter:towerVector)
                    {
                        if(iter->getIndex()==floorIndex&&iter->getUpdateType()!=6&&iter->getUpdateType()!=7)
                        {
                            //qDebug()<<"get";
                            if(affixLibrary->haveDeceleratingNumber())
                            {
                                iter->updateDecelerating();
                                affixLibrary->subDeceleratingNumber();
                                QSound::play("../TowerDefenseGame/res/diskhit.wav");
                            }
                            break;
                        }
                    }
                }
                towerPlace=0;
            }
            else if(towerPlace==10)   //铲子
            {
                if(towerMap[floorIndex.x()][floorIndex.y()]==1||towerMap[floorIndex.x()][floorIndex.y()]==2)
                {
                    for(auto iter:towerVector)
                    {
                        if(iter->getIndex()==floorIndex)
                        {
                            //qDebug()<<"get";
                            iter->hitted(1000000);
                            money+=20;
                            QSound::play("../TowerDefenseGame/res/comet.wav");
                            break;
                        }
                    }
                }
                towerPlace=0;
            }
        }
    }
}

void GameWindow::paintEvent(QPaintEvent*) //重写绘图事件
{
    QPainter painter(this);

    paintMap(painter);

    paintEnemy(painter);
    paintTower(painter);
    paintBullet(painter);

    paintMenuBar(painter);
}

void GameWindow::paintMenuBar(QPainter& painter)  //绘制菜单栏
{
    QPixmap menuBar("../TowerDefenseGame/res/menuBar.png");  //菜单栏图标
    QPixmap towerBar1("../TowerDefenseGame/res/tower_1.png"); //防御塔图标1
    QPixmap towerBar2("../TowerDefenseGame/res/tower_2.png"); //防御塔图标2
    QPixmap affix("../TowerDefenseGame/res/affixlibrary.png"); //词缀库图标
    QPixmap shovel("../TowerDefenseGame/res/shovel.jpg");     //铲子图标

    painter.drawPixmap(0,0,1600,100,menuBar);
    painter.drawPixmap(45,25,100,70,towerBar1);
    painter.drawPixmap(245,25,100,70,towerBar2);
    painter.drawPixmap(645,25,400,68,affix);
    painter.drawPixmap(1455,25,100,70,shovel);

    QString frenzied="近战塔 狂暴的:" + QString::number(affixLibrary->getFrenziedNumber());//近战塔 狂暴的
    QString glacial="近战塔 冰系的:" + QString::number(affixLibrary->getGlacialNumber());//近战塔 冰系的
    QString recovered="近战塔 恢复的:" + QString::number(affixLibrary->getRecoveredNumber());//近战塔 恢复的
    QString massInjured="通用塔 群伤的:" + QString::number(affixLibrary->getMassInjuredNumber());//通用塔 群伤的
    QString bloodletting="远程塔 放血的:" + QString::number(affixLibrary->getBloodLettingNumber());//远程塔 放血的
    QString decelerating="远程塔 减速的:" + QString::number(affixLibrary->getDeceleratingNumber());//远程塔 减速的

    QString myMoney="现有金钱："+QString::number(money);
    QString myHeart="现有血量："+QString::number(heart);

    painter.drawText(660,50, frenzied);
    painter.drawText(660,80, glacial);
    painter.drawText(790,50, recovered);
    painter.drawText(790,80, massInjured);
    painter.drawText(915,50, bloodletting);
    painter.drawText(915,80, decelerating);

    painter.drawText(1300,50,myMoney);
    painter.drawText(1300,80,myHeart);


    if(aff)
    {
        painter.drawText(1050,50, "获得词缀："+affixLibrary->translate(aff));
        if(!--affTime)
        {
            aff=0;
        }
    }
}

void GameWindow::paintBullet(QPainter& painter)   //绘制子弹
{
    QPixmap bullet_1("../TowerDefenseGame/res/bullet_1.png");
    QPixmap bullet_2("../TowerDefenseGame/res/bullet_2.png");
    for(auto iter:towerBulletVector)
    {
        if(iter->isAlive())
        {
            if(iter->getSou()->getType()==1)
            {
                painter.drawPixmap(iter->getPosition().y(),iter->getPosition().x(),30,30,bullet_1);
            }
            else if(iter->getSou()->getType()==2)
            {
                painter.drawPixmap(iter->getPosition().y(),iter->getPosition().x(),60,60,bullet_2);
            }
        }
    }
    for(auto iter:enemyBulletVector)
    {
        if(iter->isAlive())
        {
            painter.drawPixmap(iter->getPosition().y(),iter->getPosition().x(),60,60,bullet_2);
        }
    }
}

void GameWindow::paintTower(QPainter& painter)   //绘制防御塔
{
    QPixmap tower_1("../TowerDefenseGame/res/tower1.png");
    QPixmap tower_2("../TowerDefenseGame/res/tower2.png");
    for(auto iter:towerVector)
    {
        if(iter->getType()==1)
        {
            painter.drawPixmap(iter->getIndex().y()*50,iter->getIndex().x()*50-20,50,70,tower_1);           
            painter.drawText(iter->getIndex().y()*50,iter->getIndex().x()*50-30,affixLibrary->translate(iter->getUpdateType_1()));
            painter.drawText(iter->getIndex().y()*50,iter->getIndex().x()*50-45,affixLibrary->translate(iter->getUpdateType_2()));
            painter.setBrush(Qt::red);
            painter.drawRect(iter->getIndex().y()*50,iter->getIndex().x()*50-25,50*((iter->getBlood()/iter->getSumBlood())>=0?(iter->getBlood()/iter->getSumBlood()):0),5);
        }
        else if(iter->getType()==2)
        {
            painter.drawPixmap(iter->getIndex().y()*50-10,iter->getIndex().x()*50-30,60,80,tower_2);
            painter.drawText(iter->getIndex().y()*50,iter->getIndex().x()*50-30,affixLibrary->translate(iter->getUpdateType_1()));
            painter.drawText(iter->getIndex().y()*50,iter->getIndex().x()*50-45,affixLibrary->translate(iter->getUpdateType_2()));
            painter.setBrush(Qt::red);
            painter.drawRect(iter->getIndex().y()*50,iter->getIndex().x()*50-25,50*((iter->getBlood()/iter->getSumBlood())>=0?(iter->getBlood()/iter->getSumBlood()):0),5);
        }
        if(iter->isEyeRange())
        {
            QColor gray20 = Qt::gray;
            gray20.setAlphaF(0.2);
            painter.setBrush(gray20);
            painter.drawEllipse(iter->getIndex().y()*50-iter->getRange()/2+25,iter->getIndex().x()*50-iter->getRange()/2+25,iter->getRange(),iter->getRange());
        }
    }
}

void GameWindow::paintEnemy(QPainter& painter)   //绘制敌人
{
    QPixmap enemy("../TowerDefenseGame/res/enemy1.png");
    for(auto iter:enemyVector)
    {
        if(iter->isAlive())
        {
             painter.drawPixmap(iter->getPosition().y()-5,iter->getPosition().x()-10,60,60,enemy);                        
             painter.setBrush(Qt::red);
             painter.drawRect(iter->getPosition().y()-5,iter->getPosition().x()-15,60*(iter->getBlood()/iter->getSumBlood()),5);
             if(iter->getType()==1)
             {
                 painter.drawText(iter->getPosition().y(),iter->getPosition().x()-20,"普通的");
             }
             else if(iter->getType()==2)
             {
                 painter.drawText(iter->getPosition().y(),iter->getPosition().x()-20,"神速的");
             }
             else if(iter->getType()==3)
             {
                 painter.drawText(iter->getPosition().y(),iter->getPosition().x()-20,"闪现的");
             }
             int temp=35;
             if(iter->isFrozen())
             {
                 painter.drawText(iter->getPosition().y(),iter->getPosition().x()-temp,"冰冻了");
                 temp+=15;
             }
             if(iter->isMaimed())
             {
                 painter.drawText(iter->getPosition().y(),iter->getPosition().x()-temp,"受伤了");
                 temp+=15;
             }
             if(iter->isSlowDown())
             {
                 painter.drawText(iter->getPosition().y(),iter->getPosition().x()-temp,"减速了");
             }
        }
    }
}

void GameWindow::paintMap(QPainter& painter)   //绘制地图
{
    QPixmap towerFloor("../TowerDefenseGame/res/towerFloor.png");
    QPixmap enemyFloor("../TowerDefenseGame/res/enemyFloor.png");
    for(int i=2;i<18;i++)
    {
        for(int j=0;j<32;j++)
        {
            if(gameMap->myGameMap[i][j]==1)
            {
                painter.drawPixmap(j*50,i*50,50,50,enemyFloor);
            }
            else
            {
                painter.drawPixmap(j*50,i*50,50,50,towerFloor);
            }
        }
    }
}

void GameWindow::setMap(QString mP)
{
    gameMap->changeMapPath(mP);
    return;
}
