#include <iostream>
using namespace std;

double series(double n)
{
    double out;
    if (n > 0)
    {
        out = (2 * n) / ((3 * n) + 2) + series(n - 1);
    }
    return out;
}

int main()
{
    double n;
    cout << "Enter n: ";
    cin >> n;
    cout << "Series"
         << "(" << n << ")"
         << " is: " << series(n);
    return 0;
}