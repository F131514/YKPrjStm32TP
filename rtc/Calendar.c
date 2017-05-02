#include "global.h"
#include "Calendar.h"

Uint8 IsLeapYear(Uint16 year)
{
    return (!(year%4)&&(year%100)||!(year%400));
}
