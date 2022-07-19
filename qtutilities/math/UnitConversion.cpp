#include "UnitConversion.h"

hsx::UnitConversion::UnitConversion()
{

}
double hsx::UnitConversion::knot2MeterPerSecond(double knot)
{
    return knot * 0.51444444444444;
}
double hsx::UnitConversion::meterPerSecond2Knot(double meter)
{
    return meter / 0.51444444444444;
}
double hsx::UnitConversion::knot2KilometerPerHour(double knot)
{
    return knot * 1.852;
}
double hsx::UnitConversion::kilometerPerHour2Knot(double kilometer)
{
    return kilometer / 1.852;
}

double hsx::UnitConversion::meterPerSecond2KilometerPerHour(double meter)
{
    return meter * 3.6;
}
double hsx::UnitConversion::kilometerPerHour2MeterPerSecond(double kilometer)
{
    return kilometer / 3.6;
}
