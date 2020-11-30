#include "dfcreature.h"

DfCreature::DfCreature() : debugPartsList {}
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



void DfCreature::simulateDwarf()
{


    // using same  index as for debugPartsList, write to debugPartsList[INDEX][0] the mergedList


    // go through each index of debugPartsList
    for (int i = 0; i < debugPartsList.size(); ++i){

        if (debugPartsList[i].isEmpty())
            continue;

        // for each stringList, pick one element and place it in mergedList (also, no space)
        for (QString adjective : debugPartsList[i]){

            QStringList split = adjective.split(",");
            QString pickedAdj = split[QRandomGenerator::global()->bounded(split.size())];
            pickedAdj.replace(" ", "");

            QStringList list;
            list << pickedAdj;
            selectedFilters[i] << list;
            // each split needs to have NO white space -> must fuze

        }


    }

    // build sheet list by going through each bodypart enum and request from db
    // if empty, request sheet without any filters
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

















































