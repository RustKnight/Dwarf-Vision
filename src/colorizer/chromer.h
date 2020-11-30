#ifndef CHROMER_H
#define CHROMER_H


#include <QList>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QRandomGenerator>

#include "colorizer/colordata.h"
#include "sheet.h"


// ability to have secondary (possibly n times) colors
// derived classes implement a virtual chromarize function
// each class will decide, when recieving a Sheet, based on their internal list, if the sheet will get a primary or secondary color
// and finally call the base class chromarize with a paramater to decide the color
    // either decide internally, what color to use, based on the primary color (red shirts will always have green undershirts, etc)


class Chromer
{
protected:

    Chromer();
    void loadColors(QString fileName);   // INFO: loadColors() apparently no longer needs to return a StringList
    void saveColors(QString fileName);
    void resetColors(QString fileName);
    bool readColor(const Sheet* sheet);



public:
    QPixmap chromarize(Sheet* sheet);
    void randomizeColor();
    void alterCurrentColorDark      (const QColor &dark);
    void alterCurrentColorBase      (const QColor &base);
    void alterCurrentColorHighlight (const QColor &highlight);
    void updateLerp(double lerpVar);
    void setColorIndex(int index);
    void addColor (QString fileName, const ColorData &color);
    void deleteColor();
    ColorData getColorData() const;
    bool isColorLocked() const;
    void setColorLocked(bool truth);
    bool tryChangeColor(QString colorName);


    QColor getDark () const;        // (?) better function: getColor (ColorType::Dark) and switch inside function
    QColor getBase () const;
    QColor getHighlight () const;
    double getLerp() const;
    QStringList getColorStrings() const;
    int getColorIndex() const;
    void insertColor(Sheet* sh);

    virtual void saveColors() = 0;
    virtual QPixmap checkAndChromarize(Sheet *sheet) = 0;
    virtual void loadColors() = 0;
    virtual void addColor(ColorData &color) = 0;
    virtual void resetColors() ;
    virtual void checkAndAssignColor(Sheet* sheet) = 0;
    virtual void checkAndReadColor(const Sheet* sheet) = 0;

protected:
    QList<ColorData> vColors;
    int clrNowIndex = 0;
    bool colorLocked = false;
};

#endif // CHROMER_H
