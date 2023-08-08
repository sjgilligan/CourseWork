//
// Created by Sebbie on 2/8/2023.
//
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int foo(int x) {
    if (x==1) return 1;
    else return 2*foo(int(x/2));
}
int main(){
    int sum = 1;
    for (int i = 2; i < 5; i++) {
        sum = sum + (1 << i);
    }
    cout << sum;
}
