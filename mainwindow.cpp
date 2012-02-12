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
    connect(ui->averageCountSlider, SIGNAL(valueChanged(int)), &vp, SLOT(setAverageCnt(int)));
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

void MainWindow::radioToggled()
{
    VideoProcessor::Mode m;
    if (ui->simpleModeRadio->isChecked())
        m = VideoProcessor::SimpleMode;
    if (ui->averageModeRadio->isChecked())
        m = VideoProcessor::AverageMode;
    if (ui->dashModeRadio->isChecked())
        m = VideoProcessor::DashMode;
    if (ui->infAverageModeRadio->isChecked())
        m = VideoProcessor::InfAverageMode;

    vp.setMode(m);
}
