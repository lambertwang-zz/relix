#include "array.h"

#include <stdlib.h>
#include <string.h>

int initArray(struct Array *array) {
    array->size = INIT_ARRAY_SIZE;
    array->count = 0;
    array->data = malloc(sizeof(void *) * INIT_ARRAY_SIZE);
    return 0;
}

int closeArray(struct Array *array) {
    free(array->data);
    return 0;
}

void *getDataArray(struct Array *array, unsigned int index) {
    return array->data[index];
}

int push(struct Array *array, void *data) {
    if (array->count >= array->size) {
        void **tmp = malloc(sizeof(void *) * array->size * 2);
        memcpy(tmp, array->data, sizeof(void *) * array->size);
        free(array->data);
        array->data = tmp;
        array->size *= 2;
    }
    array->data[array->count++] = data;
    return array->count;
}

void *pop(struct Array *array) {
    return array->data[--array->count];
}

