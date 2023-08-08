/**
 * This function calculates the election results
 
It will return an int. It should be 10 times the i1 value + i2 value + the integer cast value of the first character of the string. For example:
i1 = 3, i2 = 1 - the return value should be 31
 
You can get the first character of the string using array syntax.
 
 * @param i1 - i1 int value
 * @param i2 - i2 int value
 * @param state - string for the state
 */
 
 
#include <iostream>

using namespace std;
 
//Add electionResult function here

int electionResult(int i1, int i2, string state)
{
    int out;
    out = (i1 * 10) + i2 + state[0];
    return out; 
}

