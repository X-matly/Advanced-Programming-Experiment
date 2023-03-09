#ifndef MAP_H
#define MAP_H

#include <QWidget>
#include <QFile>
#include <QDebug>
#include <QString>
#include <QTextStream>
#include <QIcon>
#include <QVector>
#include <QPoint>
class Map : public QWidget
{
    Q_OBJECT
protected:
    QString mapPath;
    int myGameMap[18][32];  //游戏地图，采用50*50格子
    QVector<QVector<QPoint>> myGamePath;
public:
    explicit Map(QWidget *parent = nullptr);
    void readMap();
    void changeMapPath(QString mP);
    friend class GameWindow;

signals:

};

#endif // MAP_H
