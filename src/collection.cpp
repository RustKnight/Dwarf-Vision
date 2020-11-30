#include "collection.h"
#include "ui_collection.h"

Collection::Collection(QWidget *parent, bool gameMode) :
    QDialog(parent),
    ui(new Ui::Collection),
    gameMode {gameMode},
    customPosition {OUT_OF_SIGHT, OUT_OF_SIGHT},
    menu {this}
{
    ui->setupUi(this);

    // for correct resizing when we draw fewer qlabels than previous draw
    ui->verticalLayout->setSizeConstraint(QLayout::SetFixedSize);

}

Collection::~Collection()
{
    delete ui;
}

void Collection::addPortrait(Portrait *portraitIncoming, QPixmap pixmap)
{
    // don't add duplicate portraits
    for (Portrait* portrait : vCollectionData)
        if (portrait == portraitIncoming)
            return;


    vCollectionData << portraitIncoming;
    vCollectionPix  << pixmap;
}

void Collection::showCollection(bool addSubmenus)   // subMenus refers to minimize, app close and resize for in-game ussage
{




    // for all current QPixmaps Portraits

    for (int i = 0; i < vCollectionPix.size(); ++i) {

        // make a new Label and assign Pixmap

        MyLabel* l = new MyLabel(this, vCollectionData[i], addSubmenus);
        l->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        l->setPixmap(vCollectionPix[i]);
        ui->horizontalLayout->addWidget(l, 0);

        connect(l, &MyLabel::deleteMe, this, &Collection::deletePortraitAndLabel);
        connect(l, &MyLabel::editMe,   this, &Collection::editPortrait);
        connect(l, &MyLabel::closeMe,   this, &Collection::closeDialog);

    }

    show();
}

void Collection::showTempCollection(QList<QPixmap> pixPortraits, QList<Portrait*> pPortrais)
{

    QList<MyLabel*> tempLabels;
    Collection tempCollection;

    tempCollection.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    // tempCollection is emitting which portrait to edit, but it is not connected to its receiver
    // tempCollection signal is connected to this-Collection signal so that it pass the portrait to edit
    connect(&tempCollection, &Collection::editThisPortrait, this, &Collection::editThisPortrait);

    for (int i = 0; i < pixPortraits.size(); ++i) {

        // make a new Label and assign Pixmap

        MyLabel* l = new MyLabel(&tempCollection, pPortrais[i], true);
        tempLabels << l;
        l->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        l->setPixmap(pixPortraits[i]);
        tempCollection.ui->horizontalLayout->addWidget(l, 0);

        //connect(l, &MyLabel::deleteMe, this, &Collection::deletePortraitAndLabel);
        l->removeAction(l->actions()[0]);   // get rid of deleteAction
        connect(l, &MyLabel::editMe,   &tempCollection, &Collection::editPortrait);
        connect(l, &MyLabel::closeMe,   &tempCollection, &Collection::closeDialog);

    }

    tempCollection.exec();// otherwise, if show(), we'll delete the portraits right away


    // might be useless since labels are children of tempCollection
//     for (int i = 0; i < tempLabels.size(); ++i) {

//         tempCollection.deleteLabel( tempLabels[i] );
//     }

}

void Collection::moveBottomLeft()
{

    // this operation must be done AFTER the showCollection routine
    // cannot calculate position without knowing what actual size our labels will take

    // move to bottomLeft only if didn't change custom_position
    if (customPosition == QPoint {OUT_OF_SIGHT, OUT_OF_SIGHT}){

        auto screens = QGuiApplication::screens();
        QRect screenRect = screens[0]->geometry();

        int width = 0;
        int height = screenRect.height() - this->height();

        move(width, height);
    }

}

