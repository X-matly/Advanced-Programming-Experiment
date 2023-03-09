#include "affixlibrary.h"

AffixLibrary::AffixLibrary(QWidget *parent)
    : QWidget{parent}
{
    frenzied=0;
    glacial=0;
    massInjured=0;
    bloodletting=0;
    recovered=0;
    decelerating=0;
}


int AffixLibrary::getFrenziedNumber()
{
    return frenzied;
}
int AffixLibrary::getGlacialNumber()
{
    return glacial;
}
int AffixLibrary::getMassInjuredNumber()
{
    return massInjured;
}
int AffixLibrary::getBloodLettingNumber()
{
    return bloodletting;
}
int AffixLibrary::getRecoveredNumber()
{
    return recovered;
}
int AffixLibrary::getDeceleratingNumber()
{
    return decelerating;
}

QString AffixLibrary::translate(int type)
{
    switch(type)
    {
    case 0:
        return "";
    case 1:
        return "狂暴的";
    case 2:
        return "冰系的";
    case 3:
        return "恢复的";
    case 4:
        return "群伤的";
    case 5:
        return "放血的";
    case 6:
        return "减速的";
    }
    return "";
}

void AffixLibrary::getRandom(int type)
{
    switch(type)
    {
    case 0:
        return;
    case 1:
        addFrenziedNumber();
        break;
    case 2:
        addGlacialNumber();
        break;
    case 3:
        addRecoveredNumber();
        break;
    case 4:
        addMassInjuredNumber();
        break;
    case 5:
        addBloodLettingNumber();
        break;
    case 6:
        addDeceleratingNumber();
        break;
    }
    return;
}

int AffixLibrary::getAffix()
{
    int chance=QRandomGenerator::global()->bounded(60);
    if(chance<10)
    {
        addFrenziedNumber();
        return 1;
    }
    else if(chance<20)
    {
        addGlacialNumber();
        return 2;
    }
    else if(chance<30)
    {
        addMassInjuredNumber();
        return 4;
    }
    else if(chance<40)
    {
        addBloodLettingNumber();
        return 5;
    }
    else if(chance<50)
    {
        addRecoveredNumber();
        return 3;
    }
    else if(chance<60)
    {
        addDeceleratingNumber();
        return 6;
    }
    return 0;

}
void AffixLibrary::addFrenziedNumber()
{
    frenzied++;
    return;
}
void AffixLibrary::addGlacialNumber()
{
    glacial++;
    return;
}
void AffixLibrary::addMassInjuredNumber()
{
    massInjured++;
    return;
}
void AffixLibrary::addBloodLettingNumber()
{
    bloodletting++;
    return;
}
void AffixLibrary::addRecoveredNumber()
{
    recovered++;
    return;
}
void AffixLibrary::addDeceleratingNumber()
{
    decelerating++;
    return;
}
void AffixLibrary::subFrenziedNumber()
{
    frenzied--;
    return;
}
void AffixLibrary::subGlacialNumber()
{
    glacial--;
    return;
}
void AffixLibrary::subMassInjuredNumber()
{
    massInjured--;
    return;
}
void AffixLibrary::subBloodLettingNumber()
{
    bloodletting--;
    return;
}
void AffixLibrary::subRecoveredNumber()
{
    recovered--;
    return;
}
void AffixLibrary::subDeceleratingNumber()
{
    decelerating--;
    return;
}

bool AffixLibrary::haveFrenziedNumber()
{
    if(frenzied>0)
    {
        return true;
    }
    return false;
}
bool AffixLibrary::haveGlacialNumber()
{
    if(glacial>0)
    {
        return true;
    }
    return false;
}
bool AffixLibrary::haveMassInjuredNumber()
{
    if(massInjured>0)
    {
        return true;
    }
    return false;
}
bool AffixLibrary::haveBloodLettingNumber()
{
    if(bloodletting>0)
    {
        return true;
    }
    return false;
}
bool AffixLibrary::haveRecoveredNumber()
{
    if(recovered>0)
    {
        return true;
    }
    return false;
}
bool AffixLibrary::haveDeceleratingNumber()
{
    if(decelerating>0)
    {
        return true;
    }
    return false;
}
