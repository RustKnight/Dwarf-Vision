#include "portraitmanager.h"

PortraitManager::PortraitManager(QWidget *parent, Colorizer *colorizer) : QGraphicsScene(parent), colorizer {colorizer}
{


    connect(&collection, &Collection::editThisPortrait, this, &PortraitManager::findDrawEditPortrait);

}


void PortraitManager::deleteAndAssignNewSheetsList(QList<Sheet *> pList)
{

    for (Sheet* sh : vGuiSheetList)
        delete sh;

    vGuiSheetList = pList;
}



void PortraitManager::updateSheetList(QList<Sheet *> sheetList)
{


    QMap<BodyPart, int> map;


    if (vGuiSheetList.isEmpty())
        vGuiSheetList = sheetList;


    // if culture AND gender are the same, keep same indexes
    else if (vGuiSheetList[0]->getGender() == sheetList[0]->getGender()){


        // LEGACY CODE
        // NOTE: mapping offsets for GuiSheet might no longer needed

        // keep current frameIndex of sheets + categories
        for (Sheet* sh : vGuiSheetList){

            if (sh->isEmpty()) continue;
            map.insert(sh->getBodyPart(), sh->getFrameIndex());
        }

        // alocate new sheets
        deleteAndAssignNewSheetsList(sheetList);


        // load saved indexes
        // if stored frameIndex of (Ex:) nose is 7, but new sheet has only 4 nose frames, setFrame is ignored
        for (Sheet* sh : vGuiSheetList){

            int mappedIndex = map[sh->getBodyPart()];

            if (mappedIndex < sh->getFrameCount())
                sh->setFrame(mappedIndex);
        }

    }

    else
        deleteAndAssignNewSheetsList (sheetList);



    // INTENDED TO INFORM LIST WHEN NEW CATEGORY IS SELECTED OF INDEX

    // inform myStringList of new index

//    int newIndex;
//    for (int i = 0; i < vGuiSheetList.size(); ++i)
//        if (vGuiSheetList[i]->isSelected())
//              newIndex = vGuiSheetList[i]->getFrameIndex();



    //setGuiSheetsNeckHeight(neckHeight);

    //emit portraitRandomized(newIndex);


    if (!portraitDB.isEmpty())  // refuze draw requests when program initializes (and DB is not yet populated)
        drawPortrait(portraitDB[portraitIndex]);
}



void PortraitManager::setGuiSheetsNeckHeight(int height)
{
    for (Sheet* sh : vGuiSheetList)
        if (sh->getBodyPart() == BASE){
            sh->setChildOffsetAt(0, height);
            break;
        }
}

void PortraitManager::changeNeckHeight(int height)
{

    // avoids unnecessary redraw
    if (portraitDB.isEmpty() || portraitDB[portraitIndex]->getNeckHeight() == height)
        return;

    portraitDB[portraitIndex]->setNeckHeight(height);

    drawPortrait(portraitDB[portraitIndex]);

}

void PortraitManager::toggleColorRandomization(bool truth)
{
    randomizeColor = truth;
}


void PortraitManager::next()
{

    if (portraitIndex < portraitDB.size() - 1)
        portraitIndex += 1;
    else
        return;

    // inform colorizer of what colors this portrait has and display in GUI the appropiate colors
    emit newPortraitSelected(portraitDB[portraitIndex]->getSheets());

    drawPortrait(portraitDB[portraitIndex]);

    // inform widgetList of curretnly selected frame
    for (Sheet* sheet : portraitDB[portraitIndex]->getSheets())
        if (sheet->isSelected())
             emit currentlySelectedFrameIndex(sheet->getFrameIndex());


}

void PortraitManager::previous()
{

    if (portraitIndex > 0)
        portraitIndex -= 1;
    else
        return;

    // inform colorizer of what colors this portrait has and display in GUI the appropiate colors
    emit newPortraitSelected(portraitDB[portraitIndex]->getSheets());

    drawPortrait(portraitDB[portraitIndex]);

    // inform widgetList of curretnly selected frame
    for (Sheet* sheet : portraitDB[portraitIndex]->getSheets())
        if (sheet->isSelected())
             emit currentlySelectedFrameIndex(sheet->getFrameIndex());


}

QPixmap PortraitManager::getPixmapCurrentPortrait()
{
    return portraitToPixmap(portraitDB[portraitIndex], 1);
}

