#include <iostream>

using namespace std;
int fact(int big, int small)
{
    int out = small + 1;
    for (int i = small + 2; i <= big; i++)
    {
        out = out * i;
    }
    return out;
}

int fact(int x){
    int out = 1;
    for (int i = 1; i <= x; i++){
        out = out * i;
    }
    return out;
}

int main()
{
    int big;
    int small;
    cin >> big;
    cin >> small;
    cout << fact(big, small)/fact(big - small);
}
