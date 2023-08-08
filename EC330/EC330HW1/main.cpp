#include <iostream>
#include <bits/stdc++.h>
#include "fun.h"
#include "balancedGroups.h"

using namespace std;

int main() {
    vector<int> birthdays2 = {17,5,3,9,14,30,21};
    vector<int> ga;
    vector<int> gb;
    for (int i = 0; i < size(birthdays2); i++) {
        cout << birthdays2[i] << " ";
    }
    cout << endl;
    gb = birthdays2;
    sort(gb.begin(),gb.end());
    int suma = 0;
    int sumb = accumulate(gb.begin(),gb.end(),0);
    while(suma < sumb){
        ga.push_back(gb.front());
        gb.erase(gb.begin());
        suma = accumulate(ga.begin(),ga.end(),0);
        sumb = accumulate(gb.begin(),gb.end(),0);
    }
    sort(ga.begin(),ga.end());
    while(sumb < suma){
        gb.push_back(ga.front());
        ga.erase(ga.begin());
        suma = accumulate(ga.begin(),ga.end(),0);
        sumb = accumulate(gb.begin(),gb.end(),0);
    }

    //ga.push_back(gb.back());
    //gb.pop_back();






    /*
    int suma = accumulate(ga.begin(),ga.end(),0);
    int sumb = accumulate(gb.begin(),gb.end(),0);
    while(abs(suma - sumb) > 2){
        ga.push_back(gb.back());
        gb.pop_back();
        suma = accumulate(ga.begin(),ga.end(),0);
        sumb = accumulate(gb.begin(),gb.end(),0);
        if(abs(suma - sumb) == 0){
            break;
        }
        ga.push_back(gb.front());
        gb.erase(gb.begin());
        suma = accumulate(ga.begin(),ga.end(),0);
        sumb = accumulate(gb.begin(),gb.end(),0);
        for (int i = 0; i < size(ga); i++) {
            cout << ga[i] << " ";
        }
        cout << "= " << suma << endl;
        for (int i = 0; i < size(gb); i++) {
            cout << gb[i] << " ";
        }
        cout << "= " << sumb << endl;
    }
     */
    /*if(accumulate(ga.begin(),ga.end(),0) > accumulate(gb.begin(),gb.end(),0)){
        gb.push_back(ga.front());
        ga.erase(ga.begin());
    }*/

    /*if(accumulate(ga.begin(),ga.end(),0) < accumulate(gb.begin(),gb.end(),0)){
        ga.push_back(gb.front());
        gb.erase(gb.begin());
        if(accumulate(ga.begin(),ga.end(),0) > accumulate(gb.begin(),gb.end(),0)){
            gb.push_back(ga.front());
            ga.erase(ga.begin());
        }
    }*/
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
