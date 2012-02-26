#include "cvwindow.h"

CvWindow::CvWindow(QObject *parent, std::string &name) :
    QObject(parent),
    nm(name),
    destroyed(false)
{
    cv::namedWindow(name, CV_WINDOW_NORMAL | CV_WINDOW_FREERATIO | CV_GUI_NORMAL);
}

CvWindow::~CvWindow()
{
    destroyWindow();
}

std::string& CvWindow::name()
{
    return nm;
}

void CvWindow::imshow(cv::Mat& img)
{
    emit showImage(nm, img);
}

void CvWindow::destroyWindow()
{
    if (!destroyed)
    {
        cv::destroyWindow(nm);
        destroyed = true;
    }
}

void CvWindow::setFullScreen(bool fullScreen)
{
    if (fullScreen)
        cv::setWindowProperty(nm, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    else
        cv::setWindowProperty(nm, CV_WND_PROP_FULLSCREEN, CV_WINDOW_NORMAL);
}

bool CvWindow::getFullScreen()
{
    return cv::getWindowProperty(nm, CV_WND_PROP_FULLSCREEN) == CV_WINDOW_FULLSCREEN;
}
