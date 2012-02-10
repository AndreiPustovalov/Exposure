#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "videoprocessor.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    vp(this),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&vp, SIGNAL(error(QString)), this, SLOT(onError(QString)));
    vp.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onError(QString mes)
{
    QMessageBox::critical(this, tr("Error"), mes);
    setEnabled(false);
}
