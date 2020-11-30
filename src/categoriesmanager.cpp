#include "categoriesmanager.h"
#include "ui_categoriesmanager.h"





CategoriesManager::CategoriesManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CategoriesManager),
    selectedCategory(nullptr)

{





    ////////////////////////////////////////////////
    //////////////// KEYBOARD SHORTCUTS/////////////

        QAction *goUp = new QAction(this);
        goUp->setShortcut(Qt::Key_W);
        connect(goUp, &QAction::triggered, this, &CategoriesManager::categoryUp);
        this->addAction(goUp);

    //-------------------------------

        QAction *goDown = new QAction(this);
        goDown->setShortcut(Qt::Key_S);
        connect(goDown, &QAction::triggered, this, &CategoriesManager::categoryDown);
        this->addAction(goDown);

    //////////////// KEYBOARD SHORTCUTS/////////////
    ////////////////////////////////////////////////




    ui->setupUi(this);


    // CREATE CATEGORIES
    for (QString bdyString : bodyPartStringList){

        Category * category = new Category(this);
        category->setCategoryName(bdyString);
        connect(category, &Category::thisCategoryClicked, this, &CategoriesManager::selectCategory);
        connect(category, &Category::guiStateChanged, this, &CategoriesManager::portraitNeedsUpdate);
        connect(category, &Category::moveMeUp, this, &CategoriesManager::categoryUp);
        connect(category, &Category::moveMeDown, this, &CategoriesManager::categoryDown);

        category->applyIcon(bdyString);


        ui->verticalLayout_2->addWidget(category);

            if (category->getCategoryName() == "base"      )  category->mySetHidden(false);
            if (category->getCategoryName() == "clothes"   )  category->mySetHidden(false);
            if (category->getCategoryName() == "head"      )  category->mySetHidden(false);
            if (category->getCategoryName() == "ears"      )  category->mySetHidden(false);
            if (category->getCategoryName() == "cheeks"    )  category->mySetHidden(false);
            if (category->getCategoryName() == "lips"      )  category->mySetHidden(false);
            if (category->getCategoryName() == "beard"     )  category->mySetHidden(false);
            if (category->getCategoryName() == "moustache" )  category->mySetHidden(false);
            if (category->getCategoryName() == "eyes"      )  category->mySetHidden(false);
            if (category->getCategoryName() == "nose"      )  category->mySetHidden(false);
            if (category->getCategoryName() == "eyebrows"  )  category->mySetHidden(false);
            if (category->getCategoryName() == "hair"      )  category->mySetHidden(false);

            if (category->getCategoryName() == "hairBack"  )  category->mySetHidden(true);
            if (category->getCategoryName() == "noseBack"  )  category->mySetHidden(true);
            if (category->getCategoryName() == "beardStich")  category->mySetHidden(true);
            if (category->getCategoryName() == "beardItems")  category->mySetHidden(true);
            if (category->getCategoryName() == "moustacheB")  category->mySetHidden(true);
            if (category->getCategoryName() == "moustacheI")  category->mySetHidden(true);
            if (category->getCategoryName() == "iris"      )  category->mySetHidden(true);
            if (category->getCategoryName() == "pupil"     )  category->mySetHidden(true);
            if (category->getCategoryName() == "eyebrowsB" )  category->mySetHidden(true);
            if (category->getCategoryName() == "hairItems" )  category->mySetHidden(true);
            if (category->getCategoryName() == "clothesBack") category->mySetHidden(true);
            if (category->getCategoryName() == "clothesAcc")  category->mySetHidden(true);
            if (category->getCategoryName() == "hairItems" )  category->mySetHidden(true);
            if (category->getCategoryName() == "hairMid" )    category->mySetHidden(true);



    }

    connect(ui->btn_hideAll, &QPushButton::clicked, this, &CategoriesManager::hideAll);
    connect(ui->btn_lockAll, &QPushButton::clicked, this, &CategoriesManager::lockAll);


    // select the first category
    selectCategory (getCategories().at(0));


    // ADD VERTICAL_SPACER AT END
    m_verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->verticalLayout_2->addItem(m_verticalSpacer);


}


QList<Category*> CategoriesManager::getCategories()
{

    QList<Category*> returnList;

    for (Category* cat : this->findChildren<Category*>())
            returnList << cat;


    return returnList;
}


CategoriesManager::~CategoriesManager()
{
    delete ui;
}

