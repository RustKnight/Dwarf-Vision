#ifndef COLORDATA_H
#define COLORDATA_H


#include <QColor>
#include <QTextStream>
#include <QDataStream>


struct ColorData {

    friend QDataStream &operator << (QDataStream & out, const ColorData& entry) {

        out << entry.dark;
        out << entry.base;
        out << entry.highlight;
        out << entry.lerp;
        out << entry.name;

        return out;
    }
    friend QDataStream &operator >> (QDataStream & in, ColorData& entry) {

        in >> entry.dark;
        in >> entry.base;
        in >> entry.highlight;
        in >> entry.lerp;
        in >> entry.name;

        return in;
    }


    QColor dark;
    QColor base;
    QColor highlight;
    double lerp = 3.0;

    QString name = "not assigned";

    QString colorsDebugOutput() const {

        QString test;
        QTextStream result(&test);

        result << name << "\n\n";

        result << "Lerp value: " << lerp << "\n\n";


        result << "Dark\t\t"
               << " " << dark.red()
               << " " << dark.green()
               << " " << dark.blue()
               << "\n";

        result << "Base\t\t"
               << " " << base.red()
               << " " << base.green()
               << " " << base.blue()
               << "\n";


        result << "Highlight\t"
               << " " << highlight.red()
               << " " << highlight.green()
               << " " << highlight.blue()
               << "\n";

        return result.readAll();
    }

};












#endif // COLORDATA_H
