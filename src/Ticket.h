#ifndef TICKET_H
#define TICKET_H

#include <QList>
#include <QMessageBox>
#include "globals.h"



enum stringDex {RACE, GENDER, BODYPART};    // Index after splitting pngName ... // ex: dwarf_male_eyes

// WHEN ADDING NEW BodyParts,
// go to MainWindow::syncSheets(QList<Sheet *> pSheetsList) and add sync it to BODY
// avoid having a single asset in a category with name empty (randmize frame from Sheet will keep on randomizing)

enum BodyPart    { BACKGROUND,
                   BASE,
                   HAIR_BACK,
                   CLOTHES_SECONDARY,
                   CLOTHES_ACCESSORIES,
                   CLOTHES,
                   HEAD,
                   HAIR_MID,
                   EARS,
                   CHEEKS,
                   LIPS,
                   NOSE_BACK,
                   BEARD,
                   BEARD_STICH,
                   BEARD_ITEMS,
                   MOUSTACHE_BACK,
                   MOUSTACHE,
                   MOUSTACHE_ITEMS,
                   EYES,
                   IRIS,
                   PUPIL,
                   NOSE,
                   EYEBROWS_B,
                   EYEBROWS,
                   HAIR,
                   HAIR_ITEMS,
                   INVALID_ASSET};

enum Gender      { MALE, FEMALE, INVALID_GENDER};

// QStackedWidget pages are coupled with button names: they must match
const QList<QString> bodyPartStringList {"background",
                                         "base",
                                         "hairBack",
                                         "clothesBack",
                                         "clothesAcc",
                                         "clothes",
                                         "head",
                                         "hairMid",
                                         "ears",
                                         "cheeks",
                                         "lips",
                                         "noseBack",
                                         "beard",
                                         "beardStich",
                                         "beardItems",
                                         "moustacheB",
                                         "moustache",
                                         "moustacheI",
                                         "eyes",
                                         "iris",
                                         "pupil",
                                         "nose",
                                         "eyebrowsB",
                                         "eyebrows",
                                         "hair",
                                         "hairItems" };



struct Ticket {


    Ticket() = default;

    Ticket (QString pngName) {


        // find BodyPart
        QStringList stringSplit = pngName.split("_");

        if (pngNameIsInvalid(stringSplit)){

            invalid = true;

            QMessageBox messageBox;
            messageBox.warning(0,"Error","Unknown Asset Present!\nFile name: " + pngName);
            messageBox.setFixedSize(500,200);

            QMessageBox::StandardButton reply;
            reply = messageBox.question(nullptr, "", "Ignore and continue?",
                                            QMessageBox::Yes|QMessageBox::No);

            if (reply == QMessageBox::No)
                exit(0);

            return;
        }


        for (int i = 0; i < bodyPartStringList.size(); ++i)

            if (bodyPartStringList[i] == stringSplit[BODYPART]){

                bodypart = (BodyPart)i;
                break;
            }


        // find Gender
        if (pngName.contains("female"))
            gender = FEMALE;
        else
            gender = MALE;


    }

    bool isInvalid(){return invalid;}

    void loadAssetEnum(QString buttonStr) {

        for (int i = 0 ; i < bodyPartStringList.size(); ++i)
            if (buttonStr.compare(bodyPartStringList[i], Qt::CaseInsensitive) == 0){
                bodypart = (BodyPart)i;
                break;
            }

    }

    bool pngNameIsInvalid(const QStringList& list) {

        if (list.isEmpty()) return true;

        if (!g_racesStrList.contains(list[RACE]))
            return true;

        if (!bodyPartStringList.contains(list[BODYPART]))
            return true;

        return false;
    }

    BodyPart bodypart = INVALID_ASSET;
    Gender      gender    = INVALID_GENDER;
    int         cultureIndex = 0;
    QList<QString> BodyPartArguments;
    bool invalid = false;

};




#endif // TICKET_H
