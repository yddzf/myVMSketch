

#include "GeneralHash.hpp"


/** a hash function that maps the string value to int value **/
int FNVHash1(char* data) {
    int prime = 16777619;
    int hash = (int) 2166136261L;
    for (int i = 0; i < strlen(data); i++)
        hash = (hash ^ data[i]) * prime;
    hash += hash << 13;
    hash ^= hash >> 7;
    hash += hash << 3;
    hash ^= hash >> 17;
    hash += hash << 5;
//    if (hash < 0){
//        return abs(hash);
//    }
    return hash;
}

//int FNVHash1(long key) {
//    key = (~key) + (key << 18); // key = (key << 18) - key - 1;
//    key = key ^ (key >> 31);
//    key = key * 21; // key = (key + (key << 2)) + (key << 4);
//    key = key ^ (key >> 11);
//    key = key + (key << 6);
//    key = key ^ (key >> 22);
//    return (int) key;
//}

long FNVHash1(long key) {
    key = (~key) + (key << 18); // key = (key << 18) - key - 1;
    key = key ^ (key >> 31);
    key = key * 21; // key = (key + (key << 2)) + (key << 4);
    key = key ^ (key >> 11);
    key = key + (key << 6);
    key = key ^ (key >> 22);
    return key;
}

/** by Thomas Wang **/
//int IntHash(int key){
//  key = (~key) + (key << 18);
//  key = key ^ (key >> 31);
//  key = (key + (key << 2)) + (key << 4);
//  key = key ^ (key >> 11);
//  key = key + (key << 6);
//  key = key ^ (key >> 22);
//  return key;
//}

long IntHash(long key){
  key = (~key) + (key << 18);
  key = key ^ (key >> 31);
  key = (key + (key << 2)) + (key << 4);
  key = key ^ (key >> 11);
  key = key + (key << 6);
  key = key ^ (key >> 22);
  return key;
}
