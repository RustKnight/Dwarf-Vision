#include "hairchromer.h"

HairChromer::HairChromer()
{
    vColorTaskList << HAIR;
    vColorTaskList << HAIR_BACK;
    vColorTaskList << HAIR_MID;
    vColorTaskList << EYEBROWS;
    vColorTaskList << BEARD;
    vColorTaskList << MOUSTACHE;
    vColorTaskList << BEARD_STICH;

    fileName = "hair_colors.txt";
}

void HairChromer::saveColors()
{
    Chromer::saveColors(fileName);
}

QPixmap HairChromer::checkAndChromarize(Sheet *sheet)
{
    if (!vColorTaskList.contains(sheet->getBodyPart()))
        return QPixmap();
    else
        return Chromer::chromarize(sheet);
}

void HairChromer::loadColors()
{
    Chromer::loadColors(fileName);
}


void HairChromer::addColor(ColorData &colorData)
{
    // function can be now directly called from base class;

    Chromer::addColor(fileName, colorData);
}

void HairChromer::resetColors()
{
    Chromer::resetColors(fileName);
}

void HairChromer::checkAndAssignColor(Sheet *sheet)
{
    if (vColorTaskList.contains(sheet->getBodyPart()))
        Chromer::insertColor(sheet);
}




void HairChromer::checkAndReadColor(const Sheet *sheet)
{
    if (vColorTaskList.contains(sheet->getBodyPart())){
        if (Chromer::readColor(sheet))
            return; // color found and selected

        else
            Chromer::addColor(QString("legacyValue"), sheet->getColorData());
    }

}






























