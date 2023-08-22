#include "analyticgeometry.h"
#include <math.h>

double distance(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

double CircleArea(double radius)
{
    return PI * pow(radius, 2);
}
double RectangleArea(double width, double height)
{
    return width * height;
}

int SegFunction(double x, double y, int n)
{
    double f = atan2(-y,-x) + PI;
    int result = floor(n * f / (2 * PI));
    return result;
}






bool isPointInsideRectangle(double x1, double y1, double Rxi, double Ryi, double Rxf, double Ryf)
{
    return ((x1 >= Rxi && x1 <= Rxf) && (y1 >= Ryi && y1 <= Ryf));
}