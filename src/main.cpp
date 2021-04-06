
#include <QApplication>
#include <QMenu>
#include <QSystemTrayIcon>

#include "ducolordialog.h"
#include "duqf-app/app-version.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setQuitOnLastWindowClosed(false);

    // settings
    QCoreApplication::setOrganizationName(STR_COMPANYNAME);
    QCoreApplication::setOrganizationDomain(STR_COMPANYDOMAIN);
    QCoreApplication::setApplicationName(STR_PRODUCTNAME);
    QCoreApplication::setApplicationVersion(STR_VERSION);

    //color picker
    DuColorDialog *cd = new DuColorDialog();

    cd->show();

    return a.exec();
}
