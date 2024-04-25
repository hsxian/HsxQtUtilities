#include <QObject>
// add necessary includes here
#include <cache/SlideWindowCache.h>

class SlideWinowCacheTest : public QObject
{
    Q_OBJECT

public:
    SlideWinowCacheTest();
    ~SlideWinowCacheTest();
private:
    hsx::SlideWindowCache<uint, QString> *m_Cache = nullptr;
    hsx::SlideWindowConfig<uint, QString> *m_Config = nullptr;
    hsx::StatusFlags m_Status;
private slots:
    void test_config();
    void test_auto_load();
    void test_value_get();
    void test_uniform_key();

};
