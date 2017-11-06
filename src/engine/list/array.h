#ifndef __ARRAY_H__
#define __ARRAY_H__


#define INIT_ARRAY_SIZE 64

// Unsorted dynamic array
struct Array {
    void **data;
    unsigned int size;
    unsigned int count;
};

int initArray(struct Array *array);
int closeArray(struct Array *array);

void *getDataArray(struct Array *array, unsigned int index);
int push(struct Array *array, void *data);
void *pop(struct Array *array);

#endif

