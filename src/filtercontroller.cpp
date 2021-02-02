#include "filtercontroller.h"
#include "ui_filtercontroller.h"

FilterController::FilterController(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterController)
{
    ui->setupUi(this);


    this->setAttribute( Qt::WA_DeleteOnClose, true );

    for (QAbstractButton* aBtn : findChildren<QAbstractButton*>())
        if ((aBtn->text().toLower() != ("all"))){    // connect buttons, except the ones with "All"
            connect(aBtn, &QAbstractButton::clicked, this, &FilterController::emitClickedStackedPage);
            connect(aBtn, &QAbstractButton::clicked, this, &FilterController::syncFilterListWidget);
        }
        else
            connect(aBtn, &QAbstractButton::clicked, this, &FilterController::emitClickedAllBtn);

    connect(ui->list_activeFilters, &QListWidget::itemClicked, this, &FilterController::filterListWidgetItemClicked);

    //outputFiltersToText();
}

FilterController::~FilterController()
{
    delete ui;

}

void FilterController::onCategoryChange(const Sheet & sheet)
{
    QString categoryName = sheet.getCategoryName();
    // according to category name, change QStackedWidget page to match name
    for (QObject* object :  ui->filterController->children()){

        QWidget* widget = qobject_cast<QWidget*>(object);

        if (widget)
            if (widget->objectName() == categoryName)
                ui->filterController->setCurrentWidget(widget);
    }



    ui->filter_label->clear();
    ui->filter_label->setTextFormat(Qt::RichText);
    ui->filter_label->setStyleSheet("font-weight: bold;");
    ui->filter_label->setText(categoryName.toUpper() + " FILTERS");
}

QStringList FilterController::getAllFiltersNoSpace()
{
    QStringList resultList;

    // get all buttons from filterController
    QList<QAbstractButton*> buttonsList = ui->filterController->findChildren<QAbstractButton*>();

    // go through all NON-all buttons, delete " " space and add to list
    for (QAbstractButton* button : buttonsList)
        if (!((button->text().compare("all", Qt::CaseInsensitive)) == 0) )
            resultList << button->text().replace(" ", ""); // remove white space from GUI name strings

    return resultList;
}

void FilterController::syncFilterListWidget()
{
    // NOTE: will not work correctly if "All" filter button is used.

    QAbstractButton* clickedButton = qobject_cast<QAbstractButton*>(QObject::sender());
    QString filterStackedPage = clickedButton->parent()->parent()->objectName();
    QString category = filterStackedPage + ":\t";    // WARNING! onFilterListClick() will break if category + : changes
    QString buttonText = clickedButton->text();
    bool buttonBool = clickedButton->isChecked();


    auto stuff = ui->list_activeFilters->findItems(category, Qt::MatchFlag::MatchContains);


    // category exists, append filter to category
    if (!stuff.isEmpty()){

        QString filterLineStr = stuff[0]->text();


        // if checked, add filter
        if (buttonBool)
            stuff[0]->setText(filterLineStr.append(", " + buttonText));


        // if unchecked
        else {

            // remove filter
            stuff[0]->setText(removeFilter(filterLineStr, buttonText));

            // if category has no filters left, remove category as well
            if (stuff[0]->text() == category){
                ui->list_activeFilters->removeItemWidget(stuff[0]); // does not work - see udemy courses on how to remove
                delete stuff[0];
            }
        }
    }

    // no category present, add category and filter (no comma)
    else{

        ui->list_activeFilters->addItem(category);
        auto stuff = ui->list_activeFilters->findItems(category, Qt::MatchFlag::MatchContains);
        stuff[0]->setText(stuff[0]->text().append(" " + buttonText));

        QString path = ":/icons/" + filterStackedPage + "_icon.png";
        QIcon icon;
        icon.addFile(path);
        stuff[0]->setIcon(icon);

        // capitalize category
        category = category.toUpper();
    }


    // change filter label color based on list (no filters, with filters)

    if (ui->list_activeFilters->count() == 0)
        ui->label_filterStatus->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600; color:#000000;\">NO FILTERS ACTIVE</span></p></body></html>", nullptr));
    else
        ui->label_filterStatus->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600; color:#FF0000;\">FILTERS IN USE</span></p></body></html>", nullptr));
}


QString FilterController::removeFilter(QString workString, QString removeThis)
{

    QString result;

    if (removeThis == "Braided"){

        // split task
        QStringList splitList = workString.split(",");

        // check each split if it does NOT contain double, and delete
        QList<QString>::iterator i;

        for (i = splitList.begin(); i != splitList.end(); ++i){

            // skip Double Braided
            if (i->contains("double", Qt::CaseInsensitive))
                continue;


            // split will be empty after removal
            if (i->contains("braided", Qt::CaseInsensitive)){

                i->remove(" "  + removeThis + ",");
                i->remove(", " + removeThis);
                i->remove(" "  + removeThis);

                break;
            }
        }


        for (QString& s : splitList){

            if (s.contains(":\t"))
                result.append(s);

            else if (s != splitList.last())
                result.append(s + ",");

            else
                result.append(s);
        }


        return result;
    }


    else {

        result = workString.remove(" " + removeThis + ",");
        result = workString.remove(", " + removeThis);
        result = workString.remove(" " + removeThis);
    }


    return result;
}


void FilterController::emitClickedStackedPage()
{

    QWidget* stackedWidget = qobject_cast<QWidget*>(QObject::sender()->parent()->parent());
    emit filterButtonClicked(stackedWidget);

}

void FilterController::emitClickedAllBtn()
{
    QAbstractButton* allBtn = qobject_cast<QAbstractButton*>(QObject::sender());
    emit filterButtonClickedAll(allBtn);

}


void FilterController::outputFiltersToText()
{

    // debug/dev

    QFile outputFile (QCoreApplication::applicationDirPath() + QDir::separator() + "output.txt");
    outputFile.open(QIODevice::WriteOnly);
    QTextStream stream(&outputFile);


    QStringList validCategories;

    validCategories << "base"     ;
    validCategories << "clothes"  ;
    validCategories << "head"     ;
    validCategories << "ears"     ;
    validCategories << "cheeks"   ;
    validCategories << "lips"     ;
    validCategories << "beard"    ;
    validCategories << "moustache";
    validCategories << "eyes"     ;
    validCategories << "nose"     ;
    validCategories << "eyebrows" ;
    validCategories << "hair"     ;

    for (QObject* object :  ui->filterController->children()){

        // get stackedWidgetPage
        QWidget* widgetPage = qobject_cast<QWidget*>(object);

        if (widgetPage){

            // skip secondary categories
            if (!validCategories.contains(widgetPage->objectName()))
                continue;

            QString category = widgetPage->objectName();
            stream << QString(category) << "\n\n";

            // get all group boxes
            for (QGroupBox* groupBox : widgetPage->findChildren<QGroupBox*>()){

                stream << "{\"";

                // get all check boxes
                auto listCheckBoxes = groupBox->findChildren<QCheckBox*>();
                QList<QCheckBox*>::iterator i;
                for (i = listCheckBoxes.begin(); i != listCheckBoxes.end(); ++i){

                    // ignore "all" buttons
                    if (((*i)->text().compare("all", Qt::CaseInsensitive)) == 0)
                        continue;

                    // if not last item, add comma
                    if (i != listCheckBoxes.end() - 1){

                        stream << (*i)->text(); stream << ", ";
                    }
                    // if last, no comma
                    else {

                        stream << (*i)->text();
                    }
                }



                stream << "\"};";
                stream << "\n";

            }

        }

        stream << "\n";
        stream << "\n";

    }


    outputFile.close();
}





















