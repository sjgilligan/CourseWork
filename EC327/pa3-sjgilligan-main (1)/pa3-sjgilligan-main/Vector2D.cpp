#include <iostream>
#include <math.h>
#include "Vector2D.h"
#include "Point2D.h"


using namespace std;

Vector2D::Vector2D() {
    x = 0;
    y = 0;
}

Vector2D::Vector2D(double in_x, double in_y) {
    x = in_x;
    y = in_y;
}

