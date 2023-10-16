

#include "kPersistent.hpp"

AROMA::AROMA(int Row, int HashTableCapacity, int VBitMapCapacity){
    this->Row = Row;
    this->HashTableCapacity = HashTableCapacity;
    this->SumOfHashValue = 0;
    this->SumOfNonNULL = 0;
    this->VBitMapCapacity = VBitMapCapacity;
    
    //HashTable = new vector<vector<unsigned short>>(Row, vector<unsigned short>(HashTableCapacity));
    HashTable = new vector<vector<int>>(Row, vector<int>(HashTableCapacity));
    for (int i = 0; i < this->Row; i++){
        for (int j = 0; j < HashTableCapacity; j++){
            //(*HashTable)[i][j] = numeric_limits<unsigned short>::max();
            (*HashTable)[i][j] = numeric_limits<int>::max();
        }
    }
    //HashTableMerge = new vector<unsigned short>(HashTableCapacity);
    HashTableMerge = new vector<int>(HashTableCapacity);
    for (int i = 0; i < HashTableCapacity; i++){
        //(*HashTableMerge)[i] = numeric_limits<unsigned short>::max();
        (*HashTableMerge)[i] = numeric_limits<int>::max();
    }
}

AROMA::~AROMA(){
    delete HashTable;
    delete HashTableMerge;
}

void AROMA::HashTableInsert(string FlowLabel, string ElementID, int RowIndex, long Random[], long Random_Map[]){
    long Long_FlowLabel = stol(FlowLabel);
    long Long_ElementID = stol(ElementID);

    long VirtualHash = IntHash(FNVHash1(Long_ElementID) ^ Random[0]);
    long VirtualIndex = (VirtualHash % (long)this->VBitMapCapacity + (long)this->VBitMapCapacity) % (long)this->VBitMapCapacity;

    VirtualIndex = IntHash(FNVHash1(Random_Map[VirtualIndex]) ^ Random[1]);

    long Long_Buffer = Long_FlowLabel ^ VirtualIndex;

    long Hash = IntHash(FNVHash1(Long_Buffer) ^ Random[2]);
    long index = (Hash % (long)this->HashTableCapacity + (long)this->HashTableCapacity) % (long)this->HashTableCapacity;
    
    long hash_value = IntHash(FNVHash1(Long_Buffer) ^ Random[3]);
    
    //int hashvalue = (int)(((hash_value - LONG_MIN) % (long)16777216 + (long)16777216) % (long)16777216); //24bit
    //int hashvalue = (int)(((hash_value - LONG_MIN) % (long)1048576 + (long)1048576) % (long)1048576); //20bit
    //unsigned short hashvalue = (unsigned short)((hash_value - LONG_MIN) % (numeric_limits<unsigned short>::max() + 1)); //16bit
    int hashvalue = (int)(((hash_value - LONG_MIN) % (long)65536 + (long)65536) % (long)65536); //16bit
    //int hashvalue = (int)(((hash_value - LONG_MIN) % (long)4096 + (long)4096) % (long)4096); //12bit
    //int hashvalue = (int)(((hash_value - LONG_MIN) % (long)256 + (long)256) % (long)256); //8bit
    
    if ((*HashTable)[RowIndex][index] > hashvalue){
        (*HashTable)[RowIndex][index] = hashvalue;
    }
}

void AROMA::MergeHashTable(){
    for (int i = 0; i < this->HashTableCapacity; i++){
        //unsigned short MinHashValue = (*HashTable)[0][i];
        int MinHashValue = (*HashTable)[0][i];
        int MinRowIndex = 0;
        for (int j = 1; j < this->Row; j++){
            if ((*HashTable)[j][i] < MinHashValue){
                MinHashValue = (*HashTable)[j][i];
                MinRowIndex = j;
            }
        }
        
        (*HashTableMerge)[i] = (*HashTable)[MinRowIndex][i];
        //this->SumOfHashValue += (*HashTableMerge)[i];
        
        //if ((*HashTableMerge)[i] != numeric_limits<unsigned short>::max()){
        if ((*HashTableMerge)[i] != numeric_limits<int>::max()){
            this->SumOfNonNULL++;
            this->SumOfHashValue += ((*HashTableMerge)[i] + 1);
        }
    }
}

