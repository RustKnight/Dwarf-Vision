#include "dfcreature.h"

DfCreature::DfCreature()
{

//    for (int i = 0; i < INVALID_ASSET; ++i){

//        bodyPartsList << QString();
//    }


//    for (int i = 0; i < INVALID_ASSET; ++i){

//        debugPartsList << QStringList();
//    }


//    for (int i = 0; i < INVALID_ASSET; ++i){

//        selectedFilters << QList<QStringList>();
//    }



//    debugPartsList[MOUSTACHE] << QString {"Stubble, Short, Medium, Long"};
//    debugPartsList[MOUSTACHE] << QString {"Unkept, Neatly Combed, Ponytail, Braided, Double Braided"};
//    debugPartsList[MOUSTACHE] << QString {"Curly, Straight"};


//    debugPartsList[BEARD] << QString  {"Stubble, Short, Medium, Long"};
//    debugPartsList[BEARD] << QString  {"Unkept, Neatly Combed, Ponytail, Braided, Double Braided"};
//    debugPartsList[BEARD] << QString  {"Curly, Straight"};


//    debugPartsList[HAIR] << QString  {"Stubble, Short, Medium, Long"};
//    debugPartsList[HAIR] << QString  {"Unkept, Neatly Combed, Ponytail, Braided, Double Braided"};
//    debugPartsList[HAIR] << QString  {"Curly, Straight"};


//    debugPartsList[EYEBROWS] << QString  {"Thin, Thick, Bushy"};
//    debugPartsList[EYEBROWS] << QString  {"Short, Medium, Long"};
//    debugPartsList[EYEBROWS] << QString  {"Low, Normal, High"};


//    debugPartsList[NOSE] << QString  {"Convex, Straight, Concav"};
//    debugPartsList[NOSE] << QString  {"Upturned, Normal, Hooked"};


//    debugPartsList[EYES] << QString  {"Slit, Narrow, Somewhat Narrow, Slightly Round, Round, Very Round, Extremely Round, Incredibly Round"};
//    debugPartsList[EYES] << QString  {"Close, Normal, Wide"};


//    debugPartsList[LIPS] << QString  {"Thin, Normal, Thick"};
//    debugPartsList[LIPS] << QString  {"Short, Medium, Long"};


//    debugPartsList[CHEEKS] << QString {"Low, Normal, High"};

//    //debugPartsList[EARS] << QString {"Low, Normal, High"};


    //simulateDwarf();
}




QList<Sheet> DfCreature::getSheets(AssetDB *assetDb)
{

    QList<Sheet> result;

    for (int i = 0; i < INVALID_ASSET; ++i){

        Ticket ticket;

        ticket.gender       = MALE;
        ticket.bodypart     = BodyPart(i);


        result << assetDb->getSheet(ticket, selectedFilters[i]);
    }

    return result;
}

void DfCreature::setID(int id)
{
    this->id = id;
}

void DfCreature::setName(QString name)
{
    this->name = name;
}

void DfCreature::setWorldName(QString world)
{
    this->world = world;
}

void DfCreature::setSelected(bool truth)
{
    selected = truth;
}

void DfCreature::setBp(const QList<int> &bpList)
{
    bpValuesList = bpList;
}

void DfCreature::checkHairConsistency()
{
    if (hair.contains("clean-shaven"))
        hair = "clean-shaven";

    if (hair.contains("stubble"))
        hair = "stubble";
}

void DfCreature::checkBeardConsistency()
{
    if (beard.contains("clean-shaven"))
        hair = "clean-shaven";

    if (beard.contains("stubble"))
        hair = "stubble";
}

void DfCreature::checkMoustacheConsistency()
{
    if (moustache.contains("clean-shaven"))
        hair = "clean-shaven";

    if (moustache.contains("stubble"))
        hair = "stubble";
}

void DfCreature::setHair(const QString &s)
{
    hair = s;
}

void DfCreature::setBeard(const QString &s)
{
    beard = s;
}

void DfCreature::setMoustache(const QString &s)
{
    moustache = s;
}

void DfCreature::setSideburns(const QString &s)
{
    sideburns = s;
}

void DfCreature::setHairColor(const QString &s)
{
    hairColor = s;
}

void DfCreature::setIrisColor(const QString &s)
{
    irisColor = s;
}

void DfCreature::setGender(bool gender)
{
    male = gender;
}

QString DfCreature::getHair() const
{
    return hair;
}

QString DfCreature::getBeard() const
{
    return beard;
}

QString DfCreature::getMoustache() const
{
    return moustache;
}

QString DfCreature::getSideburns() const
{
    return sideburns;
}

QString DfCreature::getHairColor() const
{
    return hairColor;
}

QString DfCreature::getIrisColor() const
{
    return irisColor;
}

bool DfCreature::isMale()
{
    return male;
}

QList<int> DfCreature::getBpList() const
{
    return bpValuesList;
}

int DfCreature::getID() const
{
    return id;
}

QString DfCreature::getName() const
{
    return name;
}

QString DfCreature::getWorldName() const
{
    return world;
}

bool DfCreature::isSelected() const
{
    return selected;
}

void DfCreature::setAppearance(QList<QList<QStringList>> selectedFilters)
{

    this->selectedFilters = selectedFilters;

}

















































