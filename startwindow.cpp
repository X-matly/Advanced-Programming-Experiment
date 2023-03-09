#include "startwindow.h"
#include "ui_startwindow.h"
#include "gamewindow.h"

StartWindow::StartWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    QPushButton * startButton = new QPushButton;//开始游戏按钮
    QPushButton * mapButton = new QPushButton;//开始游戏按钮
    QMediaPlayer *startSound=new QMediaPlayer;  //创建一个音乐播放器
    startSound->setMedia(QUrl("../TowerDefenseGame/res/menu.mp3"));
    startSound->setVolume(80);  //音量
    startSound->play();
    startButton->setParent(this);
    mapButton->setParent(this);
    startButton->setText("开始游戏");
    mapButton->setText("导入地图");
    startButton->move(700,300);
    mapButton->move(700,500);
    setFixedSize(1600,900);
    startButton->resize(200,100);
    mapButton->resize(200,100);
    mP="../TowerDefenseGame/res/map_1.txt";

    connect(startButton, &QPushButton::clicked, [=]()  //点击开始游戏后关闭窗口并创建游戏界面窗口
        {
            GameWindow* gameWindow = new GameWindow(mP);
            delete startSound;
            this->close();
            gameWindow->show();
        });

    connect(mapButton, &QPushButton::clicked, [=]()  //点击导入地图后选择地图文件
        {
            mP=QFileDialog::getOpenFileName(this,tr("文件"),"",tr("text(*.txt)"));
        });
}

StartWindow::~StartWindow()
{
    delete ui;
}

