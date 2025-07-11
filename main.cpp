#include "mainwindow.h"
#include "backend.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "networkvisualizer.h"
#include "layerblockitem.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "CodeWings_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w;
    MainWindow::setInstance(&w);
    w.show();
    return a.exec();
}
