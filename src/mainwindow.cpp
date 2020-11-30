#include "mainwindow.h"
#include "ui_mainwindow.h"


// Error handling for:

// DONE invalid file name format              (only checks if first split is Race)
// DONE no assets for a certain category      (displays messages in ListWidget)
// DONE no assets with currently used filters (displays messages in ListWidget)



//Categories Manager loads pngs and stores them as <class>sheets. These get updated as GUI options are ticked (hide/lock)
    // when a new Sheet is allocted to a Category, if Hide is ticked, Category will change the state of the newly allocated Sheet to "isHidden"
    // when GUI raceSelector changes, it needs to notify CatManager and hand over new sheets to Categories

//Portrait Manager. The order of arival is the current order of categories arangement. A category order change/hide/lock is considered a new portraitRequest.


// WidgetList gets the sheet of currently selected Category from CatMan(when new categ. is selected)
// and is sent from PortraitMan the currently used frameIndex whenever it changes
// A change in in frame selection from list is communicated back to Portrait Manager via CatMan and WidgetList (see onListWidgetFrameChange() SLOT from mainWindow)


// Un-Feature

// back/mid layers can be changed independently from primary layers, but will reset upon category selection, due to sheetList update.
// update occurs on category change because PortraitManager needs to know which category is now selected.
// this involves a lot of "work" for just a simple bool toggle in one of the sheets, AND unintendently changes the frame in secondary frames
// (because primary frames get re-selected and causes reflections to change as well)




// IN PROGRESS: gui sync needs more work

// GUI filters need to be synced when some certain sheets are synced (Ex: back hair with front hair)
    // possibly keep an enum pari-list of bodyParts and see which one should be linked
        // then cascade changes to GUI and Sheets
    // OBS: maybe the client of syncronize() sheets should be mainWindow. That way sheet lists arrive at Portrait Manager already synced.
            // also sheet syncing process can be declared in the same place as GUI syncing, in one function (of mainWindow)

// FOR ABOVE

    // have some sort of global list that shows what bodypart/category should be paired with who
    // then GUI reads this and connects appropiate Widgets so that they keep FILTERS in sync
    // Sheets will get connected accordingly as well
        // if pairing sheet does not find that certain frame, it should be flagged as invisible (no-draw)

// make current sys. read the global parining list




// tree offsets frames
    // neck height -> head
        // head shape -> eyes, mouth, nose, hair, etc
            // mouth -> beard



// icons for category buttons - for fast visual reading of what they are


// [DONE] COLORING

// randomize between colors from a list that's loaded upon program launch (better a simple text)
// click on a button (add new color) and have 3 sliders for base, dark, highlight. When the right color, click save and store it in the above list
// list should also have names for color data

// DONE Needs dynamic GUI control over: - lerp function (  color2 = lerpLamb (color2, highlight, clampLamb ((currentPixelCol.green() - 128) * HERE 3.5 HERE));) ,
                               //  - base, dark, highlight

// DONE add DF colors


//DONE BUT if ViewPort becomes too small for Image, resize Image --- always center image
    //BUT: graphics aren't really shown in the middle of the screen (some long hair is left outside)


//DONE volatile portrait gallery
    // ask how many portraits we want
    // pulse and store resulting list x needed portraits
    // after job is done, for each list open a dialgue displaying each portrait

// DONE one to many syncro

// DONE draw stich only for LIPS frame thin and normal. Must check sheet when it hits thin or normal and THEN draw an empty beardStich

//DONE make invisible some categories buttons, but keep them internally and controlobale via code

//DONE hiding master sheet needs to hide rest of subordinates as well


//DONE CategoriesManager Widget size reFix

//DONE MOUSE moves after clicked widget

//DONE Expand colorizer to accomodate tracking/saving/modfying N types of Bodyparts



//Temp list of generated characters per session
    // DONE can go backward/forward to previously generated portraits

    // DONE can export portrait as png & can save portrait data for later reEdit
        // ->> uses DB and Main GUI/PorMan

    // needs to match an ID with a portrait so that we don't RE-generate a portrait everytime the player or other entity needs to be displayed
        // ->> uses DB


    // random gen. names?



//  DONE unify draw methods i.e make galery use main drawPortrait()
    //DONE gallery portraits neck height bug



// DONE hide secondary layers
// DONE while hidden, only relevant MAIN/MASTER layers will be shown



