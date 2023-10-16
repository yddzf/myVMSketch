

#ifndef GeneralHash_hpp
#define GeneralHash_hpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cstring>

/** a hash function that maps the string value to int value */
int FNVHash1(char* data);

//int FNVHash1(long key);

long FNVHash1(long key);

//int IntHash(long key); // by Thomas Wang

long IntHash(long key);
#endif /* GeneralHash_hpp */
