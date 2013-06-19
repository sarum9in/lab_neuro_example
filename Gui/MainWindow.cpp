#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, tr("About"), tr("Neural networks application example for boolean functions"));
}

void MainWindow::showAboutQt()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}
