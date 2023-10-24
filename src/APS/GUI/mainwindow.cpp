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

  connect(ui->runStepButton, &QPushButton::clicked, this, &MainWindow::showStepModeTab);
  connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stopMode);
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
    std::cerr << "errorStepMode";
    return;
  }
  _stepMode_init();

  // Show step mode
  ui->tabWidget->setCurrentIndex(1);
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
  return ui->alphaValue->value() < ui->betaValue->value();
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
    newState_str += "Device registered " + QString::number(req.source_id) + "." + QString::number(req.source_req_number) + " request";
    break;
  case APS::EngineEvent::DeviceReleased:
    updateDeviceTable_func();
    newState_str += "Device released " + QString::number(req.source_id) + "." + QString::number(req.source_req_number) + " request";
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
