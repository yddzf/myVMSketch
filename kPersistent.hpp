

#ifndef kPersistent_hpp
#define kPersistent_hpp

#include <stdio.h>
#include <iostream>
#include <cmath>
#include <string>

#include "GeneralHash.hpp"

using namespace std;

//struct HashNode{
//    string FlowLabel;
//    string ElementID;
//    float HashValue;
//};

class AROMA{
private:
    int Row;
    int HashTableCapacity;
    double SumOfHashValue;
    int SumOfNonNULL;
    //vector<vector<unsigned short>> *HashTable;
    //vector<unsigned short> *HashTableMerge;
    vector<vector<int>> *HashTable;
    vector<int> *HashTableMerge;
    
//    vector<vector<float>> *HashTable;
//    vector<float> *HashTableMerge;
    int VBitMapCapacity;
    
public:
    AROMA(int Row, int HashTableCapacity, int VBitMapCapacity);
    
    ~AROMA();
    
    void HashTableInsert(string FlowLabel, string ElementID, int RowIndex, long Random[], long Random_Map[]);

    //Print Data in HashTable
    void PrintHashTable();

    //Merge t period HashTable
    void MergeHashTable();

    void PrintHashTableMerge();


    vector<float> EstimatedKSpread(string FlowLabel, long Random[], long Random_Map[]);
};

#endif /* kPersistent_hpp */
