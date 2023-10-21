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

void MainWindow::sclickedSlot()
{
  std::cout << "Button pressed!!!!\n";
}

void MainWindow::showStepModeTab()
{
  if (!_checkValues())
  {
    std::cerr << "errorStepMode";
    return;
  }

  const auto sourcesSize = ui->sourcesValue->value();
  const auto bufferSize = ui->bufferSizeValue->value();
  const auto devicesSize = ui->devicesValue->value();

  _engine_ptr = std::make_unique< APS::Engine >(sourcesSize,
   bufferSize,
   devicesSize,
   ui->alphaValue->value(),
   ui->betaValue->value(),
   ui->lambdaValue->value());

  _clearStepMode();

  // Disabling/enabling buttons
  ui->runStepButton->setDisabled(true);
  ui->runAutoButton->setDisabled(true);
  ui->stopButton->setDisabled(false);

  ui->oneStepButton->setDisabled(false);

  // Set up tables
  auto bufferTableHeaders = QStringList() << "id"
                                          << "State";
  ui->bufferTable->setColumnCount(2);
  ui->bufferTable->setRowCount(bufferSize);
  for (int i = 0; i < bufferSize; ++i)
  {
    ui->bufferTable->setItem(i, 0, new QTableWidgetItem(std::to_string(i).c_str()));
  }
  ui->bufferTable->setHorizontalHeaderLabels(bufferTableHeaders);
  // ui->bufferTable->hideColumn(0);

  ui->devicesTable->clear();

  ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::stepMode_stepButton()
{
  _engine_ptr->step();
  // _engine_ptr->
  ui->createdValueLabel->setText(std::to_string(_engine_ptr->getCreated()).c_str());
  ui->processedValueLabel->setText(std::to_string(_engine_ptr->getProcessed()).c_str());
  ui->rejectedValueLabel->setText(std::to_string(_engine_ptr->getRejected()).c_str());
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

MainWindow::~MainWindow()
{
  delete ui;
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

void MainWindow::_clearStepMode()
{
  ui->createdValueLabel->setText("0");
  ui->processedValueLabel->setText("0");
  ui->rejectedValueLabel->setText("0");
  ui->bufferTable->clear();
  ui->devicesTable->clear();
}

bool MainWindow::_checkValues()
{
  return ui->alphaValue->value() < ui->betaValue->value();
}
