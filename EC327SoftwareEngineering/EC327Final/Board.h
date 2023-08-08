//
//  Board.h
//  
//
//  Created by Nick Ramondo on 12/3/22.
//

#ifndef Board_h
#define Board_h

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <time.h>
#include "Point2d.h"

class Board{
public:
    int boardptr[6][7];
    int game_over;
    int moveCount;
    
    Board();
    int winCheck();
    Point2d getMove(int move);
    void placePiece(int move,char symbol);
};

#endif /* Board_h */
