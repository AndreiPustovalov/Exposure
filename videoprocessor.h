#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <QThread>

class VideoProcessor : public QThread
{
    Q_OBJECT
public:
    explicit VideoProcessor(QObject *parent = 0);
protected:
    void run();

signals:

public slots:

};

#endif // VIDEOPROCESSOR_H
