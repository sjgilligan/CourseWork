#include<iostream>
#include<vector>


//using merge sort from https://slaystudy.com/c-merge-sort-vector/ and modified to sort by size based of mod 330
//inspiration from ChatGPT https://chat.openai.com/chat/de49ad4c-da34-4447-b89b-3c0378029724


// the interval from [s to m] and [m+1 to e] in v are sorted
// the function will merge both of these intervals
// such the interval from [s to e] in v becomes sorted
void MergeSortedIntervals(std::vector<int>& v, int s, int m, int e) {

    // temp is used to temporary store the vector obtained by merging
    // elements from [s to m] and [m+1 to e] in v
    std::vector<int> temp;

    int i, j;
    i = s;
    j = m + 1;

    while (i <= m && j <= e) {

        if (((v[i])%330) <= (v[j]%330)) {
            temp.push_back(v[i]);
            ++i;
        }
        else {
            temp.push_back(v[j]);
            ++j;
        }

    }

    while (i <= m) {
        temp.push_back(v[i]);
        ++i;
    }

    while (j <= e) {
        temp.push_back(v[j]);
        ++j;
    }

    for (int i = s; i <= e; ++i)
        v[i] = temp[i - s];

}

// the MergeSort function
// Sorts the array in the range [s to e] in v using
// merge sort algorithm
void MergeSort(std::vector<int>& v, int s, int e) {
    if (s < e) {
        int m = (s + e) / 2;
        MergeSort(v, s, m);
        MergeSort(v, m + 1, e);
        MergeSortedIntervals(v, s, m, e);
    }
}

std::vector<int> findStudents(std::vector<int> id){
    int size = id.size();
    MergeSort(id, 0, size - 1 );

    int i, j;
    int smallsum;
    int currentsum;
    std::vector<int> result {0,0};
    // initiallize traversal pointers
    i = 0;
    j = size-1;
    smallsum = 1000;

    while (i < j){
        currentsum = (id[i]%330)+(id[j]%330);
        if(currentsum>= 330){
            currentsum = currentsum%330;
            if(currentsum < smallsum){
                smallsum = currentsum;
                result[0] = id[i];
                result[1] = id[j];


            }
            j--;
        }else{
            if (currentsum < smallsum){
                smallsum = currentsum;
                result[0] = id[i];
                result[1] = id[j];

            }
            i++;
        }
        if(smallsum == 0){
            i = j + 1;
        }

    }
    currentsum = (id[0]%330)+(id[1]%330);
    //   std::cout<<currentsum<<std::endl;
    //   std::cout<<smallsum<<std::endl;
    if(currentsum < smallsum){
        smallsum = currentsum;
        result[0] = id[0];
        result[1] = id[1];
    }

    return result;

}