// DONE have a seperate dialog window that shows what filters are currently in use
    // DONE icons for listItems
    // DONE clicking on them switches to that category
    // DONE allow combination of multiple radioFilters? (switch to checkboxes)
    // DONE all should inverse values of buttons
    // DONE change Label color for active/inactive filters

    // TODO: will not work correctly if "All" filter button is used.
    // maybe have a global filter reset

// DONE After loading assets, a check is done to see if loaded assets names match existing filters


// DONE (Seems fixed now) check random - it does not seem as random as it should


// DONE reFix moveMouse when clicking to reoder categories (not centered)
    // note: solution varies with nr of shown categories


// DONE add a background layer and control what color background we can have



// DONE lock color - checkbox in colorizer -> read state on chromarizer selection and have it on off depending on the state of each chromarizer


// DONE slider for neck height in realtime between min and max (just right side of the portrait, vertical slider)

// DONE if component is locked and then generate is clicked, that component is not drawn
    // -> I suspect it has something to do with the absorbsion of new framesList and then the index is looked on 0?....
    // I thought it was because I setting frameIndex directly from Sheet, without going through setFrameInternal ... but it's not that
    // the reason why it's not drawn is because the resulting pixmap is invalid

    // PARTIAL, if going back to to other portrait, guiListSheet does not read in those frames, and will lock it's CURRENT frames (from last portrait generation)
        // gui sheets should just read the frames -

// DONE when saving portraits (PNG or DATA) or anything file related, replace all qDialogInput with a modal save window (think about the default folder/location)




// DONE fulscreen should not reveal chopped limbs - neither fullscreen gallery

// DONE stop gallery to generate XXXXX (too) many portraits (avoid PC locks and generate portraits that are not seen)

// pixmap / png files get a resolution cut from drawPortrait()

// game mode
    // have a screen that only displays portraits
        // -> uses DB
        // -> uses save sessions
        // layout would depend based on adventure/fortress mode


// Chromer::chromarize is a bit laggy -> maybe QT lerping could be better?


// DONE make secondary window minimize/maximize along with main
    // & other behaviour that needs to be synced

// DONE Expand and compress collection correctly
// DONE Selection rect around focused creature (and bypass sameAsLastMessage)
// DONE Get creature info but use normal portrait generation routine
// DONE Move Collection view in bottom left corner of the Window
// DONE DrawPortrait method with a paramater for PNG size output
// DONE Make collection with a fixed width size and decrease/increase size of portraits depending on how many there are


// GLOBALS DECLARATION

QStringList g_racesStrList;
QSize g_assetSize;
int g_maxNeckHeight;
int g_minNeckHeight;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , filterController {new FilterController(this)}
    //, collection(this)
    , watcher(this)

