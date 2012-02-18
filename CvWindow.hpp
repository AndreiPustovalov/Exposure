#ifndef CVWINDOW_HPP
#define CVWINDOW_HPP

#include <opencv/highgui.h>

class CvWindow
{
private:
    std::string nm;
    bool present;
public:
    CvWindow(std::string name) : nm(name)
    {
        cv::namedWindow(name, CV_WINDOW_NORMAL | CV_WINDOW_FREERATIO | CV_GUI_NORMAL);
        present = true;
    }
    CvWindow(std::string& name) : nm(name)
    {
        cv::namedWindow(name);
    }
    ~CvWindow()
    {
        if (present)
            destroyWindow();
    }

    std::string& name()
    {
        return nm;
    }
    void imshow(cv::Mat img)
    {
        cv::imshow(nm, img);
    }
    void destroyWindow()
    {
        cv::destroyWindow(nm);
        present = false;
    }
    void setFullScreen(bool fullScreen)
    {
        if (fullScreen)
            cv::setWindowProperty(nm, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
        else
            cv::setWindowProperty(nm, CV_WND_PROP_FULLSCREEN, CV_WINDOW_NORMAL);
    }
    bool getFullScreen()
    {
        return cv::getWindowProperty(nm, CV_WND_PROP_FULLSCREEN) == CV_WINDOW_FULLSCREEN;
    }
};

#endif // CVWINDOW_HPP
