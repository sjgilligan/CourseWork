#include <vector>
#include <bits/stdc++.h>

//algorithmic idea from https://www.geeksforgeeks.org/lexicographic-permutations-of-string/
//utilized reverse of the next permutation method.
// Go from last index on the right and check if it is increasing
// if n is greater than n+1 stop
// find the number to the right of it that is closest to it but smaller
//swap and sort the values after the index of the first number in reverse order (largest to smallest)
// if numbers are already going from smallest to the largest, just return.




std::vector<int> priorPerm(std::vector<int> P){


    bool perm = false;
    bool perm2 = true;
    int firstnumindece = 0;
    int secondnum = 0;
    std::vector<int> temp;
//std::cout<<P.size()<<std::endl;
    for(int i = P.size() - 1; i> 0; i--){
        if (P[i-1] > P[(i)]){

            perm = true;

            firstnumindece = i - 1;
            //std::cout<<firstnumindece<<std::endl;
            //std::cout<<P[firstnumindece]<<std::endl;
            temp.insert(temp.begin(), P.begin() +  i, P.end());

            i = -1;

        }

    }
    if(perm){
        std::sort(temp.rbegin(),temp.rend());
//std::cout<<temp.size()<<std::endl;

        for(int i = 0; i < temp.size(); i++){
            if((P[firstnumindece] > temp[i])&&(perm2)){
                secondnum = temp[i];
                temp[i] = P[firstnumindece];
                P[firstnumindece] = secondnum;
                P[firstnumindece + i + 1] = temp[i];
                perm2 = false;
            }else{
                // std::cout<<temp[i]<<std::endl;
                P[firstnumindece + i + 1] = temp[i];
            }
        }
    }
    return P;
}
/*
int main(){
    std::vector<int>in {2429,5904,3861,4102};
    std::vector<int> out = priorPerm(in);

    for(int i = 0; i < out.size(); i++){
        std::cout<<out[i]<<std::endl;
    }

}

*/




