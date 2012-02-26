#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <QThread>
#include <opencv/highgui.h>
#include "cvwindow.h"
#include <QMutex>

class VideoProcessor : public QThread
{
    Q_OBJECT
public:
    enum Mode{
        SimpleMode,
        AverageMode,
        InfAverageMode
    };
private:
    volatile int average_cnt;
    volatile int visible_cnt;
    volatile Mode gmode, lmode;
    volatile bool flipV, flipH;
    volatile float threshold;
    volatile bool clear_flag, running;
    cv::VideoCapture cap;
    CvWindow wnd;
    QString m_condition;
    QMutex m_conditionMutex;
    volatile bool m_conditionChanged;

public:
    explicit VideoProcessor(QObject *parent = 0);
    ~VideoProcessor();
    bool getFullScreen()
    {
        return wnd.getFullScreen();
    }
    const CvWindow& getWnd() const
    {
        return wnd;
    }
protected:
    void run();

signals:
    void error(QString message);
public slots:
    void setAverageCnt(int v)
    {
        average_cnt = v;
    }

    void setMode(Mode m)
    {
        gmode = m;
    }

    void setVisibleCnt(int n)
    {
        visible_cnt = n;
    }
    void setFlipVertical(bool f)
    {
       flipV = f;
    }

    void setFlipHorizontal(bool f)
    {
        flipH = f;
    }
    void setThreshold(float t)
    {
        threshold = t;
    }
    void clear()
    {
        clear_flag = true;
    }
    void setFullScreen(bool fullScreen)
    {
        wnd.setFullScreen(fullScreen);
    }
    void setCondition(QString& condition);

    void stop();
};

#endif // VIDEOPROCESSOR_H
