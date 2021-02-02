#include "portrait.h"

Portrait::Portrait()
{
    randomizeNeckHeight();
}

Portrait::Portrait(QList<Sheet *> sheetList)
{

    // create new sheets and store
    for (int i = 0; i < sheetList.size(); ++i){

        vSheets << new Sheet (*sheetList[i]);
        hashCategoryPositions.insert(sheetList[i]->getBodyPart(), i);
    }

    // re-pairs sheets offset and mirrors
    refreshSheetsPointers();

    randomizeNeckHeight();
}


Portrait::~Portrait()
{
    for (Sheet* sh : vSheets)
        delete sh;
}

void Portrait::updateSheets(QList<Sheet *> sheetList)
{
    // check sheetList and vSheets for equal nr. of sheets
    if (sheetList.count() != vSheets.count()){
        QMessageBox messageBox;
        messageBox.warning(0,"Warning","Incoming sheets count is not equal with current Portrait sheet count!");
        messageBox.setFixedSize(500,200);
    }


    for (int i = 0; i < sheetList.size(); ++i)
        *vSheets[i] = *sheetList[i];


    refreshSheetsPointers();
}


void Portrait::refreshSheetsPointers()
{

    auto findMultipleSheetsInList = [&](QList<BodyPart> bpList) {

        QList<Sheet*> returnList;



        for (BodyPart bp : bpList)
            returnList << vSheets[hashCategoryPositions.value(bp)];

        return returnList;
    };



    // OFFSET PAIRING

    QList<Sheet*> offsetChildrens = findMultipleSheetsInList(QList<BodyPart>{ HAIR_MID, EARS, HAIR_BACK, CHEEKS, LIPS, NOSE_BACK, BEARD, BEARD_STICH, BEARD_ITEMS, MOUSTACHE_BACK, MOUSTACHE, MOUSTACHE_ITEMS, EYES, IRIS, NOSE, PUPIL, EYEBROWS, EYEBROWS_B, HAIR, HAIR_ITEMS, });
    Sheet* offsetParentHead = vSheets[hashCategoryPositions.value(HEAD)];
    Sheet* offsetParentBase = vSheets[hashCategoryPositions.value(BASE)];
    //Sheet* offsetClothes = findSheetInList(vSheets, CLOTHES);  keep with 0 offset for now
    //Sheet* offsetClothes = findSheetInList(vSheets, CLOTHES_SECONDARY);  keep with 0 offset for now

    for (Sheet* childrenSheet : offsetChildrens)
        childrenSheet->isOffsetChildOf(offsetParentHead);

    offsetParentHead->isOffsetChildOf(offsetParentBase);
    //offsetClothes->isOffsetChildOf(offsetParentBase);   // issue because when asking offset of base, we're getting the offsetBase + child




    // SHEET PAIRING

    Sheet* nose     = vSheets[hashCategoryPositions.value(NOSE)]; //findSheetInList(vSheets, NOSE);
    Sheet* noseBack = vSheets[hashCategoryPositions.value(NOSE_BACK)];
    nose->isMasterOf(noseBack);


    Sheet* beardStich = vSheets[hashCategoryPositions.value(BEARD_STICH)];
    Sheet* beard      = vSheets[hashCategoryPositions.value(BEARD)];
    beard->isMasterOf(beardStich);


    Sheet* eyebrows     = vSheets[hashCategoryPositions.value(EYEBROWS)];
    Sheet* eyebrowsB    = vSheets[hashCategoryPositions.value(EYEBROWS_B)];
    eyebrows->isMasterOf(eyebrowsB);


    Sheet* beardItems   = vSheets[hashCategoryPositions.value(BEARD_ITEMS)];
    beard->isMasterOf(beardItems);


    Sheet* hair     = vSheets[hashCategoryPositions.value(HAIR)];
    Sheet* hairBack = vSheets[hashCategoryPositions.value(HAIR_BACK)];
    Sheet* hairMid  = vSheets[hashCategoryPositions.value(HAIR_MID)];
    hair->isMasterOf(hairBack);
    hair->isMasterOf(hairMid);


    Sheet* hairItems = vSheets[hashCategoryPositions.value(HAIR_ITEMS)];
    hair->isMasterOf(hairItems);


    Sheet* moustache    = vSheets[hashCategoryPositions.value(MOUSTACHE)];
    Sheet* moustacheI   = vSheets[hashCategoryPositions.value(MOUSTACHE_ITEMS)];
    Sheet* moustacheB   = vSheets[hashCategoryPositions.value(MOUSTACHE_BACK)];
    moustache->isMasterOf(moustacheI);
    moustache->isMasterOf(moustacheB);

    Sheet* eyes  = vSheets[hashCategoryPositions.value(EYES)];
    Sheet* iris  = vSheets[hashCategoryPositions.value(IRIS)];
    Sheet* pupil = vSheets[hashCategoryPositions.value(PUPIL)];
    eyes->isMasterOf(iris);
    eyes->isMasterOf(pupil);


    Sheet* clothes      = vSheets[hashCategoryPositions.value(CLOTHES)];
    Sheet* clothesSec  = vSheets[hashCategoryPositions.value(CLOTHES_SECONDARY)];
    Sheet* clothesAcc   = vSheets[hashCategoryPositions.value(CLOTHES_ACCESSORIES)];
    clothes->isMasterOf(clothesSec);
    clothes->isMasterOf(clothesAcc);

}

