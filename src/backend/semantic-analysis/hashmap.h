#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


typedef void * tAny;
typedef uint64_t (*hashFp)(tAny);
typedef struct hashMapCDT * hashMapADT;

hashMapADT newHashMap(uint64_t keySize, uint64_t valueSize, hashFp prehash);

void insertOrUpdate(hashMapADT hm, tAny key, tAny value);

bool remove(hashMapADT hm, tAny key);

bool find(hashMapADT hm, tAny key, tAny value);

uint64_t size(hashMapADT hm);

void freeHashMap(hashMapADT hm);

#endif
