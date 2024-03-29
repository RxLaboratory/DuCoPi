#include "ducolordialog.h"

DuColorDialog::DuColorDialog(QWidget *parent): QColorDialog(parent)
{
    this->setOptions(QColorDialog::DontUseNativeDialog | QColorDialog::ShowAlphaChannel);
    this->setModal(false);
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint | Qt::SubWindow);
    this->setWindowIcon(QIcon(":/icons/app"));

    //Restore custom colors
    QSettings settings;
    int size = settings.beginReadArray("customColors");
    for (int i = 0; i < size; i++)
    {
        settings.setArrayIndex(i);
        QColor c = QColor( settings.value("color", "#000000").toString() );
        this->setCustomColor( i, c );
    }
    settings.endArray();

    clipboard = QGuiApplication::clipboard();

    connect(this, &QColorDialog::colorSelected, this, &DuColorDialog::selectColor);
    connect(this, &QDialog::rejected, this, &DuColorDialog::cancel);

    loadCSS(":/styles/default");
}

void DuColorDialog::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason != QSystemTrayIcon::Context) showHide();
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

void DuColorDialog::selectColor()
{
    addTrayIcon();
    _current = this->currentColor();
    copyColor();
}

void DuColorDialog::copyColor()
{
    clipboard->setText(_current.name());

    //Save custom colors
    saveCustomColors();
}

void DuColorDialog::cancel()
{
    addTrayIcon();
    this->setCurrentColor(_current);
}

bool DuColorDialog::addTrayIcon()
{
    if (trayOk)
    {
        trayIcon->setVisible(true);
        return true;
    }

    trayOk = true;
    //tray icon
    actionShow = new QAction(QIcon(":/icons/show"), "Show");
    actionQuit = new QAction(QIcon(":/icons/quit"), "Quit");
    QMenu *trayMenu = new QMenu("DuCoPi",this);
    trayIcon = new QSystemTrayIcon( QIcon(":/icons/tray"), this );
    trayMenu->addAction(actionShow);
    QAction *copyAction = new QAction(QIcon(":/icons/copy"), "Copy");
    trayMenu->addAction(copyAction);
    trayMenu->addAction(actionQuit);
    trayMenu->addSeparator();
    QAction *exportAction = new QAction(QIcon(":/icons/export"), "Export Palette");
    trayMenu->addAction(exportAction);
    QAction *importAction = new QAction(QIcon(":/icons/import"), "Import Palette");
    trayMenu->addAction(importAction);
    trayMenu->addSeparator();
    bool chat = QString(URL_CHAT) != "";
    bool bugReport = QString(URL_BUGREPORT) != "";
    bool forum = QString(URL_FORUM) != "";
    bool doc = QString(URL_DOC) != "";
    if (doc)
    {
        QAction *docAction = new QAction(QIcon(":/icons/doc"), "Help");
        docAction->setToolTip("Read the documentation");
        docAction->setShortcut(QKeySequence("F1"));
        trayMenu->addAction(docAction);
        if (!chat && !forum && !doc) trayMenu->addSeparator();
        connect(docAction, &QAction::triggered, this, &DuColorDialog::duqf_doc);
    }
    if (bugReport)
    {
        QAction *bugReportAction = new QAction(QIcon(":/icons/bug-report"), "Bug Report");
        bugReportAction->setToolTip("Report a bug");
        trayMenu->addAction(bugReportAction);
        if (!chat && !forum) trayMenu->addSeparator();
        connect(bugReportAction, &QAction::triggered, this, &DuColorDialog::duqf_bugReport);
    }
    if (chat)
    {
        QAction *chatAction = new QAction(QIcon(":/icons/chat"), "Chat");
        chatAction->setToolTip("Come and have a chat");
        trayMenu->addAction(chatAction);
        if (!forum) trayMenu->addSeparator();
        connect(chatAction, &QAction::triggered, this, &DuColorDialog::duqf_chat);
    }
    if (forum)
    {
        QAction *forumAction = new QAction(QIcon(":/icons/forum"), "Forum");
        forumAction->setToolTip("Join us on our forum");
        trayMenu->addAction(forumAction);
        trayMenu->addSeparator();
        connect(forumAction, &QAction::triggered, this, &DuColorDialog::duqf_forum);
    }
    QAction *version = new QAction(QString(STR_INTERNALNAME) + " - " + QString(STR_VERSION));
    version->setEnabled(false);
    trayMenu->addAction(version);
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();
    connect(actionShow, &QAction::triggered, this, &DuColorDialog::show);
    connect(copyAction, &QAction::triggered, this, &DuColorDialog::copyColor);
    connect(actionQuit, &QAction::triggered, qApp, &QApplication::quit);
    connect(importAction, &QAction::triggered, this, &DuColorDialog::importPalette);
    connect(exportAction, &QAction::triggered, this, &DuColorDialog::exportPalette);
#ifdef Q_OS_WIN
    connect(trayIcon, &QSystemTrayIcon::activated, this, &DuColorDialog::trayActivated);
#endif
    this->hide();
    return true;
}

void DuColorDialog::exportPalette()
{
    QString palette = QFileDialog::getSaveFileName( this,
                                                    "Select a color palette",
                                                    "",
                                                    "Gimp Palettes (*.gpl *.GPL);;Text files (*.txt);;All files (*.*)");
    if (palette == "") return;

    DuGimpPalette p(palette);
    for (int i = 0; i < this->customCount(); i++)
    {
        p.addColor( this->customColor(i) );
    }
    p.save();

    this->show();
}

void DuColorDialog::importPalette()
{
    QString palette = QFileDialog::getOpenFileName( this,
                                                    "Select a color palette",
                                                    "",
                                                    "Gimp Palettes (*.gpl *.GPL);;Text files (*.txt);;All files (*.*)");
    if (palette == "") return;

    DuGimpPalette p(palette);
    QMapIterator<QString, QColor> it(p.colors());
    int i = 0;
    while (it.hasNext()) {
        it.next();
        qDebug() << it.key();
        this->setCustomColor(i, it.value());
        i++;
    }
    saveCustomColors();

    this->show();
}

void DuColorDialog::duqf_bugReport()
{
    QDesktopServices::openUrl ( QUrl( URL_BUGREPORT ) );
}

void DuColorDialog::duqf_forum()
{
    QDesktopServices::openUrl ( QUrl( URL_FORUM ) );
}

void DuColorDialog::duqf_chat()
{
    QDesktopServices::openUrl ( QUrl( URL_CHAT ) );
}

void DuColorDialog::duqf_doc()
{
    QDesktopServices::openUrl ( QUrl( URL_DOC ) );
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

void DuColorDialog::saveCustomColors()
{
    QSettings settings;
    settings.beginWriteArray("customColors");
    for (int i = 0; i < this->customCount(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("color", this->customColor(i).name() );
    }
    settings.endArray();
}

