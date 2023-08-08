#include <vector>
#include "bfilter.h"
#include "iostream"

using namespace std;

// (a)

class SillyHashFun : public HashFun {
    unsigned int operator()(const std::string& key) {
        // Here's my silly hash function that always returns 0.
        // You should make some good hash functions and not use this one.
        // This is just an example of how to inherit the HashFun class.
        return 0;
    }
};

class GoodHashFun_0 : public HashFun {
    unsigned int operator()(const std::string& key) {
        const unsigned int fnv_prime = 0x811C9DC5;
        unsigned int hash = 0;
        unsigned int i = 0;
        unsigned int len = key.length();

        for (i = 0; i < len; i++)
        {
            hash *= fnv_prime;
            hash ^= (key[i]);
        }

        return hash;
    }
};

class GoodHashFun_1 : public HashFun {
    unsigned int operator()(const std::string& key) {
        unsigned int hash = 1315423911;

        for (int i = 0; i < key.size(); i++)
        {
            hash ^= ((hash << 5) + key[i] + (hash >> 2));
        }

        return hash;
    }
};

class GoodHashFun_2 : public HashFun {
    unsigned int operator()(const std::string& key) {
        const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
        const unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
        const unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
        const unsigned int HighBits          =
                (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
        unsigned int hash = 0;
        unsigned int test = 0;
        unsigned int i    = 0;

        for (i = 0; i < key.size(); i++)
        {
            hash = (hash << OneEighth) + key[i];

            if ((test = hash & HighBits) != 0)
            {
                hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
            }
        }

        return hash;
    }
};

class GoodHashFun_3 : public HashFun {
    unsigned int operator()(const std::string& key) {
        unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
        unsigned int hash = 0;
        unsigned int i    = 0;

        for (i = 0; i < key.size(); ++i)
        {
            hash = (hash * seed) + key[i];
        }

        return hash;
    }
};


// ...
// You can make as many hash functions as you want!
// ...

void BloomFilter::add_hash_funs() {
    // You will need to implement this function, it is responsible
    // for adding your custom hash functions to your bloom filter.

    // Here's an example where I add two hash functions.
    // You will need to add at least one hash function.
    //SillyHashFun *h1 = new SillyHashFun();
    GoodHashFun_0 *h2 = new GoodHashFun_0();
    GoodHashFun_1 *h3 = new GoodHashFun_1();
    GoodHashFun_2 *h4 = new GoodHashFun_2();
    GoodHashFun_3 *h5 = new GoodHashFun_3();
    //this->hash_funs.push_back(h1);
    this->hash_funs.push_back(h2);
    this->hash_funs.push_back(h3);
    this->hash_funs.push_back(h4);
    this->hash_funs.push_back(h5);
}

void BloomFilter::insert(const std::string& key) {
    // You will need to implement this function, it is responsible
    // for inserting key into the bloom filter.

    // Here's how you can call your hash functions that you added in add_hash_funs()
    /*unsigned int first_hash_result = this->call_hash(0, key);
    // std::cout << "Hash of '" << key << "' is '" << first_hash_result << "' using the first hash function." << std::endl;
    unsigned int second_hash_result = this->call_hash(1, key);
    // std::cout << "Hash of '" << key << "' is '" << second_hash_result << "' using the second hash function." << std::endl;
    unsigned int third_hash_result = this->call_hash(2, key);
    unsigned int fourth_hash_result = this->call_hash(3, key);


    // You'll need to use the results of the hash function to update this->filter
    // Here's an example
    (this->filter)[first_hash_result % 330] = 1;
    (this->filter)[second_hash_result % 330] = 1;
    (this->filter)[third_hash_result % 330] = 1;
    (this->filter)[fourth_hash_result % 330] = 1;*/
    //cout << this->hash_funs.size();

    unsigned int temp;

    for(int i = 0; i < this->hash_funs.size(); i++){
        temp = this->call_hash(i,key);
        //cout << temp%330 << endl;
        (this->filter)[temp % 330] = 1;
    }

    /*for(int i = 0; i < 330; i++){
        cout << this->filter[i];
    }*/

}

bool BloomFilter::member(const std::string& key) {
    // This function should report true if key is potentially in the bloom filter.
    // It should only return false if you know for sure that key is not in the bloom filter.

    //
    // your code here ...
    //

    /*int a = this->filter[call_hash(0,key)%330];
    cout << a << endl;
    int b = this->filter[call_hash(1,key)%330];
    cout << b << endl;
    int c = this->filter[call_hash(2,key)%330];
    cout << c << endl;
    int d = this->filter[call_hash(3,key)%330];
    cout << d << endl;*/

    for(int i = 0; i < hash_funs.size();i++){
        if(this->filter[call_hash(i,key)%330] == 0){
            return false;
        }
    }
    return true;

    //return false; // you will need to change this.
}
