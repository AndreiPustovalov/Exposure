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
    running(true),
    wnd("VideoWindow")
{
    cap.open(0);
}

VideoProcessor::~VideoProcessor()
{
    qDebug() << "VedioProcessor destructor";
    if (isRunning())
        wait();
}

class comp
{
private:
    int i,j;
public:
    comp(int i, int j) : i(i), j(j) {}
    bool operator()(const cv::Mat& a, const cv::Mat& b)
    {
        return norm(a.at<cv::Vec3b>(i,j)) < norm(b.at<cv::Vec3b>(i,j));
    }
};

bool Vec3bComp(const cv::Vec3b& a, const cv::Vec3b& b)
{
    return norm(a) < norm(b);
}

void VideoProcessor::run()
{
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
        //        img.convertTo(img, CV_32FC3, 1.0/255.0);
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
                sum = cv::max(sum, img);
                buf.push_back(img);
            }else
            {
                buf.push_back(img);
                for (int i = 0; i < 1+((int)buf.size() > aver_cnt); ++i)
                {
                    cv::Mat front = buf.front();
                    buf.pop_front();
                    for (int i = 0; i < img.rows; ++i)
                    {
                        cv::Vec3b* sumRow = sum.ptr<cv::Vec3b>(i);
                        cv::Vec3b* imgRow = img.ptr<cv::Vec3b>(i);
                        cv::Vec3b* frontRow = front.ptr<cv::Vec3b>(i);
                        for (int j = 0; j < img.cols; ++j)
                        {
                            if (sumRow[j] == frontRow[j])
                                sumRow[j] = *std::max_element(buf.begin(), buf.end(), comp(i,j));
                            else
                                sumRow[j] = std::max(imgRow[j], sumRow[j], Vec3bComp);
                        }
                    }
                }
            }
            res = sum;
            break;

        case SimpleMode:
            res = img;
            break;
        case InfAverageMode:
        {
            sum = cv::max(sum, img);
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

        if (running)
        {
            emit needWaitKey();
        }
    }
    qDebug() << "Thread loop end";
    wnd.destroyWindow();
    qDebug() << "Thread end";
}

void VideoProcessor::stop()
{
    running = false;
    qDebug() << "Stop thread";
}
