#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <QThread>

class VideoProcessor : public QThread
{
    Q_OBJECT
public:
    enum Mode{
        SimpleMode,
        AverageMode,
        InfAverageMode,
        DashMode
    };
private:
    volatile int average_cnt;
    volatile int visible_cnt;
    Mode mode;
    volatile bool flipV, flipH;

public:
    explicit VideoProcessor(QObject *parent = 0);
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
        mode = m;
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
};

#endif // VIDEOPROCESSOR_H
