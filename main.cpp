#include <QApplication>
#include <QSettings>
#include <QStyle>
#include <QStyleFactory>
#include <QTextCodec>
#include <QTimer>

#include "supplywriter.h"

SupplyWriter* writer;

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("fusion"));

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QCoreApplication::setOrganizationName("SupplyWriter");
    QCoreApplication::setApplicationName("user");

    QSettings setting;
    setting.setValue("desc", "supply writer user info");

    SupplyWriter w;
    writer = &w;
    w.show();

    return a.exec();
}
