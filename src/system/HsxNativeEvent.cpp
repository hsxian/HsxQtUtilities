#include "HsxNativeEvent.h"
#include <QtDebug>

#include <QApplication>
#if WIN64|WIN32
#include <Windows.h>
#include <WinUser.h>
#endif

using namespace hsx;
HsxNativeEvent::HsxNativeEvent()
{
    qApp->installNativeEventFilter(this);
}

HsxNativeEvent::~HsxNativeEvent()
{
    qApp->removeNativeEventFilter(this);
}

#if (QT_VERSION <= QT_VERSION_CHECK(6,0,0))
bool HsxNativeEvent::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
#else
bool HsxNativeEvent::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result)
#endif
{
    //    qDebug() << __FUNCTION__ << eventType;
#if WIN64|WIN32
    if(eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG")
    {
        auto msg = reinterpret_cast<MSG *>(message);
        if(msg->message == WM_LBUTTONDOWN || msg->message == WM_RBUTTONDOWN)
        {
            //            qDebug() << __FUNCTION__ << msg->message << eventType;
            emit onEventSignal(QEvent::MouseButtonPress);
            //            return true;
        }
    }
#endif
    return false;
}
