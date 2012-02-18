#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "videoprocessor.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    vp(this),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&vp, SIGNAL(error(QString)), this, SLOT(onError(QString)));
    connect(ui->averageCountSlider, SIGNAL(valueChanged(int)), &vp, SLOT(setAverageCnt(int)));
    connect(ui->actionFlipHorizontal, SIGNAL(toggled(bool)), &vp, SLOT(setFlipHorizontal(bool)));
    connect(ui->actionFlipVertical, SIGNAL(toggled(bool)), &vp, SLOT(setFlipVertical(bool)));
    connect(ui->actionClear, SIGNAL(triggered()), &vp, SLOT(clear()));
    connect(ui->actionFullScreen, SIGNAL(toggled(bool)), &vp, SLOT(setFullScreen(bool)));
    connect(&vp, SIGNAL(needWaitKey()), this, SLOT(callWaitKey()), Qt::BlockingQueuedConnection);
    vp.start();
}

MainWindow::~MainWindow()
{
    qDebug() << "Main window destructor";
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

void MainWindow::callWaitKey()
{
    if (cv::waitKey(5) == 13)
    {
        ui->actionFullScreen->toggle();
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    qDebug() << "Close main window";
    vp.stop();
    event->accept();
}
