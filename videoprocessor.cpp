#include "videoprocessor.h"
#include <opencv/highgui.h>
#include "CvWindow.hpp"
#include <algorithm>
#include <QDebug>

VideoProcessor::VideoProcessor(QObject *parent) :
    QThread(parent),
    average_cnt(10),
    mode(SimpleMode)
{
}

void VideoProcessor::run()
{
//    cv::VideoCapture cap(0);
    class MyCap
    {
    private:
        int n;
    public:
        MyCap() : n(1) {}
        bool isOpened()
        {
            return true;
        }
        MyCap& operator>>(cv::Mat& img)
        {
            img = cv::imread(QString("E:\\My Documents\\Programming\\Projects\\Exposure\\imgs\\%1.jpg").arg((n==495)?n=1:n++).toStdString(), -1);
            return *this;
        }
    } cap;

    if (!cap.isOpened())
    {
            emit error(tr("Can't open video capture device"));
            return;
    }
    CvWindow wnd("Video Window");
    std::list<cv::Mat> buf;
    cv::Mat sum;
    while (true)
    {
        cv::Mat img, res;
        cap >> img;
        int aver_cnt = average_cnt;
        switch (mode)
        {
        case AverageMode:
            if (sum.empty())
                sum = cv::Mat::zeros(img.rows, img.cols, img.type());
            if ((int)buf.size() < aver_cnt)
            {
                for (int y = 0; y < img.rows; ++y)
                {
                    uchar* sumRow = sum.ptr(y);
                    const uchar* imgRow = img.ptr(y);
                    for (int x = 0; x < img.cols*img.channels(); ++x)
                    {
                        sumRow[x] = (buf.size()*sumRow[x] + imgRow[x])/(buf.size()+1);
                    }
                }
            }else
            {
                cv::Mat frnt = buf.front();
                for (int y = 0; y < img.rows; ++y)
                {
                    uchar* sumRow = sum.ptr(y);
                    const uchar* imgRow = img.ptr(y);
                    const uchar* frontRow = frnt.ptr(y);
                    for (int x = 0; x < img.cols*img.channels(); ++x)
                    {
                        sumRow[x] = std::min(255,std::max(0,(aver_cnt*sumRow[x] + imgRow[x] - frontRow[x])/(aver_cnt)));
                    }
                }
                buf.pop_front();
                if ((int)buf.size() > aver_cnt)
                    buf.pop_front();
            }
            buf.push_back(img.clone());
            res = sum;
            break;

        case SimpleMode:
            res = img;
        break;
        case InfAverageMode:
            res = cv::Mat::eye(img.rows, img.cols, img.type());
            break;
        case DashMode:
            res = cv::Mat::eye(img.rows, img.cols, img.type()).inv();
        break;
        }
        wnd.imshow(res);
    }

}
