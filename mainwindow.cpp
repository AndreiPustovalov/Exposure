#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv/highgui.h>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QFileDialog fd(this, tr("Browse"), QString(), tr("Images (*.bmp *.jpg *.png)"));
    if (fd.exec())
    {
        cv::namedWindow("qwe");
        cv::imshow("qwe", cv::imread(fd.selectedFiles().front().toStdString()));
    }
}