{
    ui->setupUi(this);

    //setWindowFlags(Qt::WindowStaysOnTopHint);

    // GLOBALS INITIALIZATION

    g_racesStrList = assetDB.getRacesList();

    g_assetSize = {357, 465};

    // max value must not be higher than min
    g_minNeckHeight = 45;   // higher values makes neck shorter  - default 45
    g_maxNeckHeight = 10;   // lower values makes neck longer    - default 20


    //random.seed(624);
    //random = random.securelySeeded();  was used by QRandomGenerator random, data member
    QRandomGenerator::global()->securelySeeded(); // is it needed for global randGenerator?


    portraitManager.applyColorizerPointer(ui->colorizer);
    portraitManager.applyGraphicsView(ui->graphicsView);

    createFolders();

    ui->culture_comboBox->addItems(assetDB.getRacesList());

    QAction *generateAction = new QAction(this);
    generateAction->setShortcut(Qt::Key_G);
    connect(generateAction, &QAction::triggered, this, &MainWindow::onGenerateClick);
    this->addAction(generateAction);

    //QAction *triggerMessage = new QAction(this);
    //triggerMessage->setShortcut(Qt::Key_Q);
    //connect(triggerMessage, &QAction::triggered, this, &MainWindow::onDwarfFortressSignal_DEBUG);
    //this->addAction(triggerMessage);


    // QWATCHER for when the .txt file is modified by DF
    // currently disabled - unreliable message sending duration from OS, fluctuates. If you hover mouse over app, back and forth, it seems to be more responsive.

    QDir dir (QCoreApplication::applicationDirPath());
    dir.cdUp();     // we assume the program is in his own folder. +1 up is DF folder.
    dir.cd("temp");
    messageTxtPath = dir.path() + QDir::separator() + "message.txt";
    //watcher.addPath(messageTxtPath);
    //QObject::connect(&watcher, &QFileSystemWatcher::fileChanged, this, &MainWindow::onDwarfFortressSignal);

    myFileWatcher = new MyFileWatcher(messageTxtPath);
    connect(myFileWatcher, SIGNAL (readDfMessage()), this, SLOT(onDwarfFortressSignal()));

    connect(filterController, QOverload<QWidget*>::of(&FilterController::filterButtonClicked), this, &MainWindow::onFilterClick);
    connect(filterController, &FilterController::filterButtonClickedAll, this, &MainWindow::onAllCheckBoxClick);


    connect(ui->culture_comboBox,   &QComboBox::textActivated,
            this,                   &MainWindow::onCultureChange);

    connect(ui->radioButtonMale,    &QRadioButton::clicked,
            this,                   &MainWindow::onGenderToggle);

    connect(ui->radioButtonFemale,  &QRadioButton::clicked,
            this,                   &MainWindow::onGenderToggle);

    connect(ui->categoriesManager,  &CategoriesManager::newCategorySelected,
            ui->listWidget,         &MyStringList::updateStringList);

    connect(ui->categoriesManager,  &CategoriesManager::newCategorySelected,
            filterController,      &FilterController::onCategoryChange); //change filter page to match category

    connect(ui->categoriesManager,  &CategoriesManager::newCategorySelected,
            &portraitManager,       &PortraitManager::onCategoryChange);

    connect(&portraitManager,       &PortraitManager::currentlySelectedFrameIndex,
            ui->listWidget,         &MyStringList::changeStrIndex);

    connect(ui->listWidget,         &MyStringList::currentTextChanged,
            this,                   &MainWindow::onListWidgetFrameChange);

    connect(ui->btnGenerate,        &QPushButton::clicked,
            &portraitManager,       &PortraitManager::onGenerateButtonClicked);

    connect(ui->galleryButton,      &QPushButton::clicked,
            this,                   &MainWindow::generateGallery);

    connect(ui->categoriesManager,  &CategoriesManager::sheetsChangedState,
            this,                   &MainWindow::prepareSheetsAndPassToPortraitManager);

    connect(&portraitManager,       &PortraitManager::portraitRandomized,
            ui->listWidget,         &MyStringList::changeStrIndex);

    connect(ui->btn_randomizeColor, &QCheckBox::toggled,
            &portraitManager,       &PortraitManager::toggleColorRandomization);

    connect(filterController,      &FilterController::filterListWidgetItemClicked,
            this,                   &MainWindow::onFilterListClick);

    connect(ui->btn_next,           &QPushButton::clicked,
            this,                   &MainWindow::onNextPortrait);

    connect(ui->btn_previous,       &QPushButton::clicked,
            this,                   &MainWindow::onPreviousPortrait);

    connect(ui->btn_savePNG,        &QPushButton::clicked,
            this,                   &MainWindow::savePNG);

    connect(&portraitManager,       &PortraitManager::neckHeightChanged,
            this,                   &MainWindow::onCodeSideNeckHeightChange);

    connect(ui->slider_neckHeight,  &QSlider::valueChanged,
            &portraitManager,       &PortraitManager::changeNeckHeight);

    connect(ui->btn_addToCollection,&QPushButton::clicked,
            &portraitManager,       &PortraitManager::addPortraitToCollection);

    connect(ui->btn_showCollection, &QPushButton::clicked,
            this,                   &MainWindow::drawCollection);

    connect(ui->btn_savePortraitData,&QPushButton::clicked,
            &portraitManager,       &PortraitManager::saveCurrentPortrait);

    connect(ui->btn_LoadPortrait,   &QPushButton::clicked,
            &portraitManager,       &PortraitManager::loadPortrait);

    connect(&collection,            &Collection::needSizeChange,
            this,                   &MainWindow::changeGameMaxPortraitSize);

    connect(ui->changeMode,         &QPushButton::clicked,
            this,                   &MainWindow::onGameChangeClick);


    assetDB.loadResources();

    assetDB.checkAssetsNameCorrectness(filterController->getAllFiltersNoSpace());

    ui->radioButtonMale->click();


    ui->graphicsView->setScene(&portraitManager);

    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    ui->radioButtonFemale->setDisabled(true);



    // cannot seem to able to control sensitivity ...

    //mouseDevice.setSensitivity(0.01);
    //auto x = mouseDevice.sensitivity();


    // DEBUG
    //onDwarfFortressSignal();

}


