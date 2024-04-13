#ifndef HSXNATIVEEVENT_H
#define HSXNATIVEEVENT_H

#include <QObject>
#include <QAbstractNativeEventFilter>
#include <QEvent>
#include <HsxQtUtilitiesCore_global.h>
namespace hsx
{
class HSXQTUTILITIESCORE_EXPORT HsxNativeEvent: public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
public: HsxNativeEvent();
    ~HsxNativeEvent();
signals:
    void onEventSignal(QEvent::Type type);
    // QAbstractNativeEventFilter interface
public:

#if (QT_VERSION <= QT_VERSION_CHECK(6,0,0))
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override;
#else
    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result);
#endif
};
}
#endif // HSXNATIVEEVENT_H
