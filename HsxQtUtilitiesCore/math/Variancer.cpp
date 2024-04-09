#include "Variancer.h"

#include <limits>

hsx::Variancer::Variancer(const std::vector<double> data)
    : _data(data)
{
    refersh();
}

void hsx::Variancer::refersh()
{
    min = std::numeric_limits<double>::max();
    max = std::numeric_limits<double>::min();
    double sum = 0;
    foreach(auto d, _data)
    {
        min = qMin(min, d);
        max = qMax(max, d);
        sum += d;
    }
    range = max - min;
    count = _data.size();
    average = count ? sum / count : 0;
}

bool hsx::Variancer::deviation(double data, double &result)
{
    if(data < min || data > max)
    {
        return false;
    }
    result = data - min;
    return true;
}

bool hsx::Variancer::uniforDeviation(double data, double &result)
{
    double ret = 0;
    if(deviation(data, ret))
    {
        result = ret /  range;
        return true;
    }
    return false;
}


