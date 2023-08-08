#include <string>
#include <iostream>
#include "trie.h"

using namespace std;

bool hasNoChildren(TrieNode);

void Trie::insert(string key, int val) {
    struct TrieNode *current_node;
    current_node = this->root;
    this->map_size++;

    for(int i =0; i < (key.size()); i++){
        if(current_node->children[(key[i]-'a')] == NULL){
            //cout << key[i] << endl;
            struct TrieNode *new_node = new TrieNode();
            current_node->children[(key[i]-'a')] = new_node;
            current_node = new_node;
            this->tree_size++;
        }else{
            current_node = current_node->children[(key[i]-'a')];
        }
    }
    current_node->val = val;
}

int Trie::search(string key) {
    struct TrieNode *current_node;
    current_node = this->root;

    for(int i =0; i < key.size(); i++){
        if(current_node->children[(key[i]-'a')] == NULL)
            return 0;
        else
            current_node = current_node->children[key[i]-'a'];
    }
    return current_node->val;
}

void Trie::remove(string key) {
    struct TrieNode *current_node;
    //struct TrieNode *next_node;
    current_node = this->root;
    if(search(key) == 0){
        return;
    }
    for(int i =0; i < (key.size()); i++){
        current_node = current_node->children[key[i]-'a'];
    }
    current_node->val = 0;
    if(hasNoChildren(*current_node)){

        string key2 = key;
        key2.pop_back();
        current_node = root;
        for(int i =0; i < (key2.size()); i++){
            current_node = current_node->children[key2[i]-'a'];

        }

        current_node->children[key[key.size()-1]-'a'] = NULL;
        this->tree_size--;

    }

    current_node->val = 0;
    this->map_size--;

    /*current_node = this->root;
    for(int i =0; i < key.size(); i++){
        if(hasOneChild(*current_node)){
            current_node->children[key[i]-'a'] = NULL;
        }
        //current_node = current_node->children[key[i]-'a'];
    }*/
}

bool hasNoChildren(TrieNode current_node){
    int count = 0;
    for(int i =0; i < 26; i++){
        if(current_node.children[i]){
            count++;
        }
    }
    if(count == 0){
        return true;
    }else{
        return false;
    }
}