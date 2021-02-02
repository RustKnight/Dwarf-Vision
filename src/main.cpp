#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{

    // generate, next, prev.
    // save png, save porftrait data


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.init();

    //if (argc > 1){

        w.portraitManager.onGenerateButtonClicked();
        w.savePNG_commandLine("debug_test"); //argv[2]

//     exit(0);
//    }

    return a.exec();
}