vector<float> AROMA::EstimatedKSpread(string FlowLabel, long Random[], long Random_Map[]){
    vector<float> EstimatedKSpread(this->Row);
    vector<int> SpreadCountArray(this->Row);
    
    long Long_Flowlabel = stol(FlowLabel);
    
    int SumOfNonNULL = this->SumOfNonNULL; // S
    int SumOfFlow = 0;    // sf
    int SumOfSpread = 0;
    
    for (int i = 0; i < this->VBitMapCapacity; i++){
        long Long_Buffer = IntHash(FNVHash1(Random_Map[i]) ^ Random[1]);
        Long_Buffer = Long_Flowlabel ^ Long_Buffer;
        
        long Hash = IntHash(FNVHash1(Long_Buffer) ^ Random[2]);
        long index = (Hash % (long)this->HashTableCapacity + (long)this->HashTableCapacity) % (long)this->HashTableCapacity;

        long hash_value = IntHash(FNVHash1(Long_Buffer) ^ Random[3]);
        
        //int hashvalue = (int)(((hash_value - LONG_MIN) % (long)16777216 + (long)16777216) % (long)16777216); //24bit
        //int hashvalue = (int)(((hash_value - LONG_MIN) % (long)1048576 + (long)1048576) % (long)1048576); //20bit
        //unsigned short hashvalue = (unsigned short)((hash_value - LONG_MIN) % (numeric_limits<unsigned short>::max() + 1)); //16bit
        int hashvalue = (int)(((hash_value - LONG_MIN) % (long)65536 + (long)65536) % (long)65536); //16bit
        //int hashvalue = (int)(((hash_value - LONG_MIN) % (long)4096 + (long)4096) % (long)4096); //12bit
        //int hashvalue = (int)(((hash_value - LONG_MIN) % (long)256 + (long)256) % (long)256); //8bit
        
        if ((*HashTableMerge)[index] == hashvalue){
            int SpreadCount = 0;
            SumOfFlow++;
            for (int j = 0; j < this->Row; j++){
                if ((*HashTable)[j][index] == (*HashTableMerge)[index]){
                    SpreadCount++;
                }
            }
            SpreadCountArray[SpreadCount - 1]++;
        }
    }

    for (int i = this->Row - 1; i >= 0; i--){
        SumOfSpread += SpreadCountArray[i];
        SpreadCountArray[i] = SumOfSpread;
    }

    if (SumOfSpread == 0){
        return EstimatedKSpread;
    }
    
    double alpha = (double)this->SumOfNonNULL / (double)this->HashTableCapacity;
    
    double EstimatedNumOfFlow = 0;
    
    if (alpha >= 0.95){
        //EstimatedNumOfFlow = (double)this->SumOfNonNULL * (double)this->SumOfNonNULL * (double)16777216 / this->SumOfHashValue;//24bit
        //EstimatedNumOfFlow = (double)this->SumOfNonNULL * (double)this->SumOfNonNULL * (double)1048576 / this->SumOfHashValue;//20bit
        EstimatedNumOfFlow = (double)this->SumOfNonNULL * (double)this->SumOfNonNULL * (double)pow(2, 16) / this->SumOfHashValue;//16bit
        //EstimatedNumOfFlow = (double)this->SumOfNonNULL * (double)this->SumOfNonNULL * (double)pow(2, 12) / this->SumOfHashValue;//12bit
        //EstimatedNumOfFlow = (double)this->SumOfNonNULL * (double)this->SumOfNonNULL * (double)pow(2, 8) / this->SumOfHashValue;//8bit
    }
    else{
        EstimatedNumOfFlow = -1 * (double)this->HashTableCapacity * log(1 - (double)this->SumOfNonNULL / (double)this->HashTableCapacity);
    }

    double NfEstimated_1 = EstimatedNumOfFlow * (double)SumOfFlow / (double)SumOfNonNULL;
    
    if(NfEstimated_1 >= this->VBitMapCapacity){
        NfEstimated_1 = this->VBitMapCapacity - 1;
    }

    double NfEstimated = -1 * this->VBitMapCapacity * log(1 - NfEstimated_1 / this->VBitMapCapacity);


    for (int i = 0; i < this->Row; i++){
        EstimatedKSpread[i] = (double)SpreadCountArray[i] / (double)SumOfSpread * NfEstimated;
    }
    
    return EstimatedKSpread;
}

void AROMA::PrintHashTable(){
    for (int i = 0; i < this->Row; i++){
        for (int j = 0; j < this->HashTableCapacity; j++){
            cout << "[" << i << "][" << j << "](" << (*HashTable)[i][j] << ")" << endl;
        }
        cout << endl;
    }
}

void AROMA::PrintHashTableMerge(){
    for (int i = 0; i < this->HashTableCapacity; i++){
        cout << "[" << i << "](" << (*HashTableMerge)[i] << ")" << endl;
    }
    cout << endl;
}
