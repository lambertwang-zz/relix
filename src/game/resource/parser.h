#ifndef __PARSER_H__
#define __PARSER_H__

// Library
#include <stdio.h>

// Engine
#include "constants.h"
#include "list/array.h"

#define NODE_NULL 0
#define NODE_STRING 1
#define NODE_INT 2
#define NODE_ARRAY 3
#define NODE_OBJ 4

typedef struct NodeVal {
    int type;
    char label[LABEL_MED];
    void *data;
} NodeVal;

// Resource Node
typedef struct RNode {
    Array values;
} RNode;

void closeRNode(RNode *node);
RNode *parseFile(FILE *file);

#endif

