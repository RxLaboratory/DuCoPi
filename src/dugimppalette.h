#ifndef DUGIMPPALETTE_H
#define DUGIMPPALETTE_H

#include <QString>
#include <QFile>
#include <QColor>
#include <QMap>
#include <QRegularExpression>
#include <QTextStream>

class DuGimpPalette
{
public:
    DuGimpPalette();
    DuGimpPalette(QString path);

    QMap<QString, QColor> colors() const;
    void addColor(const QColor &c, QString name = "");

    void load(QString path);
    void save(QString path = "");

private:
    QMap<QString, QColor> _colors;
    QFile _paletteFile;

    void parseFile();
    QString channelToString(int channelVal);
};

#endif // DUGIMPPALETTE_H
