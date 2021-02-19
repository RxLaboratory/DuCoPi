
#include <QApplication>
#include <QMenu>
#include <QSystemTrayIcon>

#include "ducolordialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //color picker
    DuColorDialog *cd = new DuColorDialog();

    cd->show();

    return a.exec();
}
