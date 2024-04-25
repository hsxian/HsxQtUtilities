#include "OpencvHelperTest.h"

void OpencvHelperTest::test_get_video_info()
{
    OpencvHelper::VideoInfo vi;
    m_OpencvHelper.getVideoInfo("../data/movie.mp4", 0.5, "png", vi);
    QVERIFY(vi.frameCount == 374);
}