void MainWindow::init()
{

    setWindowTitle ( "Dwarf Vision" );

    ui->chk_box_hideSubordinates->hide();

    ui->categoriesManager->clickCategory(BASE);
    this->resize(1,1);
    ui->btnGenerate->click();
    ui->chk_box_hideSubordinates->click();
    ui->btn_randomizeColor->click();

    ui->slider_neckHeight->setMinimum(g_maxNeckHeight);
    ui->slider_neckHeight->setMaximum(g_minNeckHeight);
    ui->slider_neckHeight->setInvertedAppearance(true);

    this->move(500,150);

    filterController->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    filterController->move(this->pos() - QPoint {filterController->frameSize().width() + 15, 0});

    filterController->setAttribute(Qt::WA_DeleteOnClose);
    filterController->show();

    this->setAttribute( Qt::WA_DeleteOnClose, true );

    this->activateWindow();

}


MainWindow::~MainWindow()
{

    delete ui;
    delete filterController;
    exit(0);    // something prevents filterController dialog to properly close after mainWindow is closed

}

Ticket MainWindow::getTicket() const
{
    Ticket ticket;
    ticket.gender       = (ui->radioButtonMale)->isChecked() ? ticket.gender = MALE : ticket.gender = FEMALE;
    ticket.cultureIndex = ui->culture_comboBox->currentIndex();
    ticket.bodypart    = (BodyPart)bodyPartStringList.indexOf (ui->categoriesManager->getSelectedCategoryName());

    return ticket;
}



void MainWindow::onPreviousPortrait()
{
    portraitManager.previous();
}

void MainWindow::onNextPortrait()
{
    portraitManager.next();
}

void MainWindow::onGenerateClick()
{
    ui->btnGenerate->click();
}

void MainWindow::onGenderToggle()
{

    ui->categoriesManager->updateSheets(assetDB.getAllSheets(getTicket()));
}

void MainWindow::onListWidgetFrameChange(const QString& frameStr)
{

    portraitManager.updateFrame(ui->categoriesManager->indexOfSelectedSheet(), frameStr);
}

void MainWindow::onCultureChange(const QString &culture)
{

    ui->categoriesManager->updateSheets(assetDB.getAllSheets(getTicket()));

}





void MainWindow::generateGallery()
{
    int qty = ui->portrait_quantity_indicator->value();

    portraitManager.drawGallery(qty, ui->portraitCollection_size->value());


}

void MainWindow::prepareSheetsAndPassToPortraitManager(const QList<Sheet>& sheetList)
{

    // sheets are new-ed here
    QList<Sheet*> pList;

    for (const Sheet& sh : sheetList)
        pList << new Sheet(sh);


    // sync sheets
    syncSheets(pList);

    // pass to PM
    portraitManager.updateSheetList(pList);

}

