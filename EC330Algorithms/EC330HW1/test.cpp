//
// Created by Sebbie on 2/8/2023.
//
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int main(){
    vector<int> birthdays = {31, 27, 1, 21, 27,23,7,16,5,8,19,12,8,31,15,17,20,27,23,14,3,1,26};
    vector<int> ga;
    vector<int> gb;
    sort(birthdays.begin(),birthdays.end(),greater<int>());
    ga.insert(ga.begin(),birthdays[0]);
    for(int i = 1; i < size(birthdays); i++){
        if(accumulate(gb.begin(),gb.end(),0) > accumulate(ga.begin(),ga.end(),0)){
            ga.insert(ga.begin(),birthdays[i]);
        }
        else if(accumulate(gb.begin(),gb.end(),0) < accumulate(ga.begin(),ga.end(),0)){
            gb.insert(gb.begin(),birthdays[i]);
        }
        else{
            ga.insert(ga.end(),birthdays[i]);
        }

        for (int j = 0; j < size(birthdays); j++) {
            cout << birthdays[j] << " ";
        }
        cout << endl;
        for (int j = 0; j < size(ga); j++) {
            cout << ga[j] << " ";
        }
        cout << endl;
        for (int j = 0; j < size(gb); j++) {
            cout << gb[j] << " ";
        }
        cout << endl;
        cout << abs(accumulate(ga.begin(),ga.end(),0) - accumulate(gb.begin(),gb.end(),0)) << endl;
    }

    for (int i = 0; i < size(birthdays); i++) {
        cout << birthdays[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < size(ga); i++) {
        cout << ga[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < size(gb); i++) {
        cout << gb[i] << " ";
    }
    cout << endl;
    cout << abs(accumulate(ga.begin(),ga.end(),0) - accumulate(gb.begin(),gb.end(),0));
    return 0;
}