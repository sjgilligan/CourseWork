#include "node.h"
#include <iostream>

using namespace std;

int weight(node*);

// your includes here

//https://www.enjoyalgorithms.com/blog/find-height-of-a-binary-tree
//https://www.geeksforgeeks.org/count-number-of-nodes-in-a-complete-binary-tree/

bool isWeightBalanced(node* root, int k) {
    if (root == NULL) {
        return true;
    }
    int left_weight = weight(root->left);
    int right_weight = weight(root->right);

    if (abs(left_weight - right_weight) > k) {
        return false;
    }else {
        return isWeightBalanced(root->left, k) && isWeightBalanced(root->right, k);
    }
}

int weight(node* root) {
    if (root == nullptr) {
        return 0;
    }
    int left = weight(root->left);
    int right =  weight(root->right);

    return left + right + 1;
}



/*int main(){
    node* my_node5 = new node(5);
    node* my_node4 = new node(4) ;
    node* my_node3 = new node(3);
    node* my_node6 = new node(6);
    node* my_node7 = new node(7);
    node* my_node52 = new node(5);
    node* my_node62 = new node(6);

    my_node5->left = my_node4;
    my_node5->left->left = my_node3;
    my_node5->left->right = my_node62;
    my_node5->right = my_node6;
    my_node5->right->right = my_node7;
    my_node5->right->left = my_node52;



    cout << isWeightBalanced(my_node5, 0);
}*/
// your helper methods here