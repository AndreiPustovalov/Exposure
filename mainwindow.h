#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "videoprocessor.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    VideoProcessor vp;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onError(QString mes);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
