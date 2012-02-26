#include "videoprocessor.h"
#include <QDebug>
#include <QMutexLocker>
#include <QStringList>
#include <calc.h>

VideoProcessor::VideoProcessor(QObject *parent) :
    QThread(parent),
    average_cnt(10),
    gmode(SimpleMode),
    flipV(0),
    flipH(0),
    threshold(0.2f),
    clear_flag(false),
    running(true),
    wnd(this, std::string("VideoWindow")),
    m_conditionChanged(false)
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
        int aver_cnt = average_cnt;

        Mode mode = gmode;
        if (sum.empty() || mode != lmode || clear_flag)
        {
            sum = img.clone();
            buf.clear();
            clear_flag = false;
        }

        QString lCond;
        if (m_conditionChanged)
        {
            QMutexLocker locker(&m_conditionMutex);
            lCond = m_condition;
            m_conditionChanged = false;
        }
        switch (mode)
        {
        case AverageMode:
            buf.push_back(img.clone());
            if ((int)buf.size() <= aver_cnt)
            {
                sum = cv::max(sum, img);
            }else
            {
                for (int i = 0; i < 1+((int)buf.size() > aver_cnt); ++i)
                {
                    buf.pop_front();
                    sum = buf.front();
                    std::for_each(buf.begin()+1, buf.end(), [&sum](cv::Mat& m)
                    {
                                  sum = cv::max(sum, m);
                });
            }
        }
        res = sum;
        break;

        case SimpleMode:
            res = img;
            break;
        case InfAverageMode:
            sum = cv::max(sum, img);
            res = sum;
            break;
    }
    lmode = mode;
    int lflipV = flipV;
    int lflipH = flipH;
    if (running)
        if (lflipV || lflipH)
        {
            cv::Mat flippedRes;
            cv::flip(res, flippedRes, 2-(lflipH | (lflipV << 1)));
            wnd.imshow(flippedRes);
        }else
            wnd.imshow(res);

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

void VideoProcessor::setCondition(QString &condition)
{
    QMutexLocker locker(&m_conditionMutex);
    m_condition = condition;
    m_conditionChanged = true;
}
