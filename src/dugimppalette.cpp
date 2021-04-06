#include "dugimppalette.h"

DuGimpPalette::DuGimpPalette()
{

}

DuGimpPalette::DuGimpPalette(QString path)
{
    load(path);
}

QMap<QString, QColor> DuGimpPalette::colors() const
{
    return _colors;
}

void DuGimpPalette::addColor(const QColor &c, QString name)
{
    if (name == "") name = c.name();
    _colors[name] = c;
}

void DuGimpPalette::load(QString path)
{
    _paletteFile.setFileName(path);
    parseFile();
}

void DuGimpPalette::save(QString path)
{
    if (path == "") path = _paletteFile.fileName();
    if (!path.endsWith(".gpl")) path = path + ".gpl";
    _paletteFile.setFileName(path);

    if (!_paletteFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&_paletteFile);
    out << "GIMP Palette" << endl;
    out << "Name: DuCoPi" << endl;
    out << "#" << endl;

    QMapIterator<QString, QColor> i(_colors);
    while (i.hasNext()) {
        i.next();
        QColor c = i.value();
        QString r = channelToString( c.red() );
        QString g = channelToString( c.green() );
        QString b = channelToString( c.blue() );
        out << r << " " << g << " " << b << "\t" << i.key() << endl;
    }
}

void DuGimpPalette::parseFile()
{
    _colors.clear();

    if (!_paletteFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QRegularExpression re("\\s*(\\d{1,3})\\s+(\\d{1,3})\\s+(\\d{1,3})\\s+(.*)");

    while (!_paletteFile.atEnd()) {
        QString line = _paletteFile.readLine();
        QRegularExpressionMatch reMatch = re.match(line);
        int i = 0;
        if (reMatch.hasMatch())
        {
            i++;
            QString r = reMatch.captured(1);
            QString g = reMatch.captured(2);
            QString b = reMatch.captured(3);
            QString name = reMatch.captured(4);
            QColor c( r.toInt(), g.toInt(), b.toInt());
            if (name == "") name = QString::number(i);
            addColor(c, name);
        }
    }
}

QString DuGimpPalette::channelToString(int channelVal)
{
    QString v = QString::number(channelVal);
    while(v.count() < 3)
    {
        v = " " + v;
    }
    return v;
}
