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

class DuColorDialog : public QColorDialog
{
public:
    DuColorDialog(QWidget *parent = nullptr);
private slots:
    void showHide();
    void copyColor();
private:
    QAction *actionShow;
    QAction *actionQuit;
    QClipboard *clipboard;

    void loadCSS(QString cssPath, QString styleValues = "");
};

#endif // DUCOLORDIALOG_H

