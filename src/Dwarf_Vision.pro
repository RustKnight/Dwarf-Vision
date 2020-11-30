QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    assetdb.cpp \
    categoriesmanager.cpp \
    category.cpp \
    collection.cpp \
    colorizer.cpp \
    colorizer/backgroundchromer.cpp \
    colorizer/chromer.cpp \
    colorizer/clotheschromer.cpp \
    colorizer/hairchromer.cpp \
    colorizer/irischromer.cpp \
    dfcreature.cpp \
    dfmessagereader.cpp \
    filtercontroller.cpp \
    main.cpp \
    mainwindow.cpp \
    myfilewatcher.cpp \
    mystringlist.cpp \
    portrait.cpp \
    portraitmanager.cpp \
    sheet.cpp

HEADERS += \
    Ticket.h \
    assetdb.h \
    categoriesmanager.h \
    category.h \
    collection.h \
    colorizer.h \
    colorizer/backgroundchromer.h \
    colorizer/chromer.h \
    colorizer/clotheschromer.h \
    colorizer/colordata.h \
    colorizer/hairchromer.h \
    colorizer/irischromer.h \
    dfcreature.h \
    dfmessagereader.h \
    filtercontroller.h \
    globals.h \
    mainwindow.h \
    myfilewatcher.h \
    mystringlist.h \
    portrait.h \
    portraitmanager.h \
    sheet.h

FORMS += \
    categoriesmanager.ui \
    category.ui \
    collection.ui \
    colorizer.ui \
    filtercontroller.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

