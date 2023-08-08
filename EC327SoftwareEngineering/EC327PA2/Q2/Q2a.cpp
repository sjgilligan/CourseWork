#include <iostream>
using namespace std;

int gcd(int m, int n)
{
    if (m <= 0 || n <= 0)
    {
        return 0;
    }
    if (m % n == 0)
    {
        return n;
    }
    else
    {
        return gcd(n, m % n);
    }
}

int main()
{
    int m, n;
    cout << "Enter m: " << endl;
    cin >> m;
    cout << "Enter n: " << endl;
    cin >> n;
    cout << "GCD is " << gcd(m, n);
    return 0;
}