#include "chromer.h"

Chromer::Chromer()
{

}

void Chromer::loadColors(QString fileName)
{
    // needs error check when loading colors. Particulary when lambReadColorLINE fails.

    vColors.clear();


    QDir workingDir (QCoreApplication::applicationDirPath());
    QString filePath = QCoreApplication::applicationDirPath() + QDir::separator() + fileName;
    QFile colorsFile (filePath);

    if (!colorsFile.open(QIODevice::ReadOnly)){

        qWarning() << "Error when opening " + fileName;
        qWarning() << colorsFile.errorString();
    }


    QTextStream stream (&colorsFile);

    auto lambReadColorLINE = [&]() {

        QString value = stream.readLine().simplified();
        QStringList splitList = value.split(" ");

        int red     = splitList[1].toInt(nullptr, 10);
        int green   = splitList[2].toInt(nullptr, 10);
        int blue    = splitList[3].toInt(nullptr, 10);

        return QColor {red, green, blue};
    };


    QString colorName;
    ColorData color;



    while (!stream.atEnd()){

        // get color name
        stream.skipWhiteSpace();

        colorName = stream.readLine();

        color.name = colorName;


        // get lerp
        stream.skipWhiteSpace();

        QString lerpStr = stream.readLine();
        QStringList splitLerp = lerpStr.split(" ");

        color.lerp = splitLerp[2].toDouble();




        // get color data
        stream.skipWhiteSpace();

        color.dark      = lambReadColorLINE();

        color.base      = lambReadColorLINE();

        color.highlight = lambReadColorLINE();

        vColors << color;

        stream.skipWhiteSpace();
    }


    if (vColors.isEmpty()){

        // express warning

        //QMessageBox messageBox;
        //messageBox.warning(0,"Error", "Colorizer couldn't load any colors from " + fileName + "!\nGrabbing internal default colors...");
        //messageBox.setFixedSize(500,200);

        QFile::copy(":/color_data/" + fileName, filePath);
        QFile freshlyCopied(filePath);
        freshlyCopied.setPermissions(QFileDevice::WriteOther);

        // retry with newly copied txt from resource
        loadColors(fileName);
    }

}

void Chromer::randomizeColor()
{

    if (colorLocked)
        return;

    int colorIndex = QRandomGenerator::global()->bounded(0, vColors.size());
    clrNowIndex = colorIndex;

}

void Chromer::alterCurrentColorDark(const QColor &dark)
{
    vColors[clrNowIndex].dark = dark;
}

void Chromer::alterCurrentColorBase(const QColor &base)
{
    vColors[clrNowIndex].base = base;
}

void Chromer::alterCurrentColorHighlight(const QColor &highlight)
{
    vColors[clrNowIndex].highlight = highlight;
}

void Chromer::updateLerp(double lerpVar)
{
    vColors[clrNowIndex].lerp = lerpVar;
}

void Chromer::setColorIndex(int index)
{
    clrNowIndex = index;
}

void Chromer::addColor(QString fileName, const ColorData &colorData)
{

     vColors << colorData;
     setColorIndex(vColors.size() - 1);
}

void Chromer::deleteColor()
{
    QList<ColorData>::iterator i;

    for (i = vColors.begin(); i != vColors.end(); ++i)
        if (i->name == vColors[clrNowIndex].name){
            vColors.erase(i);
            break;
        }

    randomizeColor();
}

ColorData Chromer::getColorData() const
{
    return vColors[clrNowIndex];
}

bool Chromer::isColorLocked() const
{
    return colorLocked;
}

void Chromer::setColorLocked(bool truth)
{
    colorLocked = truth;
}

bool Chromer::tryChangeColor(QString colorName)
{
    for (int i = 0; i < vColors.size(); ++i)
        if (vColors[i].name.contains(colorName, Qt::CaseInsensitive)){
            setColorIndex(i);
            return true;
        }



    // try switching names (Sandy Tapue -> Taupe Sandy)
    auto split = colorName.split(" ");

    if (split.size() >= 2){

        QString temp0 = split[0];
        split[0] = split[1];
        split[1] = temp0;

        colorName = split.join(" ");
    }

    // second try
    for (int i = 0; i < vColors.size(); ++i)
        if (vColors[i].name.contains(colorName, Qt::CaseInsensitive)){
            setColorIndex(i);
            return true;
        }



    return false;

}

