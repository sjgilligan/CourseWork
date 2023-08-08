#include "sortStudents.h"
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

//https://cplusplus.com/forum/beginner/126917/

//helper function: takes vector and int, returns number of times that int appears in the vector.
int appears_in_vector(vector<int> vec, int input){
    int result = 0;
    for (int i = 0; i < vec.size(); i++){
        if (input == vec[i]) {
            result++;
        }
    }
    return result;
}


vector<int> sortStudents(vector<int> days, vector<int> ids) {
    cout << "days ";
    for(int i = 0; i < size(days); i++){
        cout << days[i] << ", ";
    }
    cout << endl << "ids: ";
    for(int i = 0; i < size(ids); i++){
        cout << ids[i] << ", ";
    }

    vector<int> output;

    int i = 0;

    //checks for elements of days that also appear in ids and puts them in the order of ids
    while(i < size(ids)) {
        if(appears_in_vector(days, ids[i]) == 0){
            i++;
        }else{
            for(int k = 0; k < appears_in_vector(days, ids[i]); k++) {
                //cout << "matched ";
                output.push_back(ids[i]);
            }
            i++;
        }
    }

    vector<int> removed_from_days;

    //iterates through days and adds remaining elements as they appear

    for(int i = 0; i < size(days);i++){
        if(appears_in_vector(days, days[i]) != 0 && appears_in_vector(ids, days[i]) == 0 && appears_in_vector(removed_from_days, days[i]) == 0){
            for(int k = 0; k < appears_in_vector(days, days[i]); k++) {
                //cout << "adding ";
                output.push_back(days[i]);
            }
            removed_from_days.push_back(days[i]);
        }
    }

    cout << endl << "output: ";
    for(int i = 0; i < size(output); i++){
        cout << output[i] << ", ";
    }

    return output;
}

int main(){
    vector<int> days = {3,4,2,16,27,27,27,2,1};
    vector<int> ids = {4,16,15,21};
    sortStudents(days,ids);
    return 0;
}