void CategoriesManager::updateSheets(QList<Sheet> sheetList)
{


    {
    // whenever GUI state changes (Gender, Race) GUI must supply (call an update function for CatMan) CategoriesManager with the coresponding list of Sheets
        // Categories manager then takes the list and:

            // for each sheet SH
                // for each category CAT
                    // if (bool CAT.matchesWithSheet(SH.ENUM) )
                        // CAT.assignSH (SH)
                        // break
    }

    //resetCategories();

    for (Sheet& sh : sheetList)
        for (Category* cat : getCategories())
            if (cat->matchesEnum(sh.getBodyPart())){

                cat->setSheet(sh);
                break;
            }

    refreshCategory();

    portraitNeedsUpdate();
}

QString CategoriesManager::getSelectedCategoryName() const
{
    return selectedCategory->getCategoryName();
}

QList<Sheet> CategoriesManager::getSheetList()
{
    QList<Sheet> list;

    for (Category* cat : getCategories())
        list << cat->getSheet();

    return list;
}

void CategoriesManager::resetCategories()
{
    for (Category* cat : getCategories())
        cat->setSheet(Sheet());
}



int CategoriesManager::indexOfSelectedSheet()
{
    return getCategories().indexOf(selectedCategory);
}

void CategoriesManager::clickCategory(BodyPart category)
{

    for (Category* cat : getCategories())
        if (cat->matchesEnum(category))
            cat->buttonPressed();
}



void CategoriesManager::categoryUp()
{

    // see if upper category is hidden (recursively)
    // get jumpStep
    // check if indexOfSelectedCat - jumpStep is less than 0
    // apply jumpStep

    
    QList<Category*> list = getCategories();

    if (list.indexOf(selectedCategory) == list.indexOf(list.first()))
        return;

    if (selectedCategory == nullptr)
        return;

    ui->verticalLayout_2->removeItem(m_verticalSpacer);
    delete m_verticalSpacer;


    int jumpStep = findNextVisibleCategoryIndex(selectedCategory, true);
    list.swapItemsAt(list.indexOf(selectedCategory), list.indexOf(selectedCategory) + jumpStep);


     for (Category* category : list){

         category->setParent(this);
         //category->show();
         ui->verticalLayout_2->addWidget(category);
     }


     m_verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
     ui->verticalLayout_2->addItem(m_verticalSpacer);

     portraitNeedsUpdate();


     // MOVES MOUSE UP WITH CATEGORY
     Category * cat;
     if ((cat = qobject_cast<Category*>(QObject::sender()))){
         QPoint point = cat->getArrowUpPos();
         QCursor::setPos(point);
     }

}

void CategoriesManager::categoryDown()
{


    QList<Category*> list = getCategories();

    if (list.indexOf(selectedCategory) == list.indexOf(list.last()))
        return;

    if (selectedCategory == nullptr)
        return;

    ui->verticalLayout_2->removeItem(m_verticalSpacer);
    delete m_verticalSpacer;


    int jumpStep = findNextVisibleCategoryIndex(selectedCategory, false);
    list.swapItemsAt(list.indexOf(selectedCategory), list.indexOf(selectedCategory) + jumpStep);


     for (Category* category : list){

         category->setParent(this);
         //category->show();
         ui->verticalLayout_2->addWidget(category);
     }


     m_verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
     ui->verticalLayout_2->addItem(m_verticalSpacer);

     portraitNeedsUpdate();


     // MOVES MOUSE DOWN WITH CATEGORY
     Category * cat;
     if ((cat = qobject_cast<Category*>(QObject::sender()))){
         QPoint point = cat->getArrowDownPos();
         QCursor::setPos(point);
     }

}

void CategoriesManager::portraitNeedsUpdate()
{
    emit sheetsChangedState(getSheetList());
}



void CategoriesManager::selectCategory(Category* category)
{
    if (selectedCategory != nullptr)
        selectedCategory->setSelected(false);


    selectedCategory = category;
    selectedCategory->setSelected(true);

    portraitNeedsUpdate();
    emit newCategorySelected(category->getSheet());
}

void CategoriesManager::hideAll()
{
    for (Category* cat : getCategories())
        cat->toggleHide();

}

void CategoriesManager::lockAll()
{
    for (Category* cat : getCategories())
        cat->toggleLock();
}



void CategoriesManager::refreshCategory()
{
    selectedCategory->setSelected(true);
}



int CategoriesManager::findNextVisibleCategoryIndex(Category* cat, bool dirUp)
{

    QList<Category*> list = getCategories();
    int indexCurrentCat = list.indexOf(cat);
    int index = indexCurrentCat;


    if (dirUp){

        for (int i = indexCurrentCat - 1; i >= 0; --i){

            if (!list[i]->myIsHidden()){
                index = i;
                break;
            }
        }

        return index - indexCurrentCat;
    }


    else {


        for (int i = indexCurrentCat + 1; i < list.size(); ++i){

            if (!list[i]->myIsHidden()){
                index = i;
                break;
            }
        }

        return index - indexCurrentCat;
    }


}




































