#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "hashmap.h"

struct key {
    //TODO: add scope?
    char * varname;
};

typedef enum VarType {
    VAR_RBT,
    VAR_BST,
    VAR_AVL,
    VAR_INT
} VarType;

struct metadata {
    int value;
};

struct value {
    VarType type;
    struct metadata metadata;
};

void symbolTableInit();

bool symbolTableFind(struct key key, struct value * value);

void symbolTableInsert(struct key key, struct value value);

void symbolTableDestroy();
#endif
