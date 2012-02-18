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
protected:
    void closeEvent ( QCloseEvent *);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onError(QString mes);
    void radioToggled();
    void callWaitKey();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
