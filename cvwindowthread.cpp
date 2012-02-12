#include "cvwindowthread.h"
#include <opencv/highgui.h>

CvWindowThread::CvWindowThread(QObject *parent) :
    QThread(parent)
{
}

void CvWindowThread::run()
{
    while (true)
        cv::waitKey();
}
