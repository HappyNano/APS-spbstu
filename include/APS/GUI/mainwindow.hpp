#ifndef APS_GUI_MAINWINDOW_HPP
#define APS_GUI_MAINWINDOW_HPP

#include <QMainWindow>
#include <QStandardItemModel>

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

 public slots:
  void sclickedSlot();

 private:
  Ui::MainWindow * ui;
  QStandardItemModel * model;

  void _setDefaultValues();
  bool _checkValues();
};

#endif