void MainWindow::syncSheets(QList<Sheet *> pSheetsList)
{

    // OFFSET PAIRING
    // all bodyparts are children of HEAD, HEAD is child of BASE


    // WARNING !!!! function must match refreshSheetsPointers() from Portrait.h

    //~~~rip~~~ DRY

    QList<Sheet*> offsetChildrens = findMultipleSheetsInList(pSheetsList, QList<BodyPart>{ BACKGROUND, HAIR_MID, EARS, HAIR_BACK, CHEEKS, LIPS, NOSE_BACK, BEARD, BEARD_STICH, BEARD_ITEMS, MOUSTACHE_BACK, MOUSTACHE, MOUSTACHE_ITEMS, EYES, IRIS, NOSE, PUPIL, EYEBROWS, EYEBROWS_B, HAIR, HAIR_ITEMS, });
    Sheet* offsetParentHead = findSheetInList(pSheetsList, HEAD);
    Sheet* offsetParentBase = findSheetInList(pSheetsList, BASE);
    //Sheet* offsetClothes = findSheetInList(pSheetsList, CLOTHES);  keep with 0 offset for now
    //Sheet* offsetClothes = findSheetInList(pSheetsList, CLOTHES_SECONDARY);  keep with 0 offset for now




    for (Sheet* childrenSheet : offsetChildrens)
        childrenSheet->isOffsetChildOf(offsetParentHead);

    offsetParentHead->isOffsetChildOf(offsetParentBase);
    //offsetClothes->isOffsetChildOf(offsetParentBase);   // issue because when asking offset of base, we're getting the offsetBase + child


    // SHEET PAIRING

    Sheet* nose     = findSheetInList(pSheetsList, NOSE);
    Sheet* noseBack = findSheetInList(pSheetsList, NOSE_BACK);
    nose->isMasterOf(noseBack);


    Sheet* beardStich = findSheetInList(pSheetsList, BEARD_STICH);
    Sheet* beard      = findSheetInList(pSheetsList, BEARD);
    beard->isMasterOf(beardStich);


    Sheet* eyebrows     = findSheetInList(pSheetsList, EYEBROWS);
    Sheet* eyebrowsB    = findSheetInList(pSheetsList, EYEBROWS_B);
    eyebrows->isMasterOf(eyebrowsB);


    Sheet* beardItems   = findSheetInList(pSheetsList, BEARD_ITEMS);
    beard->isMasterOf(beardItems);


    Sheet* hair     = findSheetInList(pSheetsList, HAIR);
    Sheet* hairBack = findSheetInList(pSheetsList, HAIR_BACK);
    Sheet* hairMid  = findSheetInList(pSheetsList, HAIR_MID);
    hair->isMasterOf(hairBack);
    hair->isMasterOf(hairMid);


    Sheet* hairItems = findSheetInList(pSheetsList, HAIR_ITEMS);
    hair->isMasterOf(hairItems);


    Sheet* moustache    = findSheetInList(pSheetsList, MOUSTACHE);
    Sheet* moustacheI   = findSheetInList(pSheetsList, MOUSTACHE_ITEMS);
    Sheet* moustacheB   = findSheetInList(pSheetsList, MOUSTACHE_BACK);
    moustache->isMasterOf(moustacheI);
    moustache->isMasterOf(moustacheB);

    Sheet* eyes  = findSheetInList(pSheetsList, EYES);
    Sheet* iris  = findSheetInList(pSheetsList, IRIS);
    Sheet* pupil = findSheetInList(pSheetsList, PUPIL);
    eyes->isMasterOf(iris);
    eyes->isMasterOf(pupil);


    Sheet* clothes      = findSheetInList(pSheetsList, CLOTHES);
    Sheet* clothesBack  = findSheetInList(pSheetsList, CLOTHES_SECONDARY);
    Sheet* clothesAcc   = findSheetInList(pSheetsList, CLOTHES_ACCESSORIES);
    clothes->isMasterOf(clothesBack);
    clothes->isMasterOf(clothesAcc);



    // we need to check AFTER we've synced sheets, if we also need to hide their subordonates to match Masters
    if (ui->chk_box_hideSubordinates->isChecked())
        for (Sheet* sh : pSheetsList)
            sh->refreshMirrorHiddenState();

}


void MainWindow::onFilterListClick(QListWidgetItem* item)
{

    // extract string up to :
    QString categoryString = item->text();
    categoryString = categoryString.section(':', 0, 0);

    // decode string into enum
    int index = bodyPartStringList.indexOf(categoryString);
    BodyPart selectedCategory = BodyPart (index);

    // use enum
    ui->categoriesManager->clickCategory(selectedCategory);
}


void MainWindow::savePNG()
{

    QPixmap pixmap = portraitManager.getPixmapCurrentPortrait();

    QFileDialog fileDialog;


    QString fileName = fileDialog.getSaveFileName(nullptr, tr("Save File"), QCoreApplication::applicationDirPath() + QDir::separator() + "portraits_PNG",
                                                                tr("Portrait Data (*.png)"));
    if (fileName.isEmpty())
        return;



    bool success = pixmap.save(fileName, nullptr, -1);

    success = false;
}

void MainWindow::createFolders()
{

    QDir workingDir (QCoreApplication::applicationDirPath());
//    if (!workingDir.exists("resources"))
//        workingDir.mkdir("resources");

//    workingDir.cd("resources");

//    if (workingDir.isEmpty(QDir::AllEntries | QDir::NoDotAndDotDot)){

//        QMessageBox msgBox;
//        msgBox.setText("No Portrait PNGs were found in the 'resources' folder!\nProgram will now exit.");
//        msgBox.exec();

//        exit(0);
//    }

//    workingDir.cdUp();

    // make portraits_DATA and portraits_PNG folders


    if (!workingDir.exists("portraits_DATA"))
        workingDir.mkdir("portraits_DATA");

    if (!workingDir.exists("portraits_PNG"))
        workingDir.mkdir("portraits_PNG");

    if (!workingDir.exists("portraits_Play_Sessions"))
        workingDir.mkdir("portraits_Play_Sessions");

}

void MainWindow::onCodeSideNeckHeightChange(int height)
{
    ui->slider_neckHeight->blockSignals(true);

    ui->slider_neckHeight->setValue(height);

    ui->slider_neckHeight->blockSignals(false);
}