void Collection::deletePortraitAndLabel()
{


    MyLabel* myLabel = qobject_cast<MyLabel*> (QObject::sender());

    for (int i = 0; i < vCollectionData.size(); ++i) {

        if (myLabel->portraitData == vCollectionData[i]){

            vCollectionData.removeAt(i);
            vCollectionPix.removeAt(i);

            ui->horizontalLayout->removeWidget(myLabel);
            myLabel->deleteLater();

            break;
        }
    }

}

void Collection::deleteLabel(MyLabel *label)
{

    ui->horizontalLayout->removeWidget(label);
    label->deleteLater();
}

void Collection::editPortrait()
{

    MyLabel* myLabel = qobject_cast<MyLabel*> (QObject::sender());

    Portrait* portrait = myLabel->portraitData;

    this->close();

    emit editThisPortrait(portrait);


}

QList<Portrait *> Collection::getPortraits()
{
    return vCollectionData;
}

void Collection::deleteAllPortraits()
{

    // instead of deleting and redrawing all the time, QPixmap portraits could be stored and hidden when no longer in use
    // issue: PNGs have different sizes and can only get a new size via the draw function from PortraitManager


    vCollectionData.clear();
    vCollectionPix.clear();

    QLayoutItem *child;
    while ((child = ui->horizontalLayout->takeAt(0)) != nullptr) {

        delete child->widget(); // delete the widget
        delete child;           // delete the layout item
    }


}


void Collection::closeDialog()
{
    this->close();
}

void Collection::minimizeMe()
{
    this->setWindowState(Qt::WindowMinimized);
}

void Collection::exitProgram()
{
    exit(0);
}



void Collection::setGameMode(bool truth)
{
    gameMode = truth;

    if (gameMode){

        QAction *minimizeMe = new QAction("Minimize", this);
        connect(minimizeMe, &QAction::triggered, this, &Collection::minimizeMe);
        this->addAction(minimizeMe);

        QAction *changePortraitSize = new QAction("Change Portrait Size", this);
        connect(changePortraitSize, &QAction::triggered, this, &Collection::needSizeChange);
        this->addAction(changePortraitSize);

        QAction *closeProgram = new QAction("Close Program", this);
        connect(closeProgram, &QAction::triggered, this, &Collection::exitProgram);
        this->addAction(closeProgram);

        menu.addActions(this->actions());
    }
}

void Collection::mousePressEvent(QMouseEvent *event)
{


    mouseClickXPos = event->x();
    mouseClickYPos = event->y();


    if (event->buttons() == Qt::RightButton && gameMode)
        menu.exec  (event->globalPos());

}

void Collection::mouseMoveEvent(QMouseEvent *event)
{

    overlay.hide();

    customPosition.setX(mouseClickXPos);
    customPosition.setY(mouseClickYPos);

    move(event->globalX()-mouseClickXPos,event->globalY()-mouseClickYPos);

}



void Collection::enterEvent(QEvent *ev)
{

    if (gameMode) {

        overlay.updateSize(this->size());
        overlay.move(pos());
        overlay.show();

    }

}

void Collection::leaveEvent(QEvent *ev)
{
    overlay.hide();
}



















void MyLabel::enterEvent(QEvent *ev)
{

    if (subMenus){

        beforeOverlay = *pixmap();

        QPixmap overlay (*pixmap());

        QPixmap selectedOverlay(":/overlay.png");

        QPixmap composedPortrait(overlay);

        QPainter painter(&composedPortrait);

        painter.drawPixmap(0, 0, selectedOverlay);

        setPixmap(composedPortrait);
    }
}

void MyLabel::leaveEvent(QEvent *ev)
{
    if (subMenus)
        setPixmap(beforeOverlay);
}

void MyLabel::mouseReleaseEvent(QMouseEvent *ev) {

    if(ev->button() == Qt::RightButton){


        QMenu MyMenu        (this);
        MyMenu.addActions   (this->actions());
        MyMenu.exec         (ev->globalPos());

    }

}


































void Overlay::updateSize(QSize newSize)
{
    this->resize(newSize);
}