QList<Sheet *> Portrait::getSheets()
{
    return vSheets;
}

int Portrait::getNeckHeight()
{

    for (Sheet* sheet : vSheets)
        if (sheet->getBodyPart() == BASE)
            return sheet->getOffset();


// bad
    return -12345;
}

void Portrait::setNeckHeight(int height)
{
    for (Sheet* sh : vSheets)
        if (sh->getBodyPart() == BASE){
            sh->setChildOffsetAt(0, height);
            break;
        }
}

void Portrait::randomizeNeckHeight()
{

    int height = QRandomGenerator::global()->bounded(g_maxNeckHeight, g_minNeckHeight);
    setNeckHeight(height);

}

void Portrait::refreshFrames()
{
    for (int i = 0; i < vSheets.size(); ++i)
        vSheets[i]->refreshFrame();
}

void Portrait::checkAndDoBeardStich()
{
    if (vSheets.isEmpty())
        return;

    // DO NOT DRAW BEARD STICH IF THICK MOUTH
    Sheet* lips = vSheets[hashCategoryPositions.value(LIPS)];
    Sheet* beardStich = vSheets[hashCategoryPositions.value(BEARD_STICH)];
    Sheet* beard = vSheets[hashCategoryPositions.value(BEARD)];


    if (lips->getFrameName().contains("thick"))     // hide stich
        beardStich->setFrame("empty");
    else
        beard->refreshMirrorFrame();     // tell beard to display stich again

}



void Portrait::absorpGuiSettings(QList<Sheet *> sourceList)
{

    if (ignoreGuiAbsorption || isPlaceholder)
        return;

    // rearrange vSheets to match sourceList
    QList<Sheet*> sortedList;
    QHash<BodyPart, int> newHash;  
    int neckHeight = getNeckHeight();

    for (int i = 0; i < sourceList.size(); ++i){

        // find our counterpart sheet of sourceList's
        Sheet* sheet = vSheets[hashCategoryPositions.value(sourceList[i]->getBodyPart())];
        sheet->absorbGuiData(sourceList[i]); // copy GUI data
        sortedList << sheet;
        newHash.insert(sheet->getBodyPart(), i);

    }

    setNeckHeight(neckHeight);

    refreshSheetsPointers();

    refreshFrames();

    checkAndDoBeardStich();

    // overwrite our list with newly sorted list
    vSheets = sortedList;
    hashCategoryPositions.swap(newHash);

}

void Portrait::setIgnoreGuiAbsorption(bool truth)
{
    ignoreGuiAbsorption = truth;
}

void Portrait::absorbDfCreature(const DfCreature &creature, int mode)
{
    RawConvertor rawConv;

    id = creature.getID();
    name = creature.getName();
    world = creature.getWorldName();
    selected = creature.isSelected();

    rawConv.readRawsList(creature.getBpList());
    QList<QString> rawStrList = rawConv.getRawsTranslated();

    if (mode == 0)  // full fabulation, all random
        return;


    else if (mode == 1){ // partial, only hair and color (color handled in mainWindow)

        vSheets[HAIR]->      setFrameViaContains(creature.getHair());
        vSheets[BEARD]->     setFrameViaContains(creature.getBeard());
        vSheets[MOUSTACHE]-> setFrameViaContains(creature.getMoustache());
    }


    else if (mode == 2){

        // checks to see if creature had any strings for beard, hair, moustache
        // if it does not, skip assignment and leave default values

        // meant for cases where we have hair curliness or length, but no description of hair style.
        // known issue: it will let the randomly chosen hair styles on creation, but length will be ignored.

        // if hair style is missing, so will its associated length (general hair length is still present thou, but not taken in account)


        if (!creature.getHair().isEmpty())
            vSheets[HAIR]->      setFrameViaContains(creature.getHair());

        if (!creature.getBeard().isEmpty())
            vSheets[BEARD]->     setFrameViaContains(creature.getBeard());

        if (!creature.getMoustache().isEmpty())
            vSheets[MOUSTACHE]-> setFrameViaContains(creature.getMoustache());

        vSheets[EYES]->     setFrameViaContains(rawStrList[EYES]);
        vSheets[EYEBROWS]-> setFrameViaContains(rawStrList[EYEBROWS]);
        vSheets[NOSE]->     setFrameViaContains(rawStrList[NOSE]);
        vSheets[EARS]->     setFrameViaContains(rawStrList[EARS]);
        vSheets[LIPS]->     setFrameViaContains(rawStrList[LIPS]);
    }


}

int Portrait::getID() const
{
    return id;
}

QString Portrait::getName() const
{
    return name;
}

bool Portrait::isSelected() const
{
    return selected;
}

void Portrait::setSelected(bool truth)
{
    selected = truth;
}

void Portrait::setID(int id)
{
    this->id = id;
}

void Portrait::convertToFemalePlaceholder(QPixmap QSplash)
{
    // get rid of all sheets so far
    for (Sheet* sh : vSheets)
        delete sh;

    vSheets.clear();

    vSheets << new Sheet (FEMALE, BASE);
    vSheets[0]->setPlaceholderFrame(QSplash);

    isPlaceholder = true;
}
































