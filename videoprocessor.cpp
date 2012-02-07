#include "videoprocessor.h"
#include <opencv/highgui.h>
#include <algorithm>

VideoProcessor::VideoProcessor(QObject *parent) :
    QThread(parent)
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
    int aver_cnt = 10;
    cv::Mat sum;
    while (true)
    {
            cv::Mat img, frnt;
            cap >> img;
            if (sum.empty())
                    sum = cv::Mat::zeros(img.rows, img.cols, img.type());
            if (buf.size() != aver_cnt)
            {
                    for (int y = 0; y < img.rows; ++y)
                    {
                            uchar* sumRow = sum.ptr(y);
                            const uchar* imgRow = img.ptr(y);
                            for (int x = 0; x < img.cols*3; ++x)
                            {
                                    sumRow[x] = (buf.size()*sumRow[x] + imgRow[x])/(buf.size()+1);
                            }
                    }
            }else
            {
                    frnt = buf.front();
                    for (int y = 0; y < img.rows; ++y)
                    {
                            uchar* sumRow = sum.ptr(y);
                            const uchar* imgRow = img.ptr(y);
                            const uchar* frontRow = frnt.ptr(y);
                            for (int x = 0; x < img.cols*3; ++x)
                            {
                                    sumRow[x] = std::min(255,std::max(0,(aver_cnt*sumRow[x] + imgRow[x] - frontRow[x])/(aver_cnt)));
                            }
                    }
                    buf.pop_front();
            }
            buf.push_back(img.clone());
            cv::imshow("window", sum);
    }
}

/*
int max(int a, int b)
{
        return (a>b)?a:b;
}
int min(int a, int b)
{
        return (a<b)?a:b;
}
int main()
{
        cv::VideoCapture cap(0);
        if (!cap.isOpened())
        {
                std::cout << "Can't open video capture device." << std::endl;
                return -1;
        }
        cv::namedWindow("window");
        std::list<cv::Mat> buf;
        int aver_cnt = 10;
        cv::Mat sum;
        while (1)
        {
                cv::Mat img, frnt;
                cap >> img;
                if (sum.empty())
                        sum = cv::Mat::zeros(img.rows, img.cols, img.type());
                if (buf.size() != aver_cnt)
                {
                        for (int y = 0; y < img.rows; ++y)
                        {
                                uchar* sumRow = sum.ptr(y);
                                const uchar* imgRow = img.ptr(y);
                                for (int x = 0; x < img.cols*3; ++x)
                                {
                                        sumRow[x] = (buf.size()*sumRow[x] + imgRow[x])/(buf.size()+1);
                                }
                        }
                }else
                {
                        frnt = buf.front();
                        for (int y = 0; y < img.rows; ++y)
                        {
                                uchar* sumRow = sum.ptr(y);
                                const uchar* imgRow = img.ptr(y);
                                const uchar* frontRow = frnt.ptr(y);
                                for (int x = 0; x < img.cols*3; ++x)
                                {
                                        sumRow[x] = min(255,max(0,(aver_cnt*sumRow[x] + imgRow[x] - frontRow[x])/(aver_cnt)));
                                }
                        }
                        buf.pop_front();
                }
                buf.push_back(img.clone());
                cv::imshow("window", sum);
                int key = cv::waitKey(1);
                if (27==key)
                        return 0;
                if (13==key)
                        sum = cv::Mat::zeros(sum.rows, sum.cols, sum.type());
        }
        return 0;
}
*/
