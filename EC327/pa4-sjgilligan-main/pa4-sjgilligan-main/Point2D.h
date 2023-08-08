#include <iostream>
#include "Vector2D.h"

using namespace std;

#ifndef PA3_POINT2D_H
#define PA3_POINT2D_H

class Point2D {
public:
    double x;
    double y;

    Point2D();

    Point2D(double, double);

    friend ostream &operator<<(ostream &os, const Point2D &p1) {
        os << "(" << p1.x << "," << p1.y << ")";
        return os;
    };

    Vector2D operator-(const Point2D p1) {
        Vector2D vout;
        vout.x = this->x - p1.x;
        vout.y = this->y - p1.y;
        return vout;
    };

    Point2D operator+(const Vector2D v1) {
        Point2D pout;
        pout.x = this->x + v1.x;
        pout.y = this->y + v1.y;
        return pout;
    }

    bool operator==(const Point2D p1) {
        if (this->x == p1.x && this->y == p1.y) {
            return true;
        } else {
            return false;
        }
    }

};

double GetDistanceBetween(Point2D, Point2D);


#endif //PA3_POINT2D_H
