#include "backgroundchromer.h"

BackgroundChromer::BackgroundChromer()
{
    vColorTaskList << BACKGROUND;

    fileName = "background_colors.txt";
}

void BackgroundChromer::saveColors()
{
    Chromer::saveColors(fileName);
}

QPixmap BackgroundChromer::checkAndChromarize(Sheet *sheet)
{
    if (!vColorTaskList.contains(sheet->getBodyPart()))
        return QPixmap();
    else
        return Chromer::chromarize(sheet);
}

void BackgroundChromer::loadColors()
{
    Chromer::loadColors(fileName);
}


void BackgroundChromer::addColor(ColorData &colorData)
{
    Chromer::addColor(fileName, colorData);
}

void BackgroundChromer::resetColors()
{
    Chromer::resetColors(fileName);
}

void BackgroundChromer::checkAndAssignColor(Sheet *sheet)
{
    if (vColorTaskList.contains(sheet->getBodyPart()))
        Chromer::insertColor(sheet);
}

void BackgroundChromer::checkAndReadColor(const Sheet *sheet)
{
    if (vColorTaskList.contains(sheet->getBodyPart())){
        if (Chromer::readColor(sheet))
            return; // color found and selected

        else
            Chromer::addColor(QString("legacyValue"), sheet->getColorData());
    }

}





















