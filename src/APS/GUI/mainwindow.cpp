#include "APS/GUI/mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget * parent):
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

void MainWindow::sclickedSlot()
{
  std::cout << "Button pressed!!!!\n";
  ui->textbox1->append("huy\n");
}

MainWindow::~MainWindow()
{
  delete ui;
}
