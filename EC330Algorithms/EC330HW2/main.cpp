#include <iostream>
#include "findStudents.h"
using namespace std;


int main() {
    vector<int> id = {5338, 2148, 5809, 5698, 8713, 3078, 7611, 3266, 9612};
    vector<int> result = findStudents(id);
    cout << result[0] << ", " << result[1] ;
    //cout << result[2] << ", " << result[3] ;
    return 0;
}
