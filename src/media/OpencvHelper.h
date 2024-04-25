#ifndef OPENCVHELPER_H
#define OPENCVHELPER_H

#include "HsxQtUtilities_global.h"
#include <QImage>

namespace hsx
{
class HSXQTUTILITIES_EXPORT OpencvHelper
{
public:
    struct VideoInfo
    {
        QImage thumbnail;
        double fps;
        double frameCount;
    };
    OpencvHelper();
    bool getVideoInfo(const QString &path, float posScale, const QString &ext, VideoInfo &ret);
private:
};
}

#endif // OPENCVHELPER_H
