#include <iostream>
#include <math.h>

using namespace std;

#ifndef PA3_VECTOR2D_H
#define PA3_VECTOR2D_H


class Vector2D {
public:
    double x;
    double y;

    Vector2D();

    Vector2D(double, double);

    friend ostream &operator<<(ostream &os, const Vector2D &v1) {
        os << "<" << v1.x << "," << v1.y << ">";
        return os;
    };

    Vector2D operator*(const double d) {
        Vector2D vout;
        vout.x = this->x * d;
        vout.y = this->y * d;
        return vout;
    }

    Vector2D operator/(const double d) {
        Vector2D vout;
        vout.x = this->x / d;
        vout.y = this->y / d;
        return vout;
    }

    bool operator<=(const Vector2D v1) {
        //double mag1 = sqrt((this->x) * (this->x) + (this->y) * (this->y));
        //double mag2 = sqrt((v1.x) * (v1.x) + (v1.y) * (v1.y));
        if (fabs(this->x) <= fabs(v1.x) && fabs(this->y) <= fabs(v1.y)) {
            return true;
        } else {
            return false;
        }

    }


};


#endif //PA3_VECTOR2D_H
