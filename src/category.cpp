#include "category.h"
#include "ui_category.h"
#include "categoriesmanager.h"

Category::Category(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Category)
{
    ui->setupUi(this);

    ui->pushButton->setCheckable(true);

    QObject::connect(ui->pushButton, &QPushButton::clicked, this, &Category::buttonPressed);
    QObject::connect(ui->pushButton_icon, &QPushButton::clicked, this, &Category::buttonPressed);

    connect(ui->lockBox, &QCheckBox::clicked, this, &Category::updateSheetAndNotify);
    connect(ui->hideBox, &QCheckBox::clicked, this, &Category::updateSheetAndNotify);




    ui->frame->setFrameShape(QFrame::Shape::NoFrame);


}

Category::~Category()
{
    delete ui;
}

void Category::setCategoryName(QString category)
{
    ui->pushButton->setText(category);
}

QString Category::getCategoryName() const
{
    return ui->pushButton->text();
}

void Category::createDrawOrderArrows()
{
    //// UP ARROW CREATION
    //////////////////////
    QPushButton* btnArrowUp = new QPushButton ("", this);
    btnArrowUp->setObjectName("arrow");

    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(btnArrowUp->sizePolicy().hasHeightForWidth());
    btnArrowUp->setSizePolicy(sizePolicy);
    btnArrowUp->setMaximumSize(QSize(40, 16777215));

    QString path = ":/icons/up_arrow_green.png";
    QIcon icon;
    icon.addFile(path, QSize(), QIcon::Normal, QIcon::Off);
    btnArrowUp->setIcon(icon);
    btnArrowUp->setIconSize(QSize(15, 15));

    ui->horizontalLayout->insertWidget(ui->horizontalLayout->count() - 1, btnArrowUp);
    connect (btnArrowUp, &QPushButton::clicked, this, &Category::moveMeUp);



    //// DOWN ARROW CREATION
    ////////////////////////
    QPushButton* btnArrowDown = new QPushButton ("", this);
    btnArrowDown->setObjectName("arrow2");

    QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(btnArrowDown->sizePolicy().hasHeightForWidth());
    btnArrowDown->setSizePolicy(sizePolicy2);
    btnArrowDown->setMaximumSize(QSize(40, 16777215));

    QString path2 = ":/icons/down_arrow_yellow.png";
    QIcon icon2;
    icon2.addFile(path2, QSize(), QIcon::Normal, QIcon::Off);
    btnArrowDown->setIcon(icon2);
    btnArrowDown->setIconSize(QSize(15, 15));

    ui->horizontalLayout->insertWidget(1, btnArrowDown);
    connect (btnArrowDown, &QPushButton::clicked, this, &Category::moveMeDown);
}

void Category::deleteDrawOrderArrows()
{
    //ui->horizontalLayout->
    QPushButton* btn = this->findChild<QPushButton*>("arrow");
    QPushButton* btn2 = this->findChild<QPushButton*>("arrow2");
    delete btn;
    delete btn2;
}

void Category::mySetHidden(bool truth)
{
    hidden = truth;
    setHidden(hidden);
}

bool Category::myIsHidden() const
{
    return hidden;
}

QPoint Category::getArrowUpPos() const
{


    QPushButton* btnUp = this->findChild<QPushButton*>("arrow");

    qDebug() << btnUp->size();
    qDebug() << btnUp->sizeHint();

    return btnUp->mapToGlobal(QPoint {btnUp->width() / 2, - int(btnUp->height() * 1.5f)});

}

QPoint Category::getArrowDownPos() const
{



    QPushButton* btnDown = this->findChild<QPushButton*>("arrow2");

    qDebug() << btnDown->size();
    qDebug() << btnDown->sizeHint();

    return btnDown->mapToGlobal(QPoint {btnDown->width() / 2, int(btnDown->height() * 2.5f)});


}

void Category::setSelected(bool truth)
{


    // if category is clicked and is not already selected

    if (truth && !isSelected)
        createDrawOrderArrows();

    isSelected = truth;

    ui->pushButton->setChecked(truth);
    sheet.selectSheet(truth);


    if (!truth)
        deleteDrawOrderArrows();

}

void Category::setSheet(Sheet sheet)
{
    this->sheet = sheet;
    this->updateSheet();
}

bool Category::matchesEnum(BodyPart enu)
{

    BodyPart asset = BodyPart (bodyPartStringList.indexOf(ui->pushButton->text()));

    return asset == enu;
}

const Sheet &Category::getSheet()
{
    return sheet;
}

void Category::toggleHide()
{
    ui->hideBox->click();
}

void Category::toggleLock()
{
    ui->lockBox->click();
}

void Category::applyIcon(QString iconName)
{
    QString path = ":/icons/" + iconName + "_icon.png";

    QIcon icon;
    icon.addFile(path, QSize(), QIcon::Normal, QIcon::Off);
    ui->pushButton_icon->setIcon(icon);
    ui->pushButton_icon->setIconSize(QSize(35, 35));
}


void Category::updateSheet()
{

    sheet.hideFrame(ui->hideBox->isChecked());

    sheet.lockFrame(ui->lockBox->isChecked());

}

void Category::buttonPressed()
{
    emit thisCategoryClicked(this);
}

void Category::updateSheetAndNotify()
{

    sheet.hideFrame(ui->hideBox->isChecked());

    sheet.lockFrame(ui->lockBox->isChecked());

    emit guiStateChanged();
}




















