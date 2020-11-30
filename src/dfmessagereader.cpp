#include "dfmessagereader.h"

DfMessageReader::DfMessageReader()
{

}

bool DfMessageReader::sameAsLastMessage()
{

    if (creatureList == lastCreatureList){

        // check if selection changed. If so, redraw
        for (int i = 0; i < creatureList.size(); ++i){

            if (creatureList[i].isSelected() != lastCreatureList[i].isSelected()){

                lastCreatureList = creatureList;
                return false;
            }
        }

        // same dwarfs, no selection change
        return true;
    }

    else{

        lastCreatureList = creatureList;
        return false;
    }

//    bool allFound = false;

//    for (const DfCreature& last_cre : lastCreatureList){

//        allFound = false;

//        bool thisCreatureFound = false;

//        for (const DfCreature& now_cre : creatureList){

//            if (now_cre.getID() == last_cre.getID())
//                thisCreatureFound = true;
//        }

//        // failed to find now_cre
//        if (!thisCreatureFound){

//            lastCreatureList = creatureList;
//            return false;
//        }

//        allFound = true;
//    }

//    // we managed to find all creatures at this point, return true
//    return allFound;

}






void DfMessageReader::readMessage(QFile& message)
{


    // delete previous creatures
    creatureList.clear();

    // read in current creatures
    if (!message.open(QIODevice::ReadOnly)){

        QMessageBox messageBox;
        messageBox.warning(0,"Error","Could not open message.txt");
        messageBox.setFixedSize(500,200);
    }

    QTextStream stream (&message);



    QString allDwarfsString = stream.readAll();
    QStringList dwarfSplitList = allDwarfsString.split("\n");

    // first split is world name, remove and read
    QString world = dwarfSplitList.takeFirst().simplified();


    for (QString& dwarfInfo : dwarfSplitList){

        if (dwarfInfo.isEmpty())
            continue;


        QStringList dwarfSplit = dwarfInfo.split("=");

        bool selected = dwarfSplit[IS_SELECTED].toInt();
        int id = dwarfSplit[ID].toInt();
        QString name = dwarfSplit[NAME];
        QString description = dwarfSplit[DESCRIPTION];


        DfCreature creature;

        creature.setSelected(selected);
        creature.setID(id);
        creature.setName(name);
        creature.setWorldName(world);

        //QElapsedTimer timer;
        //timer.start();

        //creature.setAppearance(parseApperance(description));
        //qDebug() << "Parsing appearance took " << timer.elapsed();

        creatureList << creature;

    }


    if (creatureList.isEmpty()){

        QMessageBox messageBox;

    }

    message.close();

}

QList<DfCreature> DfMessageReader::getCreatures() const
{
    return creatureList;
}

QList<QList<QStringList> > DfMessageReader::parseApperance(QString text)
{

    QList<QList<QStringList>> foundFilters;

    for (int i = 0; i < INVALID_ASSET; ++i){

        foundFilters << QList<QStringList>();
    }


    QStringList bodyparts = {"moustache", "beard", "hair", "eyebrows", "nose", "eyes", "lips", "ears", "cheeks"};
    // check if cheeks appear

    QStringList hairKeywords      = {"Stubble", "Short", "Medium", "Long", "Unkept", "Neatly Combed", "Ponytail", "Braided", "Double Braided", "Curly", "Straight"};
    QStringList beardKeywords     = {"Stubble", "Short", "Medium", "Long", "Unkept", "Neatly Combed", "Ponytail", "Braided", "Double Braided", "Curly", "Straight"};
    QStringList moustacheKeywords = {"Stubble", "Short", "Medium", "Long", "Unkept", "Neatly Combed", "Ponytail", "Braided", "Double Braided", "Curly", "Straight"};

    QHash<QString, QStringList> hash;
    hash.insert("hair", hairKeywords);
    hash.insert("nose", hairKeywords);

    // caution when checkin color for hair. It can have a color + a touch of <gray?>
    // color can only be found for hair - no need to check color for rest of hair.

    QStringList sentences = text.split(".");

    for (const QString& sentence : sentences){

        // check sentence for relevant bodypart
        for (const QString& bp : bodyparts){
            if (sentence.contains(bp, Qt::CaseInsensitive)){

                // if word matches adjective keyword
                for (const QString& keyword : hash.value(bp)){
                    if (sentence.contains(keyword, Qt::CaseInsensitive)){

                        int bpIndex = bodyPartStringList.indexOf(bp);
                        foundFilters[bpIndex] << QStringList {QString{keyword}.replace(" ", "")};
                    }
                }


            }
        }

                // create StringList, add word to it and add StringList to list
    }

    // what bodypart is the sentence about
    // collect all adjectives in individual lists
    return foundFilters;
    // corelate adjectives ranges to our ranges
}
































