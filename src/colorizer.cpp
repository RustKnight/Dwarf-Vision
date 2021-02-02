#include "colorizer.h"
#include "ui_colorizer.h"

Colorizer::Colorizer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Colorizer)
{
    ui->setupUi(this);

    // WARNING: NEW-ING ORDER MUST MATCH colorManagersIndex ENUMs !!!
    vChromers << new HairChromer;
    vChromers << new IrisChromer;
    vChromers << new ClothesChromer;
    vChromers << new ClothesSecChromer;
    vChromers << new BackgroundChromer;


    // chromers load colors
    for (Chromer* chromer : vChromers)
        chromer->loadColors();


    QList<QRadioButton*> radioBtnList = ui->chromer_GroupBoxSelector->findChildren<QRadioButton*>();
    for (QRadioButton* radio : radioBtnList)
    connect(radio, &QRadioButton::clicked, this, &Colorizer::onChromerRadioChange);

    connect(ui->lerpValueMod,   QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Colorizer::updateLerpVal);
    connect(ui->btn_dark,       &QPushButton::clicked, this, &Colorizer::openColorDialog);
    connect(ui->btn_base,       &QPushButton::clicked, this, &Colorizer::openColorDialog);
    connect(ui->btn_highlight,  &QPushButton::clicked, this, &Colorizer::openColorDialog);
    connect(ui->comboBox_Colors, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Colorizer::changeSelectedColorIndex);
    connect(ui->btn_newColor,   &QPushButton::clicked, this, &Colorizer::addNewColor);
    connect(ui->btn_deleteColor, &QPushButton::clicked, this, &Colorizer::deleteColor);
    connect(ui->btn_resetColors, &QPushButton::clicked, this, &Colorizer::resetColors);
    connect(ui->chkBox_lockColor, &QPushButton::clicked, this, &Colorizer::onColorLockedClick);


    // clicks on hairColor radio
    ui->chromer_RadioSelector_hair->click();

    randomizeUsedColor();

    ui->lerpValueMod->setValue(vChromers[chrIndex]->getLerp());
    ui->lerpValueMod->setSingleStep(0.15);
}

Colorizer::~Colorizer()
{
    delete ui;
}


void Colorizer::randomizeUsedColor()
{
    for (Chromer* chromer : vChromers)
        chromer->randomizeColor();

    //avoid triggering unnecessary portrait draw
    ui->comboBox_Colors->blockSignals(true);
    ui->comboBox_Colors->setCurrentIndex(vChromers[chrIndex]->getColorIndex());
    ui->comboBox_Colors->blockSignals(false);

}

void Colorizer::loadColors()
{
    // used by colorDialogs to call chromers reRead the newly modified colors
    vChromers[chrIndex]->loadColors();
}


void Colorizer::alterColor(const QColor &color)
{

    // base colMan class must have a alterSelectedColor function that takes color and assigns it
    if (QObject::sender()->objectName() == "Dark"){
        vChromers[chrIndex]->alterCurrentColorDark(color);
    }

    else if (QObject::sender()->objectName() == "Base"){
        vChromers[chrIndex]->alterCurrentColorBase(color);
    }

    else if (QObject::sender()->objectName() == "Highlight"){
        vChromers[chrIndex]->alterCurrentColorHighlight(color);
    }

    emit selectedColorChanged();
}

void Colorizer::updateLerpVal(double val)
{
    // lerp needs to be a private data member of the base colMan class
    // base needs an updateLerp() that takes val and updates its state
    // when the colMan classes are destroyed, they should write to their .txt files their current lerp values and REload them upon launch (in loadColors() )
    vChromers[chrIndex]->updateLerp(val);

    saveColorsToText();

    emit selectedColorChanged();
}

void Colorizer::saveColor(const QColor &color)
{

    // base colMan class must have a saveColor

    // ask via messageBox if we want to save color
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "", "Overwrite current color?",
                                    QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
        saveColorsToText();

    if (reply == QMessageBox::No){
        Colorizer::loadColors();
        emit Colorizer::selectedColorChanged();
    }

}

