#include "videoprocessor.h"
#include <opencv/highgui.h>
#include "CvWindow.hpp"
#include <QDebug>
#include <qtconcurrentmap.h>
#include <boost/iterator/counting_iterator.hpp>

#ifdef QT_NO_CONCURRENT
#error("No concurrent")
#endif

VideoProcessor::VideoProcessor(QObject *parent) :
    QThread(parent),
    average_cnt(10),
    mode(SimpleMode),
    flipV(0),
    flipH(0)
{
}

class SumFunctor
{
    cv::Mat &img, &sum;
    float cur_size;
public:
    SumFunctor(cv::Mat& img, cv::Mat& sum, float bufSize) : img(img), sum(sum), cur_size(bufSize) {}
    void operator()(int i)
    {
        sum.row(i) += img.row(i)/(cur_size+1);
        sum.row(i) *= (cur_size+1)/(2.0+cur_size);
    }
};

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
            img = cv::imread(QString("C:\\Documents and Settings\\PustovalovAP\\My Documents\\Qt\\Exposure\\imgs\\%1.jpg").arg((n==495)?n=1:n++).toStdString(), -1);
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
        img.convertTo(img, CV_32FC3, 1.0/255.0);
        int aver_cnt = average_cnt;

        switch (mode)
        {
        case AverageMode:
            if (sum.empty())
                sum = cv::Mat::zeros(img.rows, img.cols, img.type());
            if ((int)buf.size() < aver_cnt)
            {
                QtConcurrent::blockingMap(boost::counting_iterator<int>(0), boost::counting_iterator<int>(img.rows), SumFunctor(img, sum, buf.size()));
 /*               sum += img/static_cast<float>(buf.size()+1);
                sum *= static_cast<float>(buf.size()+1)/(2.0+static_cast<float>(buf.size()));*/
            }else
            {
                for (int i = 0; i < 1+((int)buf.size() > aver_cnt); ++i)
                {
                    cv::Mat frnt = buf.front();
                    sum += (img-frnt)/static_cast<float>(aver_cnt);
                    buf.pop_front();
                }
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
        int lflipV = flipV;
        int lflipH = flipH;
        if (lflipV || lflipH)
            cv::flip(res, res, 2-(lflipH | (lflipV << 1)));
        wnd.imshow(res);
    }

}
