#ifndef __ARRAY_H__
#define __ARRAY_H__

#define INIT_ARRAY_SIZE 64

// Unsorted dynamic array
typedef struct Array {
    void **data;
    unsigned int size;
    unsigned int count;
} Array;

int initArray(Array *array);
int clearArray(Array *array);
int closeArray(Array *array);

// getDataArray is a little bit pointless.
void *getDataArray(Array *array, unsigned int index);
int push(Array *array, void *data);
void *pop(Array *array);

#endif

