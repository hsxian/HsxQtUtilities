#include "OpencvHelper.h"
#include "opencv2/opencv.hpp"
#include <QDebug>
#include <QImage>

using namespace hsx;

OpencvHelper::OpencvHelper()
{

}

bool OpencvHelper::getVideoInfo(const QString& path, float posScale, const QString& ext, VideoInfo& ret)
{
    auto success = false;
    cv::VideoCapture cap(path.toStdString());
    if(cap.isOpened())
    {
        ret.fps =  cap.get(CV_CAP_PROP_FPS);
        ret.frameCount =  cap.get(CV_CAP_PROP_FRAME_COUNT);
        qDebug() << __FUNCTION__ << __LINE__
                 << cap.set(CV_CAP_PROP_POS_FRAMES, cap.get(CV_CAP_PROP_FRAME_COUNT) * posScale);
        ;
        cv::Mat fm;
        if(cap.read(fm))
        {
            std::vector<uchar> imgBuf;
            if(cv::imencode(QString(".%1").arg(ext).toStdString(), fm, imgBuf)
                    && ret.thumbnail.loadFromData(QByteArray((char*)imgBuf.data(), static_cast<int>(imgBuf.size())), ext.toStdString().c_str()))
            {
                success = true;
            }
        }
        else
        {
            qDebug() << __FUNCTION__ << "cap.read(fm) failed";
        }
    }
    cap.release();
    return success;
}
