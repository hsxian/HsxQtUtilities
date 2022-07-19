#ifndef UNITCONVERSION_H
#define UNITCONVERSION_H

#include <qtutilities_global.h>

namespace hsx
{
    class QTUTILITIESSHARED_EXPORT UnitConversion
    {
    public:
        UnitConversion();
        double knot2MeterPerSecond(double knot);
        double meterPerSecond2Knot(double meter);
        double knot2KilometerPerHour(double knot);
        double kilometerPerHour2Knot(double kilometer);
        double meterPerSecond2KilometerPerHour(double meter);
        double kilometerPerHour2MeterPerSecond(double kilometer);
    };
}
#endif // UNITCONVERSION_H