void MainWindow::savePortraits()
{


    QDir worldDir (QCoreApplication::applicationDirPath() + QDir::separator() + "portraits_Play_Sessions" + QDir::separator());
    if (!worldDir.exists(currentWorld))
        if (!worldDir.mkdir(currentWorld)){

            return;
        }

    QString path = QCoreApplication::applicationDirPath() + QDir::separator() + "portraits_Play_Sessions" + QDir::separator() + currentWorld + QDir::separator();

    for (Portrait* portrait : portraitDB){

        QString fileName = path + QString::number(portrait->getID()) + ".bin";
        QFile newPortraitFile(fileName);
        bool success = newPortraitFile.open(QIODevice::WriteOnly);
        QDataStream stream(&newPortraitFile);

        stream << *portrait;

        newPortraitFile.close();
    }

}

void MainWindow::loadPortraits(QString world)
{

    // portraits that are generated in onDwarfFortressSignal have just 1 frame per sheet. There is no randomization to be done here.
    // this is the reason why reloading reverts to frame 0 for all sheets
    // needs further investigation

    QDir worldDir (QCoreApplication::applicationDirPath() + QDir::separator() + "portraits_Play_Sessions" + QDir::separator() + world);
    worldDir.setFilter(QDir::NoDotAndDotDot | QDir::Files);

    for (QFileInfo fileInfo : worldDir.entryInfoList()){


        QString debugSTRING = fileInfo.filePath();

        QFile portraitData (fileInfo.filePath());

        bool success = portraitData.open(QIODevice::ReadOnly);
        QDataStream stream(&portraitData);

        Portrait* portrait = new Portrait();
        stream >> *portrait;

        portraitDB.insert(portrait->getID(), portrait);
    }


}



void MainWindow::onDwarfFortressSignal(bool forceExecute)
{



   {
    // if creature ID exists, draw portrait

    //else
    // get dfCreature
    // get filtered sheets from assetDB based on dfCreature strings
    // create portrait from sheets and add further attributes (age, ID, gender, etc.) ; add color
    // store portrait ID
    // draw portrait


    // DfCreature -> contains data/traits of a df creature
        // includes ticket.h, will hold an array of strings, equal to the nr of total bodyparts (enum)
        // each string will get populated with filter adjectives

    // MsgReader -> contains rules for parsing .txt messages and creating DfCreature objects
        // compose a string with relevant key words for each bodypart
        // Ex: His very long moustache is neatly combed = long moustache neatly combed
            // we could apply extracted adjectives as filters and get just 1 frame sheets from DB that creates the portrait we need
            // if curly or straight or other adjectives are missing, pick at random



    // dfCreature is used by app to draw portrait
        // (?) dfCreature -> Portrait OR Sheets

    // sheet.isFabulation = sheet frame that was picked randomly due to lacking description in DF
        // in case we update portraits on each dwarf read (if hair must turn white because of age, etc.), this must remain fixed after creation

    // a sheet list / portrait is created, which represents our text_to_portrait graphics

    // this portrait needs to be stored in CreaturesDB via hash

    // finally render it to the screen


    // mainWindow will create a portrait from dfCreature. It will ask db for filtered sheets + colorize it
    // portrait will be stored

}



    auto screens = QGuiApplication::screens();
    QRect screenRect = screens[0]->geometry();


    QFile file (messageTxtPath);
    dfMsgReader.readMessage(file);
    QList<DfCreature> creaturesList = dfMsgReader.getCreatures();


    if (dfMsgReader.sameAsLastMessage() && !forceExecute){
        return;
    }


    float perCentSizePortrait = 1.0f;

    if (creaturesList.size() >= 1){
        // code determining portrait size, depending on how many portraits to draw
        int portraitsNr = creaturesList.size();
        int aThirdOfScreen = screenRect.width() / 3;
        int eachPortraitWidth = aThirdOfScreen / portraitsNr;
        if (eachPortraitWidth > maxPortraitSize)
            eachPortraitWidth = maxPortraitSize;

        perCentSizePortrait = ((float)eachPortraitWidth / (float)g_assetSize.width());  // convert pixel size to % of original size
    }

    collection.deleteAllPortraits();

    for (DfCreature creature : creaturesList){

        // to know in which folder to save
        if (creature.getWorldName() != currentWorld){

            changeWorld (creature.getWorldName());
            loadPortraits(currentWorld);
        }

        // load from DB if we already have this ID + name
        if (portraitDB.contains(creature.getID()) && portraitDB.value(creature.getID())->getName() == creature.getName()){

            Portrait* existingPortrait = portraitDB.value(creature.getID());

            existingPortrait->setSelected(creature.isSelected());

            collection.addPortrait(existingPortrait, portraitManager.portraitToPixmap(existingPortrait, perCentSizePortrait));
        }

        // create new portrait and store in db
        else {

            // get sheetList    -> NOTE: must simulate dwarf or creature.selectedFilters must not be empty!
            //QList<Sheet> list = creature.getSheets(&assetDB);

            // move on heap
            //QList<Sheet*> pList;
            //for (const Sheet& sh : list)
            //    pList << new Sheet(sh);


            //// insert color in sheets
            //ui->colorizer->randomizeUsedColor();
            //for (Sheet* sheet : pList)
            //    ui->colorizer->overwriteAllColors(sheet);


            //// sync sheets
            //syncSheets(pList);

            // create portrait
            Portrait* portrait = portraitManager.createPortrait(); // new Portrait (pList);


            portrait->setIgnoreGuiAbsorption(true); // if false, will create displaying problems
            portrait->refreshFrames();

            portrait->absorbDfCreature(creature);


            portraitDB.insert(portrait->getID(), portrait);
            collection.addPortrait(portrait, portraitManager.portraitToPixmap(portrait, perCentSizePortrait));
        }

    }


    collection.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    collection.showCollection(false);
    collection.moveBottomLeft();

    savePortraits();
}