void Colorizer::openColorDialog()
{
    // base colMan needs a getCurrentColor that returns the current color by value

    QColorDialog* colorDialog = new QColorDialog(this);

    if (qobject_cast<QPushButton*>(QObject::sender())->text() == "Dark"){
        colorDialog->setWindowTitle("Dark");
        colorDialog->setObjectName("Dark");
        colorDialog->setCurrentColor(vChromers[chrIndex]->getDark());
    }

    else if (qobject_cast<QPushButton*>(QObject::sender())->text() == "Base"){
        colorDialog->setWindowTitle("Base");
        colorDialog->setObjectName("Base");
        colorDialog->setCurrentColor(vChromers[chrIndex]->getBase());
    }

    else if (qobject_cast<QPushButton*>(QObject::sender())->text() == "Highlight"){
        colorDialog->setWindowTitle("Highlight");
        colorDialog->setObjectName("Highlight");
        colorDialog->setCurrentColor(vChromers[chrIndex]->getHighlight());
    }

    colorDialog->show();
    colorDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(colorDialog, &QColorDialog::currentColorChanged, this, &Colorizer::alterColor);
    connect(colorDialog, &QColorDialog::colorSelected, this, &Colorizer::saveColor);
    connect(colorDialog, &QColorDialog::finished, this, &Colorizer::loadColors);
    connect(colorDialog, &QColorDialog::rejected, this, &Colorizer::loadColors);
    connect(colorDialog, &QColorDialog::rejected, this, &Colorizer::selectedColorChanged);
}

void Colorizer::changeSelectedColorIndex(int index)
{

    vChromers[chrIndex]->setColorIndex(index);
    updateGui();
    emit selectedColorChanged();
}

void Colorizer::onChromerRadioChange()
{
    // find toggled radio name
    QRadioButton* toggledRadio = qobject_cast<QRadioButton*>(QObject::sender());
    QString radioObjName = toggledRadio->objectName();

    // assign index to currently selected chromer via GUI
    if (radioObjName == "chromer_RadioSelector_hair")
        swapCurrentChromer (HAIR_CHROMER);

    else if (radioObjName == "chromer_RadioSelector_iris")
        swapCurrentChromer (IRIS_CHROMER);

    else if (radioObjName == "chromer_RadioSelector_clothes")
        swapCurrentChromer (CLOTHES_CHROMER);

    else if (radioObjName == "chromer_RadioSelector_clothes_secondary")
        swapCurrentChromer (CLOTHES_SECONDARY_CHROMER);

    else if (radioObjName == "chromer_RadioSelector_background")
        swapCurrentChromer (BACKGROUND_CHROMER);


    //changeSelectedColorIndex(0);
}

void Colorizer::addNewColor()
{

    // only base can be modified when adding new color
    // delete colors

    // 1. ask for name;
    bool ok = true;
    QString name = QInputDialog::getText(this, tr("Pick color name"),
                                         tr("New color name:"), QLineEdit::Normal, QString(""), &ok);
    if (!ok)
        return;

    else if (name.isEmpty()) {

        QMessageBox messageBox;
        messageBox.information(0,"Note","Nameless colors are not accepted.");
        messageBox.setFixedSize(500,200);
        return;
    }


    // 2. send for storing in colorsList, color with default values (some weird ones);
    ColorData colorData;
    colorData = vChromers[chrIndex]->getColorData();
    colorData.name = name;
    vChromers[chrIndex]->addColor(colorData);



    // 3. open colorDialog and alter currently-newly-selected color
    QColorDialog* colorDialog = new QColorDialog(this);

    colorDialog->setWindowTitle("Pick color");
    colorDialog->setObjectName("Base");
    colorDialog->setCurrentColor(vChromers[chrIndex]->getBase());
    colorDialog->show();
    colorDialog->setAttribute(Qt::WA_DeleteOnClose);

    connect(colorDialog, &QColorDialog::currentColorChanged, this, &Colorizer::alterColor);
    connect(colorDialog, &QColorDialog::colorSelected, this, &Colorizer::saveColorsToText);
    connect(colorDialog, &QColorDialog::rejected, this, &Colorizer::deleteColor);

    emit colorDialog->currentColorChanged(vChromers[chrIndex]->getBase());

    updateGui();

}

void Colorizer::deleteColor()
{


    if (ui->comboBox_Colors->count() <= 1){

        QMessageBox messageBox;
        messageBox.warning(0,"Invalid","Number of colors cannot be less than 1!");
        messageBox.setFixedSize(500,200);
        return;
    }


    //1. prompt user via dialog yes/no: " Delete currently selected color "light red"? "

    if (QObject::sender()->objectName() == "btn_deleteColor"){  // don't ask if you're a colorDialog that rejected new color; just delete it

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "", "Delete currently selected color \"" + ui->comboBox_Colors->currentText() + "\"",
                                        QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::No)
           return;
    }

    //2. ask current chromer to delete color
    vChromers[chrIndex]->deleteColor();

    //3. deleteColor() also randomizez its current color, refresh portrait
    emit selectedColorChanged();

    //4. update gui
    updateGui();

    //5. save to write remaining colors to txt
    saveColorsToText();

}

