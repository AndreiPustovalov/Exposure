#ifndef CVWINDOW_H
#define CVWINDOW_H

#include <QObject>
#include <opencv/highgui.h>

class CvWindow : public QObject
{
    Q_OBJECT
private:
    std::string nm;
    bool destroyed;
public:
    explicit CvWindow(QObject *parent = 0, std::string& name = std::string("VideoWindow"));
    ~CvWindow();
    
    std::string& name();
    void imshow(cv::Mat& img);
    void destroyWindow();
    void setFullScreen(bool fullScreen);
    bool getFullScreen();
signals:
    void showImage(std::string& wname, cv::Mat &img);
public slots:
    
};

#endif // CVWINDOW_H
