#include "map.h"
using namespace std;
Map::Map(QWidget *parent)
    : QWidget{parent}
{
    mapPath="../TowerDefenseGame/res/map_1.txt";
}

void Map::changeMapPath(QString mP)
{
    mapPath=mP;
    return;
}

void Map::readMap()
{
    QFile myMap(mapPath);                   //读取地图文件
    if(!myMap.open(QIODevice::ReadOnly|QIODevice::Text)){
            qDebug()<<"文件打开失败";
        }
    QTextStream in(&myMap);
    int add1=0;
    int add2=0;
    QVector<QPoint> vectemp;
    while(!in.atEnd())
    {
        if(add1!=18)
        {
            int temp;
            in >> temp;
            myGameMap[add1][add2]=temp;
            add2++;
            if(add2>=32)
            {
                add1+=1;
                add2=0;
            }
        }
        else
        {
            int x;
            in>>x;
            if(x!=99)
            {
                int y;
                in>>y;
                QPoint temp;
                temp.setX(x);
                temp.setY(y);
                vectemp.push_back(temp);
            }
            else
            {
                myGamePath.push_back(vectemp);
                vectemp.clear();
            }
        }
    }
}
