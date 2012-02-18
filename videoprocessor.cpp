#include "videoprocessor.h"
#include <QDebug>

VideoProcessor::VideoProcessor(QObject *parent) :
    QThread(parent),
    average_cnt(10),
    gmode(SimpleMode),
    flipV(0),
    flipH(0),
    threshold(0.2f),
    clear_flag(false),
    running(false),
    wnd("VideoWindow")
{
}

VideoProcessor::~VideoProcessor()
{
    stop();
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

void VideoProcessor::run()
{
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

    running = true;
    cv::VideoCapture cap(0);
    if (!cap.isOpened())
    {
        emit error(tr("Can't open video capture device"));
        return;
    }
    QList<cv::Mat> buf;
    cv::Mat sum;
    while (running)
    {
        cv::Mat img, res;
        cap >> img;
        img.convertTo(img, CV_32FC3, 1.0/255.0);
        int aver_cnt = average_cnt;

        Mode mode = gmode;
        if (sum.empty() || mode != lmode || clear_flag)
        {
            sum = img.clone();//cv::Mat::zeros(img.rows, img.cols, img.type());
            buf.clear();
            clear_flag = false;
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
        {
            cv::Mat flippedRes;
            cv::flip(res, flippedRes, 2-(lflipH | (lflipV << 1)));
            wnd.imshow(flippedRes);
        }else
            wnd.imshow(res);
        emit needWaitKey();
        msleep(10);
    }
    wnd.destroyWindow();
}

bool VideoProcessor::stop(unsigned long waitTime)
{
    running = false;
    return wait(waitTime);
}
