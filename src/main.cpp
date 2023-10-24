#include <iostream>
#include <string>

#include "APS/engine.hpp"

#define SOURCES 3
#define BUFFER 4
#define DEVICES 3
#define ALPHA 1
#define BETA 1.5
#define LAMBDA 5

int run()
{
  APS::Engine engine(SOURCES, BUFFER, DEVICES, ALPHA, BETA, LAMBDA);
  engine.run();
  return 0;
}

#include "APS/GUI/mainwindow.hpp"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char * argv[])
{
  // return run();
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