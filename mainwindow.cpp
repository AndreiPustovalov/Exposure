#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "videoprocessor.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>
#include "calc.h"
#include <QTime>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    vp(this),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, QApplication::applicationDirPath());
    QSettings sets(QSettings::IniFormat, QSettings::SystemScope, "APSoft", "Exposure");
    int size = sets.beginReadArray("Conditions");
    for (int i = 0; i < size; ++i)
    {
        sets.setArrayIndex(i);
        ui->conditionEdit->addItem(sets.value("val").toString());
    }
    sets.endArray();

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
    QSettings sets(QSettings::IniFormat, QSettings::SystemScope, "APSoft", "Exposure");
    sets.beginWriteArray("Conditions");
    for (int i = 0; i < ui->conditionEdit->count(); ++i)
    {
        sets.setArrayIndex(i);
        sets.setValue("val", ui->conditionEdit->itemText(i));
    }
    sets.endArray();
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
void MainWindow::on_setCondition_clicked()
{
    vp.setCondition(ui->conditionEdit->currentText());
    ui->conditionEdit->addItem(ui->conditionEdit->currentText());
}
