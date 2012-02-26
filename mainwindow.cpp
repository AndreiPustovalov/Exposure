#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "videoprocessor.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>
#include "calc.h"
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    vp(this),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&vp, SIGNAL(error(QString)), this, SLOT(onError(QString)));
    connect(ui->averageCountSlider, SIGNAL(valueChanged(int)), &vp, SLOT(setAverageCnt(int)), Qt::DirectConnection);
    connect(ui->actionFlipHorizontal, SIGNAL(toggled(bool)), &vp, SLOT(setFlipHorizontal(bool)), Qt::DirectConnection);
    connect(ui->actionFlipVertical, SIGNAL(toggled(bool)), &vp, SLOT(setFlipVertical(bool)), Qt::DirectConnection);
    connect(ui->actionClear, SIGNAL(triggered()), &vp, SLOT(clear()), Qt::DirectConnection);
    connect(ui->actionFullScreen, SIGNAL(toggled(bool)), &vp, SLOT(setFullScreen(bool)), Qt::DirectConnection);
    connect(&vp.getWnd(), SIGNAL(showImage(std::string&,cv::Mat&)), this, SLOT(showImage(std::string&,cv::Mat&)), Qt::BlockingQueuedConnection);
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
    if (ui->infAverageModeRadio->isChecked())
        m = VideoProcessor::InfAverageMode;

    vp.setMode(m);
}

void MainWindow::showImage(std::string &wname, cv::Mat &img)
{
    cv::imshow(wname, img);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    qDebug() << "Close main window";
    vp.stop();
    event->accept();
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    bool ok;
    ui->debugLbl->setText(QString().setNum(Calc::Instance().evaluate(arg1, ok)));
    if (!ok)
        ui->debugLbl->setText("err");
}
