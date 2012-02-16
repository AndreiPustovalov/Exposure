#include "videoprocessor.h"
#include <opencv/highgui.h>
#include "CvWindow.hpp"
#include <QDebug>
#include <QFile>

VideoProcessor::VideoProcessor(QObject *parent) :
    QThread(parent),
    average_cnt(10),
    gmode(SimpleMode),
    flipV(0),
    flipH(0),
    threshold(0.2f)
{
}

/*class AddFunctor
{
    cv::Mat *img, *sum;
    float cur_size;
public:
    AddFunctor(cv::Mat* sum, cv::Mat* img, float bufSize) : img(img), sum(sum), cur_size(bufSize) {}
    void operator()(int i)
    {
        sum->row(i) += img->row(i)/(cur_size+1);
        sum->row(i) *= (cur_size+1)/(2.0+cur_size);
    }
};

class AddSubFunctor
{
    cv::Mat *img, *sum, *frnt;
    float cur_size;
public:
    AddSubFunctor(cv::Mat* sum, cv::Mat* img, cv::Mat* frnt, float bufSize) : img(img), sum(sum), frnt(frnt), cur_size(bufSize) {}
    void operator()(int i)
    {
        sum->row(i) += (img->row(i)-frnt->row(i))/cur_size;
    }
};*/

bool cmpvec(const cv::Vec3f& a, const cv::Vec3f b)
{
    return cv::norm(a) > cv::norm(b);
}

void VideoProcessor::run()
{
    cv::VideoCapture cap(0);
/*    class MyCap
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
            img = cv::imread(QString("..\\Exposure\\imgs\\%1.jpg").arg((n==495)?n=1:n++).toStdString(), -1);
            return *this;
        }
    } cap;*/

    if (!cap.isOpened())
    {
        emit error(tr("Can't open video capture device"));
        return;
    }
    CvWindow wnd("Video Window");
    QList<cv::Mat> buf;
    cv::Mat sum;
    while (true)
    {
        cv::Mat img, res;
        cap >> img;
        img.convertTo(img, CV_32FC3, 1.0/255.0);
        int aver_cnt = average_cnt;

        Mode mode = gmode;
        if (sum.empty() || mode != lmode)
        {
            sum = cv::Mat::zeros(img.rows, img.cols, img.type());
        }
        switch (mode)
        {
        case AverageMode:
            if ((int)buf.size() < aver_cnt)
            {
                sum += img/(static_cast<float>(buf.size())+1);
                sum *= (static_cast<float>(buf.size())+1)/(2.0+static_cast<float>(buf.size()));
            }else
            {
                for (int i = 0; i < 1+((int)buf.size() > aver_cnt); ++i)
                {
                    cv::Mat frnt = buf.front();
                    sum += (img-frnt)/static_cast<float>(buf.size());
                    buf.pop_front();
                }
            }
            buf.push_back(img);
            res = sum;
            break;

        case SimpleMode:
            res = img;
            break;
        case InfAverageMode:
        {
/*            float lthr = threshold;
            for (int i = 0; i < img.rows; ++i)
            {
                cv::Vec3f* sumRow = sum.ptr<cv::Vec3f>(i);
                const cv::Vec3f* imgRow = img.ptr<cv::Vec3f>(i);
                for (int j = 0; j < img.cols; ++j)
                {
                    if (norm(imgRow[j])>lthr && norm(sumRow[j]) < 1.0f)
                    {
                        sumRow[j] += imgRow[j];
                        if (norm(sumRow[j])>1.0f)
                            sumRow[j] *= 1/std::max(sumRow[j][0], std::max(sumRow[j][1], sumRow[j][2]));
                    }
                }
            }*/
            for (int i = 0; i < img.rows; ++i)
            {
                cv::Vec3f* sumRow = sum.ptr<cv::Vec3f>(i);
                const cv::Vec3f* imgRow = img.ptr<cv::Vec3f>(i);
                for (int j = 0; j < img.cols; ++j)
                {
                    if (norm(imgRow[j])>norm(sumRow[j]))
                        sumRow[j] = imgRow[j];
                }
            }
            res = sum;
        }
        break;

        case DashMode:
            res = img;
            break;
        }
        lmode = mode;
        int lflipV = flipV;
        int lflipH = flipH;
        if (lflipV || lflipH)
            cv::flip(res, res, 2-(lflipH | (lflipV << 1)));
        wnd.imshow(res);
    }

}
