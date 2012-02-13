#ifndef CVWINDOW_HPP
#define CVWINDOW_HPP

#include <opencv/highgui.h>

class CvWindow
{
private:
    std::string nm;
public:
    CvWindow(std::string name) : nm(name)
    {
        cv::namedWindow(name, CV_WINDOW_NORMAL | CV_WINDOW_FREERATIO /*| CV_GUI_NORMAL*/);
    }
    CvWindow(std::string& name) : nm(name)
    {
        cv::namedWindow(name);
    }
    ~CvWindow()
    {
        cv::destroyWindow(nm);
    }

    std::string& name()
    {
        return nm;
    }
    void imshow(cv::Mat img)
    {
        cv::imshow(nm, img);
        cv::waitKey(1);
    }
};

#endif // CVWINDOW_HPP