void MainWindow::onDwarfFortressSignal_DEBUG()
{
    collection.deleteAllPortraits();
    auto screens = QGuiApplication::screens();
    QRect rect = screens[0]->geometry();


    int portraitsNr = QRandomGenerator::global()->bounded(1, 10);

    const int maxPortraitSize = 170;
    int aThirdOfScreen = rect.width() / 3;
    int eachPortraitWidth = aThirdOfScreen / portraitsNr;
    if (eachPortraitWidth > maxPortraitSize)
        eachPortraitWidth = maxPortraitSize;
    float perCentSizePortrait = ((float)eachPortraitWidth / (float)g_assetSize.width());

    // determine a % size of the screen width size (say 33%)
    // that represents the total land scape
    // now take the total nr of portraits and divide with total land scape
    // result is how many px each portrait width must have to fit in land scape
    // calculate how much % is resultingPortraitSize from the total width size of 375
    // in case too much landscape is available and too few portraits, establish a max width of portraits


    for (int i = 0; i < portraitsNr; ++i){


        // create portrait
        Portrait* portrait = portraitManager.createPortrait(); // new Portrait (pList);


        portrait->setIgnoreGuiAbsorption(true); // if false, will create displaying problems
        portrait->refreshFrames();
        portrait->setID(QRandomGenerator::global()->bounded(10000));

        portraitDB.insert(portrait->getID(), portrait);
        collection.addPortrait(portrait, portraitManager.portraitToPixmap(portrait, perCentSizePortrait));
    }

    collection.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    collection.showCollection(false);




    int width = 0;
    int height = rect.height() - collection.height();

    collection.move(width, height);


}

void MainWindow::changeWorld(QString newWorld)
{


    for (Portrait* portrait : portraitDB){
        delete portrait;
    }
    portraitDB.clear();

    currentWorld = newWorld;

}

void MainWindow::drawCollection()
{

    float fSize = ui->portraitCollection_size->value();

    fSize /= 100;

    portraitManager.showCollection(fSize);


    if (collection.getPortraits().isEmpty())
        QToolTip::showText( mapToGlobal( ui->btn_addToCollection->pos() ), ui->btn_addToCollection->toolTip());
}

void MainWindow::changeGameMaxPortraitSize()
{

    bool ok;

    int size = -1;

    size = QInputDialog::getInt(this, tr("New size"),
                                         tr("Enter new portrait size:"), QLineEdit::Normal, 0, 100, 1, &ok);


    if (ok && size != -1){

        float f = (size / 100.0f) * (float)g_assetSize.width();
        maxPortraitSize = f;

    }

    onDwarfFortressSignal(true);

}

void MainWindow::onGameChangeClick()
{


    // ask to change mode
    if (!inGameMode){

        QMessageBox messageBox;
        QMessageBox::StandardButton reply;
        reply = messageBox.question(nullptr, "", "Switch to game mode?",
                                        QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes){

            inGameMode = true;
            // hide all windows
            // show portrait window
            collection.setGameMode(true);
            filterController->hide();
            this->hide();
            myFileWatcher->start();
        }

        else
            return;

    }



}

