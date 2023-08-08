#include "balancedGroups.h"
#include "balancedGroups.h"
#include <bits/stdc++.h>
// your includes here

using namespace std;

Assignment balancedGroups(std::vector<int> const &birthdays) {
    vector<int> ga;
    vector<int> gb;
    vector<int> birthdays2 = birthdays; //copy const vector to regular vector
    sort(birthdays2.begin(),birthdays2.end(),greater<int>()); //reverse sort
    ga.insert(ga.begin(),birthdays2[0]); //first element to ga
    for(int i = 1; i < size(birthdays2); i++) { //loop through rest of array, whichever's sum is smaller gets the next element, if they are equal it goes to ga
        if (accumulate(gb.begin(), gb.end(), 0) > accumulate(ga.begin(), ga.end(), 0)) {
            ga.insert(ga.begin(), birthdays2[i]);
        } else if (accumulate(gb.begin(), gb.end(), 0) < accumulate(ga.begin(), ga.end(), 0)) {
            gb.insert(gb.begin(), birthdays2[i]);
        } else {
            ga.insert(ga.end(), birthdays2[i]);
        }
    }
    
    return Assignment(abs(accumulate(ga.begin(),ga.end(),0) - accumulate(gb.begin(),gb.end(),0)), ga, gb); // Don't forget to change this
}
/* your helper function here if you have any */