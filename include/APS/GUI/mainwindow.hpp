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
  void showAutoModeTab();
  void stopMode();
  void stepMode_stepButton();

 private:
  Ui::MainWindow * ui;
  QStandardItemModel * model;

  std::unique_ptr< APS::Engine > _engine_ptr;

  void _setDefaultValues();
  void _clearStepMode();
  bool _checkValues();

  // StepMode functions
  void _stepMode_ChangeEvent(APS::EngineEvent event, const APS::Request & req);
  void _stepMode_init_bufferTable();
  void _stepMode_init_devicesTable();
  void _stepMode_init_calendarTable();
  void _stepMode_init();
  void _stepMode_clear();

  // AutoMode functions
  void _autoMode_showStatistic();
  void _autoMode_init_sourcesTable();
  void _autoMode_init_devicesTable();
  void _autoMode_init();
  void _autoMode_clear();
};

#endif
