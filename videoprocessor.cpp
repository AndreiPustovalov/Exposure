#include "videoprocessor.h"
#include <opencv/highgui.h>
#include <algorithm>

VideoProcessor::VideoProcessor(QObject *parent) :
    QThread(parent),
    average_cnt(10)
{
}

void VideoProcessor::run()
{
    cv::VideoCapture cap(0);
    if (!cap.isOpened())
    {
        exit(-1);
    }
    cv::namedWindow("Video Window");
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

        case InfAverageMode:
            res = cv::Mat::eye(img.rows, img.cols, img.type());

        case DashMode:
            res = cv::Mat::eye(img.rows, img.cols, img.type()).inv();
        }
        cv::imshow("window", res);
    }
}
