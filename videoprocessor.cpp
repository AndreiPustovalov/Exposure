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
            sum = cv::max(sum, img);
            /*for (int i = 0; i < img.rows; ++i)
            {
                cv::Vec3f* sumRow = sum.ptr<cv::Vec3f>(i);
                const cv::Vec3f* imgRow = img.ptr<cv::Vec3f>(i);
                for (int j = 0; j < img.cols; ++j)
                {
                    if (norm(imgRow[j])>norm(sumRow[j]))
                        sumRow[j] = imgRow[j];
                }
            }*/
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
