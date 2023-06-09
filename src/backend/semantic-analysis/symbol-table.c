#include "symbol-table.h"
#include "hashmap.h"
#include <stdint.h>

#define SEED 0x13572468

static hashMapADT table = NULL;

// MurmurOAAT_32_Modified
static uint64_t symbolHashFunction(tAny key) {

    uint64_t h = SEED;
    struct key aux = *(struct key*) key;

    const char * str = aux.varname;
    // One-byte-at-a-time hash based on Murmur's mix
    // Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp
    for (; *str; ++str) {
        h ^= *str;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}

void symbolTableInit() {
    if(table != NULL) {
        symbolTableDestroy();
    }
    table = newHashMap(sizeof(struct key), sizeof(struct value), symbolHashFunction);
}

bool symbolTableFind(struct key key, struct value * value) {
    return find(table, &key, value);
}

void symbolTableInsert(struct key key, struct value value) {
    insertOrUpdate(table, &key, &value);
}

void symbolTableDestroy() {
    freeHashMap(table);
    table = NULL;
}