void PortraitManager::updateColorForCurrentPortrait()
{

    for (Sheet* sheet : portraitDB[portraitIndex]->getSheets()){

        colorizer->overwriteColor(sheet);
    }
}

void PortraitManager::changePortraitColor(Portrait *portrait, QString hairColor, QString irisColor)
{
    // ask colorizer to change color of portrait



    // hair: black, eyes: blue
    colorizer->changeIrisColor(portrait->getSheets(), irisColor);
    colorizer->changeHairColor(portrait->getSheets(), hairColor);
}

void PortraitManager::onCategoryChange()
{

    if (portraitDB.isEmpty())
        return;

    for (Sheet* sheet : portraitDB[portraitIndex]->getSheets())
        if (sheet->isSelected())
            emit currentlySelectedFrameIndex(sheet->getFrameIndex());
}

void PortraitManager::addPortraitToCollection()
{
    collection.addPortrait(portraitDB[portraitIndex], portraitToPixmap(portraitDB[portraitIndex], 1));
}

void PortraitManager::showCollection(float portraitSize)
{
    // get portraits
    QList<Portrait*> portraitList = collection.getPortraits();

    if (portraitList.isEmpty()){

        QMessageBox messageBox;
        messageBox.warning(0,"No portraits","No portraits were added into Collection\nAdd some portraits and retry.");
        messageBox.setFixedSize(500,200);


        return;
    }


    // delete current ones
    collection.deleteAllPortraits();

    //update portraits
    for (Portrait* portrait : portraitList)
        collection.addPortrait(portrait, portraitToPixmap(portrait, portraitSize));


    collection.setWindowFlags(Qt::FramelessWindowHint);
    collection.showCollection(true);

}

void PortraitManager::findDrawEditPortrait(Portrait *portrait)
{

    // if portrait exists in our list
    if (portraitDB.contains(portrait)){

        // find its index and place as current
        portraitIndex = portraitDB.indexOf(portrait);
    }

    drawPortrait(portrait);
}

void PortraitManager::saveCurrentPortrait()
{

    QFileDialog fileDialog;

    //QString name = fileDialog.getOpenFileName(nullptr, tr("Open .bin file"), "", tr("Portrait Data Files (*bin)"));
    QString fileName = fileDialog.getSaveFileName(nullptr, tr("Save File"), QCoreApplication::applicationDirPath() + QDir::separator() + "portraits_DATA",
                                                                tr("Portrait Data (*.bin)"));
    if (fileName.isEmpty())
        return;




    QFile newPortraitFile(fileName);
    bool success = newPortraitFile.open(QIODevice::WriteOnly);
    QDataStream stream(&newPortraitFile);

    stream << *portraitDB[portraitIndex];

    newPortraitFile.close();


}

void PortraitManager::loadPortrait()
{

    // check against for data that does not exist (colors / PNGs, or other data) and avoid crashes

    QFileDialog fileDialog;

    QString fileName = fileDialog.getOpenFileName(nullptr, tr("Open .bin file"), QCoreApplication::applicationDirPath() + QDir::separator() + "portraits_DATA", tr("Portrait Data Files (*bin)"));

    if (fileName.isEmpty())
        return;

    QFile newPortraitFile(fileName);

    // check if we're trying to load an already loaded portrait
    QFileInfo fileInfo (newPortraitFile);
    if (loadedFiles.contains(fileInfo.baseName())){

        QMessageBox messageBox;
        messageBox.warning(0,"Error","Portrait(s) already loaded!\nFile name: " + fileInfo.baseName());
        messageBox.setFixedSize(500,200);

        return;
    }


    bool success = newPortraitFile.open(QIODevice::ReadOnly);
    QDataStream stream(&newPortraitFile);

    Portrait* portrait = new Portrait();
    stream >> *portrait;

    DBstorePortrait(portrait);

    newPortraitFile.close();


    loadedFiles << fileInfo.baseName();

    drawPortrait(portrait);


}



void PortraitManager::randomizePortraitSheets()
{
    // select new color
    if (randomizeColor)
        colorizer->randomizeUsedColor();


    // randomize sheet frames
    for (Sheet* sheet : vGuiSheetList){

        if (!sheet->isLocked())
            sheet->randomizeSelectedFrame();

        // insert color in sheets
        colorizer->overwriteAllColors(sheet);
    }

}

