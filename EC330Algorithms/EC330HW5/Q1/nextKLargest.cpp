#include "BST.h"
#include <vector>
#include "stdlib.h"

using namespace std;
//Collaborated with Kevin Chen
//https://www.geeksforgeeks.org/tree-traversals-inorder-preorder-and-postorder/

void inOrder(Node*,vector<int>&);

/* this is the function that you need to implement */
int BST::nextKLargest(int x, int k) {
    vector<int> nodes;
    inOrder(this->root,nodes);
    if(size(nodes) < k){
        return nodes[size(nodes) - 1];
    }
    for(int i = 0; i < size(nodes);i++){
        if(nodes[i] == x && (i <(size(nodes) - k))){
            return nodes[i + k];
        }else if(nodes[i] > x && (i <(size(nodes) - k))){
            return nodes[i + k - 1];
        }else if(nodes[i] > x){
            return nodes[size(nodes) - 1];
        }
    }
    return 0;
}

void inOrder(Node* root, vector<int>& list){
    if(root == NULL){
        return;
    }

    inOrder(root->left,list);

    list.push_back(root->key);

    inOrder(root->right,list);
}
