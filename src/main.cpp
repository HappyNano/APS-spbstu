#include <iostream>
#include <string>

#include "APS/engine.hpp"

#define SOURCES 3
#define BUFFER 4
#define DEVICES 2
#define ALPHA 1
#define BETA 2
#define LAMBDA 5

void run()
{
  APS::Engine engine(SOURCES, BUFFER, DEVICES, ALPHA, BETA, LAMBDA);
  engine.run();
}

#include "APS/GUI/mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char * argv[])
{
  QApplication a(argc, argv);

  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString & locale: uiLanguages)
  {
    const QString baseName = "aps_" + QLocale(locale).name();
    if (translator.load(":/i18n/" + baseName))
    {
      a.installTranslator(&translator);
      break;
    }
  }
  MainWindow w;

  w.show();
  return a.exec();
}