void PortraitManager::updateFrame(int SheetIndex, QString newFrame)
{

    // update for our selected portrait
    if (!portraitDB.isEmpty()){  // refuze draw requests when program initializes (and DB is not yet populated)

        portraitDB[portraitIndex]->getSheets()[SheetIndex]->setFrame(newFrame);

//        if (portraitDB[portraitIndex]->getSheets()[SheetIndex]->getBodyPart() == LIPS )
//            checkAndDoBeardStich();

        drawPortrait(portraitDB[portraitIndex]);
    }


    // update for gui list as well
        // if we generate a new portrait, it will lock the frame that it currently has selected from last generation - we want to update that

    vGuiSheetList[SheetIndex]->setFrame(newFrame);

}


void PortraitManager::applyColorizerPointer(Colorizer *colorizer)
{
    this->colorizer = colorizer;

    // pass sheet to colorizer to update shown gui colors
    connect(this, &PortraitManager::newPortraitSelected, colorizer, &Colorizer::readSheetsForColors);

    // give all sheets of current portrait to current chromer to paint new color (that changed)
    connect(colorizer, &Colorizer::selectedColorChanged, this, &PortraitManager::updateColorForCurrentPortrait);
    // ask a redraw
    connect(colorizer, &Colorizer::selectedColorChanged, this, &PortraitManager::RedrawPortrait);
}

void PortraitManager::applyGraphicsView(QGraphicsView *graphicsView)
{
    this->graphicsView = graphicsView;
}


void PortraitManager::drawPortrait(Portrait* portrait)
{
    QElapsedTimer timer;
    timer.start();



    // note: we're effectivly deleting all our data from portrait, loading categories draw order/settings
    // and then rewrite our stored indexes, neck height, color etc. + other possible future data we might store
    // a way to just get interested data out of vSheetList without losing our current portrait data should be a better approach

    this->clear();

    //inform slider of this portrait neck height
    emit neckHeightChanged(portrait->getNeckHeight());

    // [[ if gameMode, don't absorb gui and refresh portrait]]

    // ADD CURRENT DRAW GUI SETTINGS FROM vSheetList
    portrait->absorpGuiSettings(vGuiSheetList);



    // DRAW PORTRAIT
    for (Sheet* sheet : portrait->getSheets()){

        if (!sheet->isHidden()){

            QPixmap pixmap = sheet->getCurrentPixmap();

            if (sheet->isColorable())
                pixmap = colorizer->colorize(sheet);

            QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap);

            item->setTransformationMode(Qt::SmoothTransformation);

            item->setFlag(QGraphicsItem::ItemIsMovable);



            addItem(item);


            // ignore base
            if (sheet->getBodyPart() != BASE)
                item->moveBy(0, sheet->getOffset());

        }
    }



    qDebug() << "Draw function took " << timer.elapsed() << "miliseconds.";


}

QPixmap PortraitManager::portraitToPixmap(Portrait *portrait, float size)
{


    drawPortrait(portrait);


    // RETURNS PIX PORTRAIT FROM WHAT IS CURRENTLY IN GRAPHICS_VIEW

    // get size of graphicsview
    QRect rect = graphicsView->viewport()->rect();

    rect.setWidth(rect.width() + 2);
    rect.setHeight(rect.height() + 2);

    // Create pixmap of same size as graphicsview
    QPixmap pixmap = QPixmap(rect.size());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);

    // Render the graphicsview onto the pixmap and save it out.
    graphicsView->render(&painter, pixmap.rect(), rect);

    // draw selection rect, if selected
    if (portrait->isSelected()){


        QColor color;
        color.setNamedColor("#d5ae36");

        QPen pen(color, 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(pen);

        painter.drawRect(rect);
    }


    QPixmap result = pixmap.scaled(pixmap.width() * size, pixmap.height() *  size, Qt::KeepAspectRatio, Qt::SmoothTransformation);


    return result;
}

