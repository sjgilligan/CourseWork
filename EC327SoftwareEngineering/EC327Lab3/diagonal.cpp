#include <iostream>
#include <string.h>
using namespace std;

int main() {
    int input;
    do {
        cout << "Please enter a non negative integer between 0 and 10: ";
        cin >> input;
        int length = input;
        if((input <= 0) || (input >= 10)){
            cout <<"Invalid input for N!" << endl;
            cout << "<Terminate>" << endl;
            return 0;
        }
        else{
            cout << "N: " << input << endl;
            string temp = " ";
            temp.insert(0, (input-1), ' ');
            
            for(int i = length; i!=0; i--){
                temp = temp.replace(i-1, 1, "");
                cout << temp;
                cout << input << endl;
                input--;
            }
        }
    }while(1);
    return 0;
}