void Chromer::resetColors()
{

}


QPixmap Chromer::chromarize(Sheet *sheet)
{
    // base colMan will use this function and also make use of the new private data member lerpVar

    QPixmap pix = sheet->getCurrentPixmap();

    QImage img = pix.toImage();


    auto clampLamb = [](double i) {

        if (i > 255)
            return 255.0;

        if (i < 0)
            return 0.0;

        return i;
    };

    auto lerpLamb = [](QColor l, QColor r, double inter) {

        QColor c;

        c.setRed (l.red() + ((inter / 255.0f) * (r.red() - (l.red()) ) ) );
        c.setGreen (l.green() + ((inter / 255.0f) * (r.green() - (l.green()) ) ) );
        c.setBlue (l.blue() + ((inter / 255.0f) * (r.blue() - (l.blue()) ) ) );

        return c;
    };


// dark, base
// color2, highlight,


    for (int h = 0; h < img.height(); ++h)
        for (int w = 0; w < img.width(); ++w) {

            QColor currentPixelCol = img.pixelColor(QPoint (w,h));

            if (currentPixelCol.alpha() > 0) {

                QColor color2 = lerpLamb (sheet->getColorData().dark, sheet->getColorData().base, clampLamb (currentPixelCol.green() * sheet->getColorData().lerp));
                color2 = lerpLamb (color2, sheet->getColorData().highlight, clampLamb ((currentPixelCol.green() - 128) * sheet->getColorData().lerp));
                color2.setAlpha(clampLamb (currentPixelCol.alpha()) );  // added clamp to alpha, as sometimes it was over 255

                img.setPixelColor(w,h, color2);
            }
        }


    return QPixmap::fromImage(img);
}

QColor Chromer::getDark() const
{
    return vColors[clrNowIndex].dark;
}

QColor Chromer::getBase() const
{
    return vColors[clrNowIndex].base;
}

QColor Chromer::getHighlight() const
{
    return vColors[clrNowIndex].highlight;
}

double Chromer::getLerp() const
{
    return vColors[clrNowIndex].lerp;
}

QStringList Chromer::getColorStrings() const
{
    QStringList list;

    for (const ColorData& colorData : vColors)
        list << colorData.name;

    return list;
}

int Chromer::getColorIndex() const
{
    return clrNowIndex;
}

void Chromer::insertColor(Sheet *sh)
{
    sh->writeColorData(vColors[clrNowIndex]);
}

void Chromer::saveColors(QString fileName)
{
    QDir workingDir (QCoreApplication::applicationDirPath());
    QString path = QCoreApplication::applicationDirPath() + QDir::separator() + fileName;
    QFile colorsFile (path);


    if (!colorsFile.open(QIODevice::WriteOnly | QIODevice::Truncate)){  // truncate deletes text file contents

        qWarning() << "Error when opening hair_colors.txt";
        qWarning() << colorsFile.errorString();
        return;
    }

    QTextStream stream (&colorsFile);

    for (ColorData& hairColor : vColors)
        stream << hairColor.colorsDebugOutput() << "\n\n";

    // file closed on scope end?
}

void Chromer::resetColors(QString fileName)
{

    QDir workingDir (QCoreApplication::applicationDirPath());
    QString filePath = QCoreApplication::applicationDirPath() + QDir::separator() + fileName;

    QFile::remove(filePath);
    QFile::copy(":/color_data/" + fileName, filePath);
    QFile freshlyCopied(filePath);
    freshlyCopied.setPermissions(QFileDevice::WriteOther);

    // retry with newly copied txt from resource
    loadColors(fileName);

    randomizeColor();
}

bool Chromer::readColor(const Sheet *sheet)
{


    for (int i = 0; i < vColors.size(); ++i){

        if (sheet->getColorData().name == vColors[i].name){
            clrNowIndex = i;
            return true;
        }
    }

    // no match found!

    QMessageBox messageBox;
    messageBox.warning(0,"Note","Unknown color read... color temporarily added.\nColor name: " + sheet->getColorData().name + "\nAsset name: " + sheet->getStrings()[sheet->getFrameIndex()]);
    messageBox.setFixedSize(500,200);

    return false;
}






























