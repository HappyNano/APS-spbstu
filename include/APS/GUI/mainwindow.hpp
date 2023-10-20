#ifndef APS_GUI_MAINWINDOW_HPP
#define APS_GUI_MAINWINDOW_HPP

#include <QMainWindow>
#include <QStandardItemModel>

#include "APS/engine.hpp"

QT_BEGIN_NAMESPACE
namespace Ui
{
  class MainWindow;
}
QT_END_NAMESPACE

class MainWindow: public QMainWindow
{
  Q_OBJECT

 public:
  MainWindow(QWidget * parent = nullptr);
  ~MainWindow();

  void showStepModeTab();
  void stopMode();
  void stepMode_stepButton();

 public slots:
  void sclickedSlot();

 private:
  Ui::MainWindow * ui;
  QStandardItemModel * model;

  std::unique_ptr< APS::Engine > _engine_ptr;

  void _setDefaultValues();
  void _clearStepMode();
  bool _checkValues();
};

#endif
