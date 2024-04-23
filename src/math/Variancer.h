#ifndef VARIANCER_H
#define VARIANCER_H

#include <vector>
#include <HsxQtUtilities_global.h>

namespace hsx
{
    class HSXQTUTILITIES_EXPORT Variancer
    {
    public:
        Variancer(const std::vector<double> data);
        double average = 0;
        double min = 0;
        double max = 0;
        double range = 0;
        int count = 0;
        void refersh();
        bool deviation(double data, double &result);
        bool uniforDeviation(double data, double &result);
    private:
        std::vector<double> _data;
    };
}
#endif // VARIANCER_H
