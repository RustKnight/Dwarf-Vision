#include "sheet.h"




Sheet::Sheet()
{

    pixList << QPixmap();
    strList << "dummy";
    category = INVALID_ASSET;

}


Sheet::Sheet(const Sheet &sourceSheet)
{
    *this = sourceSheet;
}

Sheet &Sheet::operator=(const Sheet &sourceSheet)
{



    //STEP 1: get all state from source
    category             =  sourceSheet.category;
    pixList              =  sourceSheet.pixList;
    strList              =  sourceSheet.strList;
    childrenOffsetsList  =  sourceSheet.childrenOffsetsList;
    selectedFrame        =  sourceSheet.selectedFrame;
    gender               =  sourceSheet.gender;
    frameIndex           =  sourceSheet.frameIndex;
    locked               =  sourceSheet.locked;
    hidden               =  sourceSheet.hidden;
    selected             =  sourceSheet.selected;
    empty                =  sourceSheet.empty;
    isSubordinate        =  sourceSheet.isSubordinate;
    colorable            =  sourceSheet.colorable;
    myOffset             =  sourceSheet.myOffset;
    subordinatesList     =  sourceSheet.subordinatesList;
    parentOffseter       =  sourceSheet.parentOffseter;
    colorData            =  sourceSheet.colorData;


    //STEP 2: delete copied pointer state
    parentOffseter = nullptr;
    subordinatesList.clear();


    //STEP 3: hope user of this function will assign new category-pairing pointers...
    return *this;
}

Sheet::Sheet(Gender gen, BodyPart category)
{

    empty = false;

    this->category = category;

    gender = gen;

    setFrameInternal(0);

}

void Sheet::addDummy(QString message)
{
    addFrame(QPixmap(), message);
}

void Sheet::refreshFrame()
{  
    setFrameInternal(frameIndex);
}


void Sheet::addFrame(QPixmap pix, QString pixName)
{
    pixList << pix;
    strList << pixName;
    childrenOffsetsList << 0;
}

QStringList Sheet::getStrings() const
{

    return strList;

}

QString Sheet::getFrameName(int index)
{
    return strList[index];
}

QString Sheet::getFrameName()
{
    return strList[frameIndex];
}

QString Sheet::getCategoryName() const
{
    return bodyPartStringList[category];
}

QPixmap Sheet::getSpecificFrame(QString str) const
{

    for (int i = 0 ; i < strList.size(); ++i){

        if (strList[i] == str)
            return pixList[i];
    }

    return QPixmap ("NOT FOUND // QPixmap Sheet::getFrame(QString str)");
}

BodyPart Sheet::getBodyPart() const
{
    return category;
}

int Sheet::getFrameCount() const
{
    return pixList.size();
}

void Sheet::setFrame(QString frameName)
{

    for (int i = 0 ; i < strList.size(); ++i){

        if (strList[i] == frameName){

            setFrameInternal(i);
            break;
        }
    }

}

void Sheet::setFrame(int frameIndex)
{
    setFrameInternal(frameIndex);
}


QPixmap Sheet::getCurrentPixmap() const
{
    return selectedFrame;
}


void Sheet::randomizeSelectedFrame()
{

    // if reflection, no random needed. change only via mirrorFrameChange(QString )
    if (isSubordinate)
        return;

    int randomFrame = QRandomGenerator::global()->bounded(pixList.size());

    // do not accept "empty" as a random pick
    while (strList[randomFrame].contains("empty"))
        randomFrame = QRandomGenerator::global()->bounded(pixList.size());

    setFrameInternal(randomFrame);

}

void Sheet::lockFrame(bool truth)
{
    locked = truth;

}

void Sheet::hideFrame(bool truth)
{

    hidden = truth;

}

void Sheet::selectSheet(bool truth)
{
    selected = truth;
}

bool Sheet::isLocked() const
{
    return locked;
}

bool Sheet::isHidden() const
{
    return hidden;
}

bool Sheet::isSelected() const
{
    return selected;
}

bool Sheet::isEmpty() const
{
    return empty;
}

Gender Sheet::getGender() const
{
    return gender;
}

int Sheet::getFrameIndex() const
{
    return frameIndex;
}

void Sheet::setChildOffsetAt(int index, int value)
{
    childrenOffsetsList[index] = value;
}

void Sheet::isMasterOf(Sheet *subordinate)
{

    subordinatesList << subordinate;
    subordinate->isSubordinate = true;
}

void Sheet::isOffsetChildOf(Sheet *parent)
{
    parentOffseter = parent;
}

int Sheet::getOffset()
{

    // if base gets called directly, it will return a wrong value, i.e. the children'sOffset + 0. It should be just 0.

    if (parentOffseter != nullptr)
        myOffset = parentOffseter->getOffset();

    // each particular frame will have different offsets to offer for the calling child
    // values are initialized in MainWindow::syncSheets()

    return myOffset + childrenOffsetsList[frameIndex];

}


void Sheet::setFrameInternal(int frameIndex)
{


    if (pixList.isEmpty()){

        this->frameIndex = frameIndex;
        return;
    }

    // if index is out of bounds with new list
    if (frameIndex >= pixList.size()){
        setFrameInternal(0);
        return;
    }


    this->frameIndex = frameIndex;
    selectedFrame = pixList[frameIndex];


    for (Sheet* subordinate : subordinatesList)
        subordinate->mirrorFrameChange(strList[frameIndex]);

}


void Sheet::mirrorFrameChange(QString frameName)
{

    // if no pairing frame is found, layer should NOT be drawn
    // avoid keeping last drawn layer (Ex: main layer might not have secondary layers, thus secondary layers should be invisible)

    // contains should be changed with == for clarity



    for (const QString& pixNames : strList)
        if (pixNames.contains(frameName)){

            // get index of strName
            int index = strList.indexOf(pixNames);

            // set new frame
            setFrameInternal(index);

            return;
        }



    // failed to find match, resort to empty if present

    for (const QString& pixNames : strList)
        if (pixNames.contains("empty")){

            // get index of strName
            int index = strList.indexOf(pixNames);

            // set new frame
            setFrameInternal(index);

            return;
        }


    // failed to find empty, notify of missing empty.png

    QMessageBox messageBox;
    messageBox.warning(0,"Error","Empty frame not found for " + bodyPartStringList[getBodyPart()]);
    messageBox.setFixedSize(500,200);


}

void Sheet::refreshMirrorFrame()
{
    for (Sheet* subordinate : subordinatesList)
        subordinate->mirrorFrameChange(strList[frameIndex]);
}

void Sheet::refreshMirrorHiddenState()
{
    for (Sheet* subordinate : subordinatesList)
        subordinate->hideFrame(hidden);
}

void Sheet::writeColorData(ColorData colorData)
{
    colorable = true;
    this->colorData = colorData;
}

bool Sheet::isColorable() const
{
    return colorable;
}

ColorData Sheet::getColorData() const
{
    return colorData;
}

void Sheet::absorbGuiData(Sheet *guiSheet)
{

    pixList  = guiSheet->pixList;
    strList  = guiSheet->strList;

    locked   = guiSheet->locked;
    hidden   = guiSheet->hidden;
    selected = guiSheet->selected;


}







































