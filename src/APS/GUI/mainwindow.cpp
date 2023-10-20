#include "APS/GUI/mainwindow.hpp"
#include "./ui_mainwindow.h"
#include <iostream>

// #define button

MainWindow::MainWindow(QWidget * parent):
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  _setDefaultValues();

  connect(ui->runStepButton, &QPushButton::clicked, this, &MainWindow::showStepModeTab);
  connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stopMode);
}

void MainWindow::sclickedSlot()
{
  std::cout << "Button pressed!!!!\n";
}

void MainWindow::showStepModeTab()
{
  if (!_checkValues())
  {
    std::cout << "error";
    return;
  }
  std::cout << "show\n";
  ui->tabWidget->setCurrentIndex(1);

  ui->runStepButton->setDisabled(true);
  ui->runAutoButton->setDisabled(true);
  ui->stopButton->setDisabled(false);
}

void MainWindow::stopMode()
{
  std::cout << "stop\n";
  ui->tabWidget->setCurrentIndex(0);

  ui->runStepButton->setDisabled(false);
  ui->runAutoButton->setDisabled(false);
  ui->stopButton->setDisabled(true);
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

bool MainWindow::_checkValues()
{
  return ui->alphaValue->value() < ui->betaValue->value();
}
