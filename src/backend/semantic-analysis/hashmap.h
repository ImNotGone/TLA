#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


typedef void * tAny;
typedef uint64_t (*hashFp)(tAny);
typedef struct hashMapCDT * hashMapADT;

hashMapADT hashMapInit(uint64_t keySize, uint64_t valueSize, hashFp prehash);

void hashMapInsertOrUpdate(hashMapADT hm, tAny key, tAny value);

bool hashMapRemove(hashMapADT hm, tAny key);

bool hashMapFind(hashMapADT hm, tAny key, tAny value);

uint64_t hashMapsize(hashMapADT hm);

void hashMapDestroy(hashMapADT hm);

#endif
