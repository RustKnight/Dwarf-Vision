#include "irischromer.h"

IrisChromer::IrisChromer()
{
    vColorTaskList << IRIS;

    fileName = "iris_colors.txt";
}

void IrisChromer::saveColors()
{
    Chromer::saveColors(fileName);
}

QPixmap IrisChromer::checkAndChromarize(Sheet *sheet)
{
    if (!vColorTaskList.contains(sheet->getBodyPart()))
        return QPixmap();
    else
        return Chromer::chromarize(sheet);
}

void IrisChromer::loadColors()
{
    Chromer::loadColors(fileName);
}


void IrisChromer::addColor(ColorData &colorData)
{
    Chromer::addColor(fileName, colorData);
}

void IrisChromer::resetColors()
{
    Chromer::resetColors(fileName);
}

void IrisChromer::checkAndAssignColor(Sheet *sheet)
{
    if (vColorTaskList.contains(sheet->getBodyPart()))
        Chromer::insertColor(sheet);
}




void IrisChromer::checkAndReadColor(const Sheet *sheet)
{
    if (vColorTaskList.contains(sheet->getBodyPart())){
        if (Chromer::readColor(sheet))
            return; // color found and selected

        else
            Chromer::addColor(QString("legacyValue"), sheet->getColorData());
    }

}































