//
//  OnePlayerMode.cpp
//  
//
//  Created by Nick Ramondo on 12/3/22.
//

#include "Board.h"

using namespace std;

Board::Board(){
    game_over = 0;
    for (int r = 0; r < 6; r++){
        for (int c = 0; c < 7; c++){
            this->boardptr[r][c] = 0;
        }
    }
}

int Board::winCheck(){
    for (int r = 0; r < 6; r++){
        for (int c = 0; c < 5; c++){
            if (this->boardptr[r][c] == 1 && this->boardptr[r][c+1] == 1 && this->boardptr[r][c+2] == 1 && this->boardptr[r][c+3] == 1){
                this->game_over = 1;
            }
            if (this->boardptr[r][c] == 2 && this->boardptr[r][c+1] == 2 && this->boardptr[r][c+2] == 2 && this->boardptr[r][c+3] == 2){
                this->game_over = 2;
            }
        }
    }
    
    for (int r = 0; r < 6; r++){
        for (int c = 0; c < 5; c++){
            if (this->boardptr[r][c] == 1 && this->boardptr[r+1][c] == 1 && this->boardptr[r+2][c] == 1 && this->boardptr[r+3][c] == 1){
                this->game_over = 1;
            }
            if (this->boardptr[r][c] == 2 && this->boardptr[r+1][c] == 2 && this->boardptr[r+2][c] == 2 && this->boardptr[r+3][c] == 2){
                this->game_over = 2;
            }
        }
    }
    
    for (int r=3; r < 6; r++){
        for (int c=0; c < 5; c++){
            if (this->boardptr[r][c] == 1 && this->boardptr[r-1][c+1] == 1 && this->boardptr[r-2][c+2] == 1 && this->boardptr[r-3][c+3] == 1){
                this->game_over = 1;
            }
            if (this->boardptr[r][c] == 2 && this->boardptr[r-1][c+1] == 2 && this->boardptr[r-2][c+2] == 2 && this->boardptr[r-3][c+3] == 2){
                this->game_over = 2;
            }
        }
    }
    
    for (int r=0; r < 6; r++){
        for (int c=0; c < 5; c++){
            if (this->boardptr[r][c] == 1 && this->boardptr[r+1][c+1] == 1 && this->boardptr[r+2][c+2] == 1 && this->boardptr[r+3][c+3] == 1){
                this->game_over = 1;
            }
            if (this->boardptr[r][c] == 2 && this->boardptr[r+1][c+1] == 2 && this->boardptr[r+2][c+2] == 2 && this->boardptr[r+3][c+3] == 2){
                this->game_over = 2;
            }
        }
    }
    
    if (this->moveCount == 42){
        this->game_over = 3;
    }
    return game_over;
}

Point2d Board::getMove(int move){
    for(int r = 0; r < 6; r++){
        if(this->boardptr[move][r] == 0) {
            return Point2d(move + 1, r-6);
        }
    }
    return Point2d(10,10);
}

void Board::placePiece(int move,char symbol){
    if (symbol == 'X'){
        for(int r = 0; r < 6 ; r++){
            if(this->boardptr[move][r] == 0){
                this->boardptr[move][r] = 1;
                return;
            }
        }
    }
    else {
        for(int r = 0; r < 6 ; r++){
            if(this->boardptr[move][r] == 0){
                this->boardptr[move][r] = 2;
                return;
            }
        }
    }
    return;
}






