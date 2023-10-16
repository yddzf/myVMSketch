//
//  main.cpp
//  AROMA
//
//  Created by yaoda on 2023/9/26.
//
#include <stdio.h>
#include <regex>
#include <cstdlib>
#include <random>
#include <chrono>

#include "kPersistent.hpp"
#include "GeneralHash.hpp"

using namespace std;

//#define HASHTABLE_CAPACITY_BASE 10
//#define VIRTUAL_BITMAP_CAPACITY 10
//#define ROW 3

//#define HASHTABLE_CAPACITY_BASE 43691 //24bit
//#define HASHTABLE_CAPACITY_BASE 52429 //20bit
#define HASHTABLE_CAPACITY_BASE 1024 * 64 * 1 //16bit, 1Mb
//#define HASHTABLE_CAPACITY_BASE 1024 * 13 //16bit, 0.2Mb
//#define HASHTABLE_CAPACITY_BASE 87381 //12bit
//#define HASHTABLE_CAPACITY_BASE 1024 * 64 * 2 //8bit
#define VIRTUAL_BITMAP_CAPACITY 10000 * 0.5
#define ROW 1

int main(int argc, const char * argv[]) {
    //cout << sizeof(unsigned short) << numeric_limits<unsigned short>::max() << endl;
    for (int round_C = 5; round_C >= 5; round_C /= 10){
        printf("Spread Calaulate Start!\n");
        int HASHTABLE_CAPACITY = round_C * HASHTABLE_CAPACITY_BASE;
        //int VBCapacity = round_C * VIRTUAL_BITMAP_CAPACITY;
        printf("%d\n", HASHTABLE_CAPACITY);
        //srand(time(NULL));
        srand(static_cast<unsigned int>(time(NULL)));
        // 使用当前时间作为随机种子
        long Random[4];
        for (int i = 0; i < 4; i++){
            Random[i] = rand();
            //cout << Random[i] << endl;
        }
        int len = VIRTUAL_BITMAP_CAPACITY;
        long Random_Map[len];
        for (int i = 0; i < VIRTUAL_BITMAP_CAPACITY; i++){
            Random_Map[i] = rand();
            //cout << Random[i] << endl;
        }
//        Random[0] = 377493508;
//        Random[1] = 866695718;
//        Random[2] = 173354825;
//        Random[3] = 792057962;
        
        AROMA myAROMA(ROW, HASHTABLE_CAPACITY, VIRTUAL_BITMAP_CAPACITY);
        
        for (int i = 1; i <= ROW; i++){
            printf("Start: %d\n", i);
            char Path[100];
            //snprintf(Path, sizeof(Path), "/Users/yaoda/Desktop/Paper/Haibo/INFOCOM2024/dataset/test/o%dtest.txt", i);
            //snprintf(Path, sizeof(Path), "/Users/yaoda/Desktop/Paper/Haibo/INFOCOM2024/dataset/dataset_nonduplicate/o%d.txt", i);
            snprintf(Path, sizeof(Path), "/Users/yaoda/Desktop/Paper/Haibo/INFOCOM2024/dataset/dataset_nonduplicate/oo%d.txt", i);
            //snprintf(Path, sizeof(Path), "/Users/yaoda/Desktop/Paper/Haibo/INFOCOM2024/dataset/dataset_yuanshi/o%d.txt", i);
            FILE *fp = fopen(Path, "r");
            if (fp == NULL){
                printf("Error opening file!\n");
                exit(1);
            }
            // 定义缓冲区
            char buffer[32];
            // 使用 fgets 读取文件中的每一行
//            int pktCnt = 0;
//            auto start = std::chrono::high_resolution_clock::now();
            
            while (fgets(buffer, sizeof(buffer), fp) != NULL){
                // 在这里处理每一行的内容
                //pktCnt++;
                if (buffer[0] == '\n'){
                    continue;
                }
                char FlowLabel[16];
                char ElementID[16];
                int m = 0, n = 0;
                int Flag = 0;
                if (strlen(buffer) == 0){
                    continue;
                }
                for (int j = 0; j < strlen(buffer); j++){
                    if (buffer[j] == '.' | buffer[j] == '\t' | buffer[j] == ','){
                        if (buffer[j] == '\t' | buffer[j] == ','){
                            Flag = 1;
                        }
                        continue;
                    }
                    if (buffer[j] == '\r' | buffer[j] == '\n' | buffer[j] == ','){
                        break;
                    }
                    else{
                        if (Flag == 0){
                            FlowLabel[m++] = buffer[j];
                        }
                        if (Flag == 1){
                            ElementID[n++] = buffer[j];
                        }
                    }
                }
                FlowLabel[m] = '\0';
                ElementID[n] = '\0';
                string str_FlowLabel = FlowLabel;
                string str_ElementID = ElementID;
//                cout << str_FlowLabel << ", " << str_ElementID << endl;
                //myAROMA.HashTableInsert(str_FlowLabel, str_ElementID, i - 1, Random);
                myAROMA.HashTableInsert(str_ElementID, str_FlowLabel, i - 1, Random, Random_Map);
            }
//            auto end = std::chrono::high_resolution_clock::now();
//
//            // 计算时间差
//            std::chrono::duration<double> duration = end - start;
//            double seconds = duration.count();
//            double throughput = pktCnt / seconds;
//            cout << "Packet Count is: " << pktCnt << endl;
//            cout << "Duration is: " << seconds << endl;
//            cout << "ThroughPut is: " << throughput << endl;
            // 关闭文件
            printf("End: %d\n", i);
            fclose(fp);
        }
        
        printf("Hello Print HashTableMerge!\n");

        myAROMA.MergeHashTable();

//        myAROMA.PrintHashTable();
//        myAROMA.PrintHashTableMerge();

        FILE *fp[ROW];//路径
        FILE *fp_NoFlowLabel[ROW];//路径
        for (int k = 1; k <= ROW; k++){
            printf("Spread Start: %d\n", k);
            char Path[300];
            char Path_E_NoFlowLabel[300];
            //snprintf(Path, sizeof(Path), "/Users/yaoda/Desktop/Paper/Haibo/INFOCOM2024/dataset/test/output_%d_spread_Estimated.txt", k);
            //snprintf(Path, sizeof(Path), "/Users/yaoda/Desktop/Paper/Haibo/INFOCOM2024/dataset/output/output_%dMb/output_%d_spread_Estimated.txt", round_C, k);
            snprintf(Path, sizeof(Path), "/Users/yaoda/Desktop/Paper/Haibo/INFOCOM2024/dataset/output/output_%dMb/output_%d_spread_Estimated.txt", round_C, k);
            fp[k - 1] = fopen(Path, "w");
            //snprintf(Path_E_NoFlowLabel, sizeof(Path_E_NoFlowLabel), "/Users/yaoda/Desktop/Paper/Haibo/INFOCOM2024/dataset/test/output_%d_spread_Estimated_NoFlowLabel.txt", k);
            //snprintf(Path_E_NoFlowLabel, sizeof(Path_E_NoFlowLabel), "/Users/yaoda/Desktop/Paper/Haibo/INFOCOM2024/Graph/Graph_Data/Graph_Data_%dMb/output_%d_spread_Estimated_NoFlowLabel.txt", round_C, k);
//            snprintf(Path_E_NoFlowLabel, sizeof(Path_E_NoFlowLabel), "/Users/yaoda/Desktop/Paper/Haibo/INFOCOM2024/Graph/Graph_Data/Graph_Data_0.%dMb/output_%d_spread_Estimated_NoFlowLabel.txt", round_C, k);
//            fp_NoFlowLabel[k - 1] = fopen(Path_E_NoFlowLabel, "w");
        }

        //FILE *fp1 = fopen("/Users/yaoda/Desktop/Paper/Haibo/INFOCOM2024/dataset/test/output_FlowLabel_test.txt", "r");
        //FILE *fp1 = fopen("/Users/yaoda/Desktop/Paper/Haibo/INFOCOM2024/dataset/dataset_nonduplicate/output_FlowLabel_HashMap_Sample.txt", "r");
        //FILE *fp1 = fopen("/Users/yaoda/Desktop/Paper/Haibo/INFOCOM2024/dataset/dataset_nonduplicate/output_FlowLabel_HashMap_copy.txt", "r");
        FILE *fp1 = fopen("/Users/yaoda/Desktop/Paper/Haibo/INFOCOM2024/dataset/dataset_nonduplicate/output_FlowLabel_HashMap.txt", "r");
        if (fp1 == NULL){
            printf("Error opening file!\n");
            exit(1);
        }
        // 定义缓冲区
        char buffer[32];
        vector<float> EstimatedKSpreadNum(ROW);
        // 使用 fgets 读取文件中的每一行
        while (fgets(buffer, sizeof(buffer), fp1) != NULL){
            buffer[strlen(buffer) - 1] = '\0';
            if (strcmp(buffer, "") == 0){
                continue;
            }
            string Flowlabel = buffer;
            EstimatedKSpreadNum = myAROMA.EstimatedKSpread(Flowlabel, Random, Random_Map);

            for (int m = 0; m < ROW; m++){
                fprintf(fp[m],"%s, %f\n", buffer, EstimatedKSpreadNum[m]);
                fflush(fp[m]);
//                fprintf(fp_NoFlowLabel[m],"%f\n", EstimatedKSpreadNum[m]);
//                fflush(fp_NoFlowLabel[m]);
            }
        }

        fclose(fp1);
        for (int k = 0; k < ROW; k++){
            fflush(fp[k]);
//            fflush(fp_NoFlowLabel[k]);
            fclose(fp[k]);
//            fclose(fp_NoFlowLabel[k]);
            printf("Spread End: %d\n", k);
        }

        printf("Spread Calaulate End!\n");
    }
    cout << "Hello, World!\n";
    return 0;
}
