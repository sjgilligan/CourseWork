//
// Created by Sebbie on 11/22/2022.
//
#include "Point2D.h"
#include "GameObject.h"

#ifndef PA3_VIEW_H
#define PA3_VIEW_H

const int view_maxsize = 20;


class View {
private:
    int size;
    double scale;
    Point2D origin;
    char grid[view_maxsize][view_maxsize][2];

    bool GetSubscripts(int &out_x, int &out_y, Point2D location);

public:
    View();

    void Clear();

    void Plot(GameObject *ptr);

    void Draw();
};


#endif //PA3_VIEW_H