Sheet* MainWindow::findSheetInList(QList<Sheet *> list, BodyPart bp)
{

    for (Sheet* sh : list){

        if (sh->getBodyPart() == bp)
            return sh;
    }

    qDebug() << "WARNING: Could not find Sheet in MainWindow::findSheetInList(QList<Sheet *> list, BodyPart bp) function call!!";
    return new Sheet();
}

QList<Sheet *> MainWindow::findMultipleSheetsInList(QList<Sheet *> list, QList<BodyPart> bpList)
{
    QList<Sheet*> returnList;

    for (Sheet* sh : list)
        for (BodyPart bp : bpList)
            if (sh->getBodyPart() == bp)
                returnList << sh;

    return returnList;
}




void MainWindow::onFilterClick(QWidget* stackedPage)
{

    QList<QStringList> filtersLists;

    // StackedWidget Page of calling Button
    QWidget* stackedWidget = stackedPage;

    // collect all QGroupBox
    QList<QGroupBox*> grBoxList = stackedWidget->findChildren<QGroupBox*>();

    // for each GroupBox
    for (int i = 0; i < grBoxList.size(); ++i){

        QStringList filterList;

        // get all Buttons
        QList<QAbstractButton*> buttonsList = grBoxList[i]->findChildren<QAbstractButton*>();

        // collect checked filters; filters ignored if empty
        for (QAbstractButton* button : buttonsList)
            if (button->isChecked() && !((button->text().compare("all", Qt::CaseInsensitive)) == 0) )
                filterList << button->text().replace(" ", ""); // remove white space from GUI name strings


        if (!filterList.isEmpty())
            filtersLists << filterList;
    }




    Ticket ticket = getTicket();
    ticket.bodypart = (BodyPart)bodyPartStringList.indexOf(stackedWidget->objectName());


    Sheet sheet = assetDB.getSheet(ticket, filtersLists);

    ui->categoriesManager->updateSheets(QList<Sheet>{sheet});

    // incorrectly populates backHair with frontHair
    ui->listWidget->updateStringList(sheet);


}


void MainWindow::onAllCheckBoxClick(QAbstractButton* allButton)
{
    // get state of clicked all-button
    bool allState;
    QAbstractButton* allBtn = allButton;
    QAbstractButton* lastBtn;
    QList<QAbstractButton*> btnList;

    allState = allBtn->isChecked();

    // find the containing QGroupBox of the caller
    QGroupBox* cmbBox = qobject_cast<QGroupBox*>(allBtn->parent());

    // get all buttons except caller in btnList and LAST button
    for (QAbstractButton* aBtn : cmbBox->findChildren<QAbstractButton*>())
        if ((aBtn->text().toLower() != ("all")))
            btnList << aBtn;

    if (btnList.size() >= 2)
        lastBtn = btnList.takeLast();
    else
        lastBtn = btnList[0]; //yolo

    // disconnect all buttons from btnList
    // change all btn state to stored state, EXCEPT last
    // connect all again
    for (QAbstractButton* aBtn : btnList){

        aBtn->blockSignals(true);
        aBtn->setChecked(!aBtn->isChecked());
        aBtn->blockSignals(false);
    }

    // change state of last button to trigger filterExecution via onFilterClick();
    //lastBtn->setChecked(allState);
    lastBtn->setChecked(!allState);
    lastBtn->click();
}


void MainWindow::resizeEvent(QResizeEvent *)
{

    //QRectF bounds = portraitManager.itemsBoundingRect();
    //bounds.setWidth(bounds.width()*1.1);         // to tighten-up margins
    //bounds.setHeight(bounds.height()*0.7);       // same as above
    //ui->graphicsView->fitInView(bounds, Qt::KeepAspectRatio);
    //ui->graphicsView->centerOn(0, 0);


}



void MainWindow::moveEvent(QMoveEvent *event)
{

    auto deltaPos = event->oldPos() - event->pos();
    filterController->move(filterController->pos() - deltaPos);

}

void MainWindow::changeEvent(QEvent *event)
{

    // no longer needed, since we made filterController a childe of mainWindow
    // now filterController minimizez and maximizez along with mainWindow

    //QMainWindow::changeEvent(event);
    //if( event->type() == QEvent::WindowStateChange )
    //{
    //    if( windowState() == Qt::WindowMinimized )
    //    {
    //        //doSomething(this);
    //        filterController->setWindowState(Qt::WindowMinimized);
    //
    //    }
    //    else if( windowState() == Qt::WindowNoState )
    //    {
    //        filterController->setWindowState(Qt::WindowNoState);
    //    }
    //}
    //
}





