void Colorizer::resetColors()
{

    QRadioButton* radioBtn;
    auto list = ui->chromer_GroupBoxSelector->findChildren<QRadioButton*>();

    for (QRadioButton* radio : list)
        if (radio->isChecked())
            radioBtn = radio;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "", "Restore default colors and Lerp value for " + radioBtn->text() + " ?\n(current colors will be lost!)",
                                    QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No)
       return;


    vChromers[chrIndex]->resetColors();
    updateGui();
    emit selectedColorChanged();
}

void Colorizer::overwriteAllColors(Sheet *sh)
{
    for (Chromer* chromer : vChromers){

        chromer->checkAndAssignColor(sh);
    }
}

void Colorizer::overwriteColor(Sheet *sh)
{

    vChromers[chrIndex]->checkAndAssignColor(sh);

}

void Colorizer::readSheetsForColors(const QList<Sheet *> sheetList)
{
    // if sheet is colorable, pass it to Chromers
    for (const Sheet* sheet : sheetList)
        if (sheet->isColorable())
            for (Chromer* chromer : vChromers){

                chromer->checkAndReadColor(sheet);
            }


    // update gui
    updateGui();
}



QPixmap Colorizer::colorize(Sheet *sheet)
{

    // WARNING: this function is certain it will find a colorable Sheet at some point
    // needs a better way of handing the right sheet to the right chromer

    QPixmap pix;

    for (Chromer* chromer : vChromers){

        pix = chromer->checkAndChromarize(sheet);
        if (!pix.isNull())
            break;
    }

    return pix;
}



void Colorizer::saveColorsToText()
{

    // baseColman functionality; same as loadColors()
    // here lerp must also be saved

    vChromers[chrIndex]->saveColors();
    
}

void Colorizer::swapCurrentChromer(colorManagersIndex index)
{

    chrIndex = index;
    updateGui();

}

void Colorizer::updateGui()
{
    // update comboBox colors
    ui->comboBox_Colors->blockSignals(true);

        ui->comboBox_Colors->clear();

        for (const QString& color : vChromers[chrIndex]->getColorStrings())
            ui->comboBox_Colors->addItem(color);

        ui->comboBox_Colors->setCurrentIndex(vChromers[chrIndex]->getColorIndex());

    ui->comboBox_Colors->blockSignals(false);


    // readIn lerp from Chromer and update gui
    ui->lerpValueMod->blockSignals(true);

        ui->lerpValueMod->setValue(vChromers[chrIndex]->getLerp());

    ui->lerpValueMod->blockSignals(false);


    if (vChromers[chrIndex]->isColorLocked())
        ui->chkBox_lockColor->setCheckState(Qt::CheckState::Checked);
    else
        ui->chkBox_lockColor->setCheckState(Qt::CheckState::Unchecked);
}


void Colorizer::onColorLockedClick(bool btnState)
{
    vChromers[chrIndex]->setColorLocked(btnState);
}

void Colorizer::changeHairColorTo(QString color)
{
    if (!vChromers[HAIR_CHROMER]->tryChangeColor(color)){

        QMessageBox messageBox;
        messageBox.warning(0,"Error","Hair Chromer could not find/change color: " + color);
        messageBox.setFixedSize(500,200);
    }
}

void Colorizer::changeIrisColorTo(QString color)
{
    if (!vChromers[IRIS_CHROMER]->tryChangeColor(color)){

        QMessageBox messageBox;
        messageBox.warning(0,"Error","Iris Chromer could not find/change color: " + color);
        messageBox.setFixedSize(500,200);
    }
}

void Colorizer::changeIrisColor(QList<Sheet *> sheetList, QString newColor)
{
    // prepare newColor in IrisChromer
    swapCurrentChromer  (IRIS_CHROMER);
    changeIrisColorTo   (newColor);

    // use newly selected color in IrisChromer to change color
    for (Sheet* sheet : sheetList)
            overwriteColor(sheet);

}

void Colorizer::changeHairColor(QList<Sheet *> sheetList, QString newColor)
{

    // prepare newColor in IrisChromer
    swapCurrentChromer  (HAIR_CHROMER);
    changeHairColorTo   (newColor);

    // use newly selected color in IrisChromer to change color
    for (Sheet* sheet : sheetList)
            overwriteColor(sheet);
}








































