//
// Created by Sebbie on 12/6/2022.
//

#ifndef EC327FINAL_VISUAL_H
#define EC327FINAL_VISUAL_H

#include "Board.h"
#include "Point2d.h"

const int rows = 7;
const int columns = 6;

class Visual {
private:
    char grid[rows][columns];
public:
    Visual();
    void Startup();
    void Draw();
    void Plot(Point2d p, char piece);
};


#endif //EC327FINAL_VISUAL_H
