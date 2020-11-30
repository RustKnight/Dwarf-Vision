#include "mystringlist.h"

MyStringList::MyStringList(QWidget *parent) : QListWidget(parent)
{

}

void MyStringList::updateStringList(const Sheet &sheet)
{

        // triggered when a new category is selected. At this point, we don't need an Index Changed from MyStringList taht will trigger a draw request

        this->blockSignals(true);

    this->clear();

    QStringList list = sheet.getStrings();

    this->addItems(list);

        this->blockSignals(false);

}

void MyStringList::changeStrIndex(int index)
{

    // triggered only by portraitManager - it does not need to know it changed and cause an additional redraw

        this->blockSignals(true);

    itemIndex = index;
    this->setCurrentRow(itemIndex);

        this->blockSignals(false);
}











