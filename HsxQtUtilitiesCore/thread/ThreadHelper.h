#ifndef THREADHELPER_H
#define THREADHELPER_H


#include <functional>
#include <HsxQtUtilitiesCore_global.h>

namespace hsx
{
    class HSXQTUTILITIESCORE_EXPORT ThreadHelper
    {
    public:
        static ThreadHelper *instance();
        void delay(int msce);
        void waitOn(const std::function<bool()> &pFunc, int msecsTimecheck = 10, int msecsTimeout = 5000);
    private:
        ThreadHelper();
        static ThreadHelper *_instance;
    };
}
#endif // THREADHELPER_H
