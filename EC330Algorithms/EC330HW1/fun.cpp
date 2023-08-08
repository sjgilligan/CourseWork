#include "fun.h"

using namespace std;

unsigned int fun(unsigned int x, unsigned int y) {
    int z;
    if(x == 0 && y == 0)
        return 1;
    /*else if(x == 0 && y != 0)
        return fun(1,y-1);*/
    else if(x != 0 && y == 0)
        return x + 1;
    else
        return 2;
        //return fun(x - 1, fun(x - 1,y-1));
        //return fun(x - 1,y-1);
    return 0;
}

/* your helper function here if you have any */