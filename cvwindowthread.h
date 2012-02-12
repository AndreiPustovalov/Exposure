#ifndef CVWINDOWTHREAD_H
#define CVWINDOWTHREAD_H

#include <QThread>

class CvWindowThread : public QThread
{
    Q_OBJECT
public:
    explicit CvWindowThread(QObject *parent = 0);
protected:
    void run();
signals:
    
public slots:
    
};

#endif // CVWINDOWTHREAD_H
