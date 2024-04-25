#include <QObject>
#include "media/OpencvHelper.h"

using namespace hsx;

class OpencvHelperTest : public QObject
{
    Q_OBJECT

public:
private:
    OpencvHelper m_OpencvHelper;
private slots:
    void test_get_video_info();

};


//QTEST_MAIN(OpencvHelperTest)

//#include "tst_opencvhelpertest.moc"
