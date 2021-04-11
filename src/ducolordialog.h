#ifndef DUCOLORDIALOG_H
#define DUCOLORDIALOG_H

#include <QColorDialog>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QClipboard>
#include <QtDebug>
#include <QSettings>
#include <QDesktopServices>
#include <QUrl>
#include <QFileDialog>

#include "duqf-app/app-version.h"
#include "dugimppalette.h"

class DuColorDialog : public QColorDialog
{
public:
    DuColorDialog(QWidget *parent = nullptr);
private slots:
    void showHide();
    void selectColor();
    void copyColor();
    void cancel();
    bool addTrayIcon();
    void exportPalette();
    void importPalette();
    void duqf_doc();
    void duqf_bugReport();
    void duqf_forum();
    void duqf_chat();
private:
    QSystemTrayIcon *trayIcon;
    QAction *actionShow;
    QAction *actionQuit;
    QClipboard *clipboard;
    QColor _current;
    bool trayOk = false;

    void loadCSS(QString cssPath, QString styleValues = "");
    void saveCustomColors();

};

#endif // DUCOLORDIALOG_H

