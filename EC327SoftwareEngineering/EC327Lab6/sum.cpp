#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

using namespace std;

char* buffer;

int sumUp(int num){
    int i, sum = 0;
    for(i = 1; i <= num; i++){
        sum += i;
    }
    if(sum == 5050) { //the sum of 1..100 is 5050
        cout << "Your total is 5050!" << endl;
        strcpy(buffer, "5050");
    }
    cout << "sum=" << sum;
    return sum;
}

void printSum(){
    char line[100];
    cout << "Please enter a number:" << endl;
    cin >> line;
    int sum = sumUp(atoi(line));
    strcpy(buffer, to_string((long long)sum).c_str());
    cout << buffer << endl;
}

int main(int argv, char** argc){
    printSum();
    return 0;
}
