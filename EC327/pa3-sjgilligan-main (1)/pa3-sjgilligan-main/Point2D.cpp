#include "Point2D.h"
#include <math.h>

using namespace std;


Point2D::Point2D() {
    x = 0;
    y = 0;
}

Point2D::Point2D(double in_x, double in_y) {
    x = in_x;
    y = in_y;
}

/**ostream &operator<<(ostream &os, Point2D &p1) {
    os << "(" << p1.x << "," << p1.y << ")";
    return os;
}*/

/*Point2D operator+(const Point2D &p1, const Vector2D &v1) {
    Point2D pout;
    pout.x = p1.x + v1.x;
    pout.y = p1.y + v1.y;
    return pout;
}*/

double GetDistanceBetween(Point2D p1, Point2D p2) {
    return sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p2.x, 2), 2));
}