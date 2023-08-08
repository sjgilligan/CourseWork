#include <iostream>
#include <string.h>
using namespace std;

int main(){
   int input;
   int exit = 0;
   do{
     cout << "How tall of a ladder do you need?" << endl;
	cin >> input;
	if(input == 0)
	  cout << "The height can't be zero." << endl;
	else if(input > 0)
	     for(int i = 0; i < input; i++){
	     	     if(i % 2 == 0)
		       cout << "# #" << endl;
		     else
		       cout << "######" << endl;
		}
	else{
	  cout << "That is invalid" << endl;
	  return 0;
	}
    }while(1);
    return 0;
}
