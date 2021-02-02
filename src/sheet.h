#ifndef SHEET_H
#define SHEET_H

#include <QString>
#include <QPixmap>
#include <QCoreApplication>
#include <QDebug>
#include "QRandomGenerator"
#include <QDataStream>

#include "Ticket.h"
#include "colorizer/colordata.h"




class Sheet
{

public:

    Sheet();

    friend QDataStream &operator << (QDataStream & out, const Sheet& entry) {

        out << entry.category;
        out << entry.childrenOffsetsList;
        out << entry.gender;
        out << entry.frameIndex;
        out << entry.locked;
        out << entry.hidden;
        out << entry.selected;
        out << entry.empty;
        out << entry.isSubordinate;
        out << entry.colorable;
        out << entry.myOffset;
        out << entry.colorData;


        return out;
    }
    friend QDataStream &operator >> (QDataStream & in, Sheet& entry) {

        in >> entry.category;
        in >> entry.childrenOffsetsList;
        in >> entry.gender;
        in >> entry.frameIndex;
        in >> entry.locked;
        in >> entry.hidden;
        in >> entry.selected;
        in >> entry.empty;
        in >> entry.isSubordinate;
        in >> entry.colorable;
        in >> entry.myOffset;
        in >> entry.colorData;


        return in;
    }

    Sheet(const Sheet& sourceSheet);
    Sheet& operator=(const Sheet& sourceSheet);

    Sheet(Gender gen, BodyPart category);


    void addFrame(QPixmap pix, QString pixName);

    QStringList getStrings() const;

    QString getFrameName (int index);
    QString getFrameName ();
    QString getCategoryName() const;

    QPixmap getSpecificFrame(QString str) const;

    BodyPart getBodyPart() const;

    int  getFrameCount() const;
    void setFrame(QString frameName);
    void setFrame(int frameIndex);
    void setFrameViaContains(QString frameIndex);   //used by Portrait when translating RawStrings to PNGs  //might not be needed

    QPixmap getCurrentPixmap() const;
    void setPlaceholderFrame(QPixmap placeholder);

    void randomizeSelectedFrame();

    void lockFrame(bool truth);
    void hideFrame(bool truth);
    void selectSheet(bool truth);

    bool isLocked()     const;
    bool isHidden()     const;
    bool isSelected()   const;
    bool isEmpty()      const;

    Gender getGender()  const;
    int  getFrameIndex() const;
    void setChildOffsetAt (int index, int value);

    void isMasterOf(Sheet* subordinate);
    void isOffsetChildOf(Sheet* parent);
    int  getOffset();

    void addDummy(QString message);
    void refreshFrame();
    void refreshMirrorFrame();
    void refreshMirrorHiddenState();

    void writeColorData(ColorData colorData);
    bool isColorable() const;
    ColorData getColorData() const;

    void absorbGuiData(Sheet* guiSheet);


private:

    void setFrameInternal(int frameIndex);
    void mirrorFrameChange(QString frameName);


private:

    BodyPart        category;
    QList <QPixmap> pixList;    //
    QList <QString> strList;    //
    QList <int>     childrenOffsetsList;
    QPixmap         selectedFrame;

    Gender          gender = INVALID_GENDER;
    int             frameIndex;

    bool            locked = false;     //
    bool            hidden = false;     //
    bool            selected = false;   //
    bool            empty = true;
    bool            isSubordinate = false;
    bool            colorable = false;

    int             myOffset    = 0;

    QList<Sheet*>   subordinatesList;
    Sheet*          parentOffseter = nullptr;

    ColorData       colorData;
};



#endif // SHEET_H