void PortraitManager::RedrawPortrait()
{

    // with the new changes done to RedrawPortrait, it is now identical with drawPortrait (almost)
    // might want to redirect callers to drawPortrait and delete RedrawPortrait

    this->clear();


    for (Sheet* sheet : portraitDB[portraitIndex]->getSheets()){

        if (!sheet->isHidden()){

            QPixmap pixmap = sheet->getCurrentPixmap();


            if (sheet->isColorable())
                pixmap = colorizer->colorize(sheet);

            QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap);

            item->setTransformationMode(Qt::SmoothTransformation);

            item->setFlag(QGraphicsItem::ItemIsMovable);

            addItem(item);


            // ignore base
            if (sheet->getBodyPart() != BASE)
                item->moveBy(0, sheet->getOffset());

        }
    }
}



Portrait* PortraitManager::createPortrait()
{

    randomizePortraitSheets();
    return new Portrait (vGuiSheetList);

}



void PortraitManager::drawGallery(int qty, int size)
{

    QList<QPixmap> pixPortraitsList;
    QList<Portrait*> pPortraitsList;

    float fSize = size / 100.0f;

    int max = 10;

    if (qty > max)
        qty = max;



    // create portraits, store them
    for (int i = 0; i < qty; ++i){

        Portrait* portrait = createPortrait();

        // store gallery portraits
        DBstorePortrait ( portrait );

        pPortraitsList   << portrait;
        pixPortraitsList << portraitToPixmap(portrait, fSize);

    }

    // send to collection for display
    collection.showTempCollection(pixPortraitsList, pPortraitsList);




    //    QDialog dialog;
    //    dialog.setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));

    //    // create horizontal layout
    //    QGridLayout layout(&dialog);


    //    int rowTotalPortraits = 5;
    //    int totalPortraits = pixPortraitsList.size();

    //    for (int y = 0, portraitIndex = 0; portraitIndex < totalPortraits; ++y){

    //        for (int x = 0; x < rowTotalPortraits && portraitIndex < totalPortraits; ++x){

    //            QLabel* l = new QLabel(&dialog);
    //            l->setStyleSheet("background-color: rgb(62, 62, 62);");
    //            l->setPixmap(pixPortraitsList[portraitIndex]);
    //            layout.addWidget(l, y, x);
    //            portraitIndex++;
    //        }
    //    }


    //    dialog.exec();
}

void PortraitManager::DBstorePortrait(Portrait * portrait)
{
    portraitDB << portrait;
    portraitIndex = portraitDB.size() - 1;
}

void PortraitManager::onGenerateButtonClicked()
{

    DBstorePortrait ( createPortrait() );
    drawPortrait    (portraitDB[portraitIndex]);

    if (!portraitDB.isEmpty()){
    // inform widgetList of currently selected frame
    for (Sheet* sheet : portraitDB[portraitIndex]->getSheets())
        if (sheet->isSelected())
             emit currentlySelectedFrameIndex(sheet->getFrameIndex());

    }
}

void PortraitManager::checkAndDoBeardStich()
{

    if (portraitDB.isEmpty())
        return;

    // DO NOT DRAW BEARD STICH IF THICK MOUTH
    Sheet* lips;
    Sheet* beardStich;
    Sheet* beard;

    //find lips
    for (Sheet* sh : portraitDB[portraitIndex]->getSheets())
        if (sh->getBodyPart() == LIPS){
            lips = sh;
            break;
        }

    //find beard stich
    for (Sheet* sh : portraitDB[portraitIndex]->getSheets())
        if (sh->getBodyPart() == BEARD_STICH){
            beardStich = sh;
            break;
        }

    //find beard
    for (Sheet* sh : portraitDB[portraitIndex]->getSheets())
        if (sh->getBodyPart() == BEARD){
            beard = sh;
            break;
        }

    if (lips->getFrameName().contains("thick"))     // hide stich
        beardStich->setFrame("empty");
    else
        beard->refreshMirrorFrame();     // tell beard to display stich again


    // code for hiding moustache shadow if no beard

//    // DO NOT DRAW MOUSTACHE SHADOW IF STUBBLE OR EMPTY BEARD
//    Sheet* moustacheB;

//    //find beard moustacheB
//    for (Sheet* sh : portraitDB[portraitIndex]->getSheets())
//        if (sh->getBodyPart() == MOUSTACHE_BACK){
//            moustacheB = sh;
//            break;
//        }

//    if (beard->getFrameName().contains("stubble") || beard->getFrameName().contains("empty"))     // hide stich
//        moustacheB->setFrame("empty");
//    else
//        moustacheB->refreshMirrorFrame();     // tell beard to display stich again
}




























