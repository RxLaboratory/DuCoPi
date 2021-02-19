#include "ducolordialog.h"

DuColorDialog::DuColorDialog(QWidget *parent): QColorDialog(parent)
{
    this->setOptions(QColorDialog::DontUseNativeDialog);
    this->setModal(false);
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint | Qt::SubWindow);
    this->setWindowIcon(QIcon(":/icons/app"));

    actionShow = new QAction("Show");
    actionQuit = new QAction("Quit");


    if (QSystemTrayIcon::isSystemTrayAvailable())
    {
        //tray icon
        QMenu *trayMenu = new QMenu("DuCoPi",this);
        QSystemTrayIcon *trayIcon = new QSystemTrayIcon( QIcon(":/icons/tray"), this );
        trayMenu->addAction(actionShow);
        trayMenu->addAction(actionQuit);
        trayIcon->setContextMenu(trayMenu);
        trayIcon->show();
        connect(actionShow, &QAction::triggered, this, &DuColorDialog::show);
        connect(actionQuit, &QAction::triggered, qApp, &QApplication::quit);
    }

    clipboard = QGuiApplication::clipboard();

    connect(this, &QColorDialog::colorSelected, this, &DuColorDialog::copyColor);


    loadCSS(":/styles/default");
}

void DuColorDialog::showHide()
{
    if (this->isVisible())
    {
        this->hide();
    }
    else
    {
        this->show();
    }
}

void DuColorDialog::copyColor()
{
    clipboard->setText(this->currentColor().name());
}

void DuColorDialog::loadCSS(QString cssPath, QString styleValues)
{
    QString css = "";
    QFile cssFile(cssPath);

    if (cssFile.exists())
    {
        if (cssFile.open(QFile::ReadOnly))
        {
            css += QString(cssFile.readAll());
            cssFile.close();
        }
    }

    //replace variables
    //find values
    QFileInfo cssInfo(cssFile);
    if (styleValues =="")
    {
        styleValues = cssInfo.path() + "/" + cssInfo.completeBaseName() + "-values";
    }
    QFile valuesFile(styleValues);
    if (valuesFile.exists())
    {
        if (valuesFile.open(QFile::ReadOnly))
        {
            css += "\n";
            //read lines
            while(!valuesFile.atEnd())
            {
                QString line = valuesFile.readLine();

                //get value and name
                QRegularExpression re("(@\\S+) *= *(\\S+)");
                QRegularExpressionMatch match = re.match(line);
                if (match.hasMatch())
                {
                    QString name = match.captured(1);
                    QString value = match.captured(2);
                    css.replace(name,value);
                }
            }
        }
    }

    qApp->setStyleSheet(css);
}
