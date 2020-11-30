#include "clotheschromer.h"


ClothesChromer::ClothesChromer()
{
    vColorTaskList << CLOTHES;

    fileName = "clothes_colors.txt";
}

void ClothesChromer::saveColors()
{
    Chromer::saveColors(fileName);
}

QPixmap ClothesChromer::checkAndChromarize(Sheet *sheet)
{
    if (!vColorTaskList.contains(sheet->getBodyPart()))
        return QPixmap();
    else
        return Chromer::chromarize(sheet);
}

void ClothesChromer::loadColors()
{
    Chromer::loadColors(fileName);
}


void ClothesChromer::addColor(ColorData &colorData)
{
    // function can be now directly called from base class;

    colorData.dark = QColor {24, 15, 7};
    colorData.base = QColor {182, 18, 35};
    colorData.highlight = QColor {228, 207, 138};

    Chromer::addColor(fileName, colorData);
}

void ClothesChromer::resetColors()
{
    Chromer::resetColors(fileName);
}


void ClothesChromer::checkAndAssignColor(Sheet *sheet)
{
    if (vColorTaskList.contains(sheet->getBodyPart()))
        Chromer::insertColor(sheet);
}



void ClothesChromer::checkAndReadColor(const Sheet *sheet)
{
    if (vColorTaskList.contains(sheet->getBodyPart())){
        if (Chromer::readColor(sheet))
            return; // color found and selected

        else
            Chromer::addColor(QString("legacyValue"), sheet->getColorData());
    }

}







ClothesSecChromer::ClothesSecChromer()
{
    vColorTaskList << CLOTHES_SECONDARY;

    fileName = "clothes_colors_secondary.txt";
}

void ClothesSecChromer::saveColors()
{
    Chromer::saveColors(fileName);
}

QPixmap ClothesSecChromer::checkAndChromarize(Sheet *sheet)
{
    if (!vColorTaskList.contains(sheet->getBodyPart()))
        return QPixmap();
    else
        return Chromer::chromarize(sheet);
}

void ClothesSecChromer::loadColors()
{
    Chromer::loadColors(fileName);
}


void ClothesSecChromer::addColor(ColorData &colorData)
{
    // function can be now directly called from base class;
    // good because DRY across all other derived classes is respected

    Chromer::addColor(fileName, colorData);
}

void ClothesSecChromer::resetColors()
{
    Chromer::resetColors(fileName);
}


void ClothesSecChromer::checkAndAssignColor(Sheet *sheet)
{
    if (vColorTaskList.contains(sheet->getBodyPart()))
        Chromer::insertColor(sheet);
}



void ClothesSecChromer::checkAndReadColor(const Sheet *sheet)
{
    if (vColorTaskList.contains(sheet->getBodyPart())){
        if (Chromer::readColor(sheet))
            return; // color found and selected

        else
            Chromer::addColor(QString("legacyValue"), sheet->getColorData());
    }

}















