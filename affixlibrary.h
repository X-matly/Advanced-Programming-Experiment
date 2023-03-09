#ifndef AFFIXLIBRARY_H
#define AFFIXLIBRARY_H

#include <QWidget>
#include <QRandomGenerator>

class AffixLibrary : public QWidget
{
    Q_OBJECT
private:
    int frenzied;   //近战塔：狂暴的
    int glacial;    //近战塔：冰系的
    int recovered;  //近战塔：恢复的
    int massInjured;//通用塔：群伤的
    int bloodletting;//远程塔：放血的
    int decelerating;//远程塔：减速的

public:
    explicit AffixLibrary(QWidget *parent = nullptr);
    int getFrenziedNumber();
    int getGlacialNumber();
    int getMassInjuredNumber();
    int getBloodLettingNumber();
    int getRecoveredNumber();
    int getDeceleratingNumber();
    int getAffix();
    bool haveFrenziedNumber();
    bool haveGlacialNumber();
    bool haveMassInjuredNumber();
    bool haveBloodLettingNumber();
    bool haveRecoveredNumber();
    bool haveDeceleratingNumber();
    QString translate(int type);
    void getRandom(int type);
    void addFrenziedNumber();
    void addGlacialNumber();
    void addMassInjuredNumber();
    void addBloodLettingNumber();
    void addRecoveredNumber();
    void addDeceleratingNumber();
    void subFrenziedNumber();
    void subGlacialNumber();
    void subMassInjuredNumber();
    void subBloodLettingNumber();
    void subRecoveredNumber();
    void subDeceleratingNumber();

signals:

};

#endif // AFFIXLIBRARY_H
