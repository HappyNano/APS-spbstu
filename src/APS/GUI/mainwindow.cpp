#include "APS/GUI/mainwindow.hpp"
#include "./ui_mainwindow.h"
#include <iostream>
#include <string>

// #define button

MainWindow::MainWindow(QWidget * parent):
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  _setDefaultValues();

  connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stopMode);
  connect(ui->runStepButton, &QPushButton::clicked, this, &MainWindow::showStepModeTab);
  connect(ui->runAutoButton, &QPushButton::clicked, this, &MainWindow::showAutoModeTab);
  connect(ui->oneStepButton, &QPushButton::clicked, this, &MainWindow::stepMode_stepButton);
  connect(ui->setDefaultButton, &QPushButton::clicked, this, &MainWindow::_setDefaultValues);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::showStepModeTab()
{
  if (!_checkValues())
  {
    std::cerr << "errorStepMode\n";
    return;
  }
  _stepMode_init();

  // Show step mode
  ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::showAutoModeTab()
{
  if (!_checkValues())
  {
    std::cerr << "errorAutoMode\n";
    return;
  }
  _autoMode_init();

  // Show auto mode
  ui->tabWidget->setCurrentIndex(2);
  while (_engine_ptr->getProcessed() + _engine_ptr->getRejected() < ui->countRequestsValue->value())
  {
    _engine_ptr->step();
  }
  _autoMode_showStatistic();
}

void MainWindow::stepMode_stepButton()
{
  for (int i = 0; i < ui->stepMode_stepValue->value(); ++i)
  {
    _engine_ptr->step();
    ui->createdValueLabel->setText(QString::number(_engine_ptr->getCreated()));
    ui->processedValueLabel->setText(QString::number(_engine_ptr->getProcessed()));
    ui->rejectedValueLabel->setText(QString::number(_engine_ptr->getRejected()));
    ui->stepMode_time->setText(QString::number(_engine_ptr->timeManager()->timeNow()));
  }
}

void MainWindow::stopMode()
{
  _engine_ptr.reset();
  ui->tabWidget->setCurrentIndex(0);

  ui->runStepButton->setDisabled(false);
  ui->runAutoButton->setDisabled(false);
  ui->stopButton->setDisabled(true);

  ui->oneStepButton->setDisabled(true);
}

void MainWindow::_setDefaultValues()
{
  ui->sourcesValue->setValue(3);
  ui->bufferSizeValue->setValue(4);
  ui->devicesValue->setValue(3);
  ui->alphaValue->setValue(1);
  ui->betaValue->setValue(2);
  ui->lambdaValue->setValue(1);
}

bool MainWindow::_checkValues()
{
  return ui->alphaValue->value() < ui->betaValue->value() && ui->countRequestsValue->value() >= 1;
}

void MainWindow::_stepMode_ChangeEvent(APS::EngineEvent event, const APS::Request & req)
{
  static auto updateBufferTable_func = [this]() -> void
  {
    int i = 0;
    for (auto && buf_item: _engine_ptr->buffer()->getBuffer())
    {
      if (buf_item.has_value())
      {
        ui->bufferTable->setItem(i,
         1,
         new QTableWidgetItem(QString::number(buf_item->second.source_id) + "." + QString::number(buf_item->second.source_req_number)));
        ui->bufferTable->setItem(i, 2, new QTableWidgetItem("Busy"));
      }
      else
      {
        ui->bufferTable->setItem(i, 1, new QTableWidgetItem(""));
        ui->bufferTable->setItem(i, 2, new QTableWidgetItem(""));
      }
      ++i;
    }
  };
  static auto updateDeviceTable_func = [this]() -> void
  {
    int i = 0;
    for (auto && device: _engine_ptr->deviceManager().getDevices())
    {
      if (!device.isAvaible())
      {
        ui->devicesTable->setItem(i,
         1,
         new QTableWidgetItem(
          QString::number(device.getRequest()->source_id) + "." + QString::number(device.getRequest()->source_req_number)));
        ui->devicesTable->setItem(i, 2, new QTableWidgetItem("Busy"));
      }
      else
      {
        ui->devicesTable->setItem(i, 1, new QTableWidgetItem(""));
        ui->devicesTable->setItem(i, 2, new QTableWidgetItem(""));
      }
      ++i;
    }
  };

  ui->calendarTable->insertRow(ui->calendarTable->rowCount());
  ui->calendarTable->setItem(ui->calendarTable->rowCount() - 1,
   0,
   new QTableWidgetItem(QString::number(_engine_ptr->timeManager()->timeNow())));
  QString newState_str{ "" };
  switch (event)
  {
  case APS::EngineEvent::SourceGenerated:
    newState_str += "Request " + QString::number(req.source_id) + "." + QString::number(req.source_req_number) + " generated";
    break;
  case APS::EngineEvent::BufferRegistered:
    updateBufferTable_func();
    newState_str += "Buffer registered " + QString::number(req.source_id) + "." + QString::number(req.source_req_number) + " request";
    break;
  case APS::EngineEvent::BufferReleased:
    updateBufferTable_func();
    newState_str += "Buffer released " + QString::number(req.source_id) + "." + QString::number(req.source_req_number) + " request";
    break;
  case APS::EngineEvent::DeviceRegistered:
    updateDeviceTable_func();
    newState_str += "Device[" + QString::number(req.device_id) + "] registered " + QString::number(req.source_id) + "." +
                    QString::number(req.source_req_number) + " request";
    break;
  case APS::EngineEvent::DeviceReleased:
    updateDeviceTable_func();
    newState_str += "Device[" + QString::number(req.device_id) + "] released " + QString::number(req.source_id) + "." +
                    QString::number(req.source_req_number) + " request";
    break;
  case APS::EngineEvent::RequestRejected:
    newState_str += "Request " + QString::number(req.source_id) + "." + QString::number(req.source_req_number) + " rejected";
    break;
  }

  ui->calendarTable->setItem(ui->calendarTable->rowCount() - 1, 1, new QTableWidgetItem(newState_str));
  ui->calendarTable->scrollToBottom();
}

void MainWindow::_stepMode_init_bufferTable()
{
  const auto bufferSize = ui->bufferSizeValue->value();

  ui->bufferTable->setColumnCount(3);
  ui->bufferTable->setRowCount(bufferSize);
  for (int i = 0; i < bufferSize; ++i)
  {
    ui->bufferTable->setItem(i, 0, new QTableWidgetItem(std::to_string(i).c_str()));
  }
  ui->bufferTable->setHorizontalHeaderLabels(
   QStringList() << "id"
                 << "Request"
                 << "State");
}

void MainWindow::_stepMode_init_devicesTable()
{
  const auto devicesSize = ui->devicesValue->value();

  ui->devicesTable->setColumnCount(3);
  ui->devicesTable->setRowCount(devicesSize);
  for (int i = 0; i < devicesSize; ++i)
  {
    ui->devicesTable->setItem(i, 0, new QTableWidgetItem(std::to_string(i).c_str()));
  }
  ui->devicesTable->setHorizontalHeaderLabels(
   QStringList() << "id"
                 << "Request"
                 << "State");
}

void MainWindow::_stepMode_init_calendarTable()
{
  ui->calendarTable->setColumnCount(2);
  ui->calendarTable->setHorizontalHeaderLabels(QStringList() << "time"
                                                             << "Event");
  ui->calendarTable->setColumnWidth(0, 61);
  ui->calendarTable->setColumnWidth(1, 232);
}

void MainWindow::_stepMode_init()
{
  // Creating engine ptr
  const auto sourcesSize = ui->sourcesValue->value();
  const auto bufferSize = ui->bufferSizeValue->value();
  const auto devicesSize = ui->devicesValue->value();

  _engine_ptr = std::make_unique< APS::Engine >(sourcesSize,
   bufferSize,
   devicesSize,
   ui->alphaValue->value(),
   ui->betaValue->value(),
   ui->lambdaValue->value());

  // Connecting to engine
  for (int eventInt = APS::EngineEvent::SourceGenerated; eventInt <= APS::EngineEvent::RequestRejected; ++eventInt)
  {
    APS::EngineEvent event = static_cast< APS::EngineEvent >(eventInt);
    _engine_ptr->_update_subs.subscribe(event,
     [this, event](const APS::Request & req)
     {
       this->_stepMode_ChangeEvent(event, req);
     });
  }

  // Clearing all
  _stepMode_clear();

  // Disabling/enabling buttons
  ui->runStepButton->setDisabled(true);
  ui->runAutoButton->setDisabled(true);
  ui->stopButton->setDisabled(false);

  ui->oneStepButton->setDisabled(false);

  // Set up tables
  _stepMode_init_bufferTable();
  _stepMode_init_devicesTable();
  _stepMode_init_calendarTable();
}

void MainWindow::_stepMode_clear()
{
  ui->createdValueLabel->setText("0");
  ui->processedValueLabel->setText("0");
  ui->rejectedValueLabel->setText("0");
  ui->stepMode_time->setText("0");
  ui->bufferTable->clear();
  ui->bufferTable->setRowCount(0);
  ui->devicesTable->clear();
  ui->devicesTable->setRowCount(0);
  ui->calendarTable->clear();
  ui->calendarTable->setRowCount(0);
}

void MainWindow::_autoMode_showStatistic()
{
  double p_reject = 1.f * _engine_ptr->getRejected() / _engine_ptr->getCreated() * 100;
  ui->createdValueLabel_AutoMode->setText(QString::number(_engine_ptr->getCreated()));
  ui->processedValueLabel_AutoMode->setText(QString::number(_engine_ptr->getProcessed()));
  ui->rejectedValueLabel_AutoMode->setText(QString::number(_engine_ptr->getRejected()) + " (" + QString::number(p_reject) + "%)");

  auto stat = _engine_ptr->collectStat();
  for (auto && source: stat.sources())
  {
    ui->autoMode_sourcesTable->insertRow(ui->autoMode_sourcesTable->rowCount());
    ui->autoMode_sourcesTable->setItem(ui->autoMode_sourcesTable->rowCount() - 1,
     0,
     new QTableWidgetItem(QString("И") + QString::number(source.first)));
    ui->autoMode_sourcesTable->setItem(ui->autoMode_sourcesTable->rowCount() - 1,
     1,
     new QTableWidgetItem(QString::number(source.second.countCreated())));
    ui->autoMode_sourcesTable->setItem(ui->autoMode_sourcesTable->rowCount() - 1,
     2,
     new QTableWidgetItem(QString::number(source.second.p_rejected())));
    ui->autoMode_sourcesTable->setItem(ui->autoMode_sourcesTable->rowCount() - 1,
     3,
     new QTableWidgetItem(QString::number(source.second.average_TimeIn())));
    ui->autoMode_sourcesTable->setItem(ui->autoMode_sourcesTable->rowCount() - 1,
     4,
     new QTableWidgetItem(QString::number(source.second.average_TimeBuffered())));
    ui->autoMode_sourcesTable->setItem(ui->autoMode_sourcesTable->rowCount() - 1,
     5,
     new QTableWidgetItem(QString::number(source.second.average_TimeService())));
    ui->autoMode_sourcesTable->setItem(ui->autoMode_sourcesTable->rowCount() - 1,
     6,
     new QTableWidgetItem(QString::number(source.second.dispersion_TimeBuffered())));
    ui->autoMode_sourcesTable->setItem(ui->autoMode_sourcesTable->rowCount() - 1,
     7,
     new QTableWidgetItem(QString::number(source.second.dispersion_TimeService())));
  }

  for (auto && device: stat.devices())
  {
    ui->autoMode_devicesTable->insertRow(ui->autoMode_devicesTable->rowCount());
    ui->autoMode_devicesTable->setItem(ui->autoMode_devicesTable->rowCount() - 1,
     0,
     new QTableWidgetItem(QString("П") + QString::number(device.first)));
    ui->autoMode_devicesTable->setItem(ui->autoMode_devicesTable->rowCount() - 1,
     1,
     new QTableWidgetItem(QString::number(device.second.usingCoef())));
  }
}

void MainWindow::_autoMode_init_sourcesTable()
{
  ui->autoMode_sourcesTable->setColumnCount(8);
  ui->autoMode_sourcesTable->setHorizontalHeaderLabels(
   QStringList() << "source_id"
                 << "Request count"
                 << "P reject"
                 << "T in"
                 << "T buffered"
                 << "T service"
                 << "D buffered"
                 << "D service");
}

void MainWindow::_autoMode_init_devicesTable()
{
  ui->autoMode_devicesTable->setColumnCount(2);
  ui->autoMode_devicesTable->setHorizontalHeaderLabels(QStringList() << "device id"
                                                                     << "using coef");
}

void MainWindow::_autoMode_init()
{
  // Creating engine ptr
  const auto sourcesSize = ui->sourcesValue->value();
  const auto bufferSize = ui->bufferSizeValue->value();
  const auto devicesSize = ui->devicesValue->value();

  _engine_ptr = std::make_unique< APS::Engine >(sourcesSize,
   bufferSize,
   devicesSize,
   ui->alphaValue->value(),
   ui->betaValue->value(),
   ui->lambdaValue->value(),
   ui->countRequestsValue->value());

  _autoMode_clear();

  // Disabling/enabling buttons
  ui->runStepButton->setDisabled(true);
  ui->runAutoButton->setDisabled(true);
  ui->stopButton->setDisabled(false);

  _autoMode_init_sourcesTable();
  _autoMode_init_devicesTable();
}

void MainWindow::_autoMode_clear()
{
  ui->createdValueLabel_AutoMode->setText("0");
  ui->processedValueLabel_AutoMode->setText("0");
  ui->rejectedValueLabel_AutoMode->setText("0");
  ui->autoMode_devicesTable->clear();
  ui->autoMode_devicesTable->setRowCount(0);
  ui->autoMode_sourcesTable->clear();
  ui->autoMode_sourcesTable->setRowCount(0);
}
