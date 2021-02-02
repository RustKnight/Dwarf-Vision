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
        messageBox.warning(0,"Error","Could not open message.txt\nBe sure to read usage instructions on Bay 12 (News and Updates link, corner right of app)\nClosing...");
        messageBox.setFixedSize(500,200);

        exit(0);
    }

    QTextStream stream (&message);



    QString allDwarfsString = stream.readAll();
    QStringList dwarfSplitList = allDwarfsString.split("\r\n");

    // first split is world name, remove and read
    QString world = dwarfSplitList.takeFirst().simplified();


    for (QString& dwarfInfo : dwarfSplitList){

        if (dwarfInfo.isEmpty())
            continue;


        QStringList dwarfSplit = dwarfInfo.split("=");
        QList<int> bpValues;

        bool selected = dwarfSplit[IS_SELECTED].toInt();
        int id = dwarfSplit[ID].toInt();
        QString name = dwarfSplit[NAME];
        QString description = dwarfSplit[DESCRIPTION];
        QString curly;
        int curlyRaw = dwarfSplit[CURLINESS].toInt();
        bool isMale = dwarfSplit[GEND].toInt(nullptr, 10);

        QStringList bpStringList   = dwarfSplit[BODYPARTS_VALUES].split(",");
        QStringList hairStylesList = dwarfSplit[HAIRSTYLE].split(";");
        QStringList colorsList     = dwarfSplit[COLORS].split(";");

        for (const QString& str : bpStringList)
            bpValues << str.toInt(nullptr, 10);

        //auto tempResult = parseApperance(description);



        DfCreature creature;

        creature.setSelected(selected);
        creature.setID(id);
        creature.setName(name);
        creature.setWorldName(world);
        creature.setBp(bpValues);
        creature.setGender(isMale);

        //        ; 0 - 76 -> straight
        //        ; 77 - 123 -> NONE
        //        ; 124 - 189 Wavy
        //        ; 190 > curly


        if (curlyRaw <= 76)
            curly = "straight";

        else if (curlyRaw >= 77 && curlyRaw <= 123)
            curly = (QRandomGenerator64::global()->bounded(2)) ? "curly" : "straight";

        else if (curlyRaw >= 124 && curlyRaw <= 189)
            curly = "curly";

         else if (curlyRaw >= 190)
            curly = "curly";


        // set hairstyles
        for (QString s : hairStylesList){

            if (s.isEmpty())
                continue;

            QStringList split = s.split(":");
            int lengthVal = split[1].toInt();

            QString lenghtStr;

            if (split[2] == "clean-shaven"){

                creature.setHair("clean-shaven");
                continue;
            }


            // [0] = hairType, [1] = hair length, [2] = hairStyle
            // length_style_curly/straight

            // TESTED ONLY FOR HAIR, UNSURE FOR MOUSTACHE/BEARD - and even for hair for that matter ... (conflict between gui/gm-unit value read and actual dwarfinfo values)

            // 0   - 4 clean shaven         1
            // 5   - 9 stubble              2
            // 10  - 29 very short          3
            // 30  - 74 short               3
            // 75  - 124 medium-length      4
            // 125 - 149 long               4
            // 150 > very long              5

            if (lengthVal <= 4)
                lenghtStr = "clean-shaven";

            else if (lengthVal >= 5 && lengthVal <= 9)
                lenghtStr = "stubble";

            else if (lengthVal >= 10 && lengthVal <= 29)
                lenghtStr = "stubble";

            else if (lengthVal >= 30 && lengthVal <= 74)
                lenghtStr = "short";

            else if (lengthVal >= 75 && lengthVal <= 124)
                lenghtStr = "medium";

            else if (lengthVal >= 125 && lengthVal <= 149)
                lenghtStr = "long";

            else if (lengthVal >= 150)
                lenghtStr = "long";



            // add lenghtStr first, then Style (curly later in rawconvertor)
            if (split[0] == "HAIR"){

                creature.setHair(lenghtStr + "_" + split[2] + "_" + curly);
                creature.checkHairConsistency();
            }

            if (split[0] == "BEARD"){

                creature.setBeard(lenghtStr + "_" + split[2] + "_" + curly);
                creature.checkBeardConsistency();
            }

            if (split[0] == "MOUSTACHE"){

                // no asset for double braided moustache
                if (split[2] == "doublebraided")
                    split[2] = "braided";

                creature.setMoustache(lenghtStr + "_" + split[2] + "_" + curly);
                creature.checkMoustacheConsistency();
            }

            if (split[0] == "SIDEBURNS")
                creature.setSideburns(lenghtStr + "_" + split[2] + "_" + curly);
        }

        // [0] = hair, [4] = iris, [3] = skin
        creature.setHairColor(colorsList[0].replace("_", " "));
        creature.setIrisColor(colorsList[4].split("_")[2].replace("_", " ")); // split IRIS_EYE_COBALT on "_", access list index 2 for actual color


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

QStringList DfMessageReader::parseApperance(QString text)
{

    QStringList descList = text.split(".");
    for (QString s : descList)
        qDebug() << s << "\n";


    QStringList foundFilters;

    for (int i = 0; i < INVALID_ASSET; ++i){

        foundFilters << QString();
    }


    QStringList subjectList = {"moustache", "beard", "hair", "eyebrows", "nose", "eyes", "lips", "ears", "cheeks"};
    // check if cheeks appear

    QStringList hairKeywords      = {"Stubble", "Short", "Medium", "Long", "Unkept", "Neatly Combed", "Ponytail", "Braided", "Double Braids", "Curly", "Straight"};
    QStringList eyesKeywords      = {"Stubble", "Short", "Medium", "Long", "Unkept", "Neatly Combed", "Ponytail", "Braided", "Double Braids", "Curly", "Straight"};



    QHash<QString, QStringList> hash;
    hash.insert("hair", hairKeywords);
    hash.insert("beard", hairKeywords);
    hash.insert("moustache", hairKeywords);
    hash.insert("eyes", eyesKeywords);
    //hash.insert("nose", hairKeywords);

    // caution when checkin color for hair. It can have a color + a touch of <gray?>
    // color can only be found for hair - no need to check color for rest of hair.

    QStringList sentences = text.split(".");

    for (const QString& sentence : sentences){

        // check sentence for relevant bodypart
        for (const QString& subject : subjectList){
            if (sentence.contains(subject, Qt::CaseInsensitive)){

                // search sentence for one of our keywords
                for (const QString& keyword : hash.value(subject)){
                    if (sentence.contains(keyword, Qt::CaseInsensitive)){

                        // trying to figure out subject -> enum
                        int bpIndex = bodyPartStringList.indexOf(subject);
                        foundFilters[bpIndex] += QString{keyword}.replace(" ", "") + " ";
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
































