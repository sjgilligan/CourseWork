#include <string>
#include <vector>
#include "iostream"
#include "sorta.h"

using namespace std;

class Element{
public:
    char name;
    int occurrences;
    int ascii_value;
};

int appears_in_vector(vector<char> vec,char input ){
    int result = 0;
    for (int i = 0; i < vec.size(); i++){
        if (input == vec[i]) {
            result++;
        }
    }
    return result;
}

std::string sortByFreq(std::string s) {
    vector<char> input(s.begin(),s.end());
    vector<Element> table(input.size());

    for(int i = 0; i < input.size(); i++){
        table[i].name = input[i];
        //cout << table[i].name << ", ";
        table[i].occurrences = appears_in_vector(input,input[i]);
        //cout << table[i].occurrences << ", ";
        table[i].ascii_value = int(input[i]);
        //cout << table[i].ascii_value << endl;
    }

    int cur_ocr,cur_aval,j;
    Element temp;

    for(int i = 1; i < table.size(); i++){
        cur_ocr = table[i].occurrences;
        cur_aval = table[i].ascii_value;
        j = i - 1;

        while(j > -1){
            if(cur_ocr < table[j].occurrences){
                temp = table[j + 1];
                table[j + 1] = table[j];
                table[j] = temp;
            }else if(cur_ocr == table[j].occurrences){
                if(cur_aval < table[j].ascii_value) {
                    temp = table[j + 1];
                    table[j + 1] = table[j];
                    table[j] = temp;
                }
            }
            j--;
        }
    }

    /*cout << endl;
    for(int i = 0; i < input.size(); i++){
        cout << table[i].name << ", ";
        cout << table[i].occurrences << ", ";
        cout << table[i].ascii_value << endl;
    }*/

    for(int i = 0; i < input.size(); i++){
        input[i] = table[i].name;
    }

    string output(input.begin(),input.end());
    //cout << output;
    return {output};
}

