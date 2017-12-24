#include "array.h"

// Library
#include <stdlib.h>
#include <string.h>

// Engine
#include "log/log.h"


int initArray(struct Array *array) {
    array->size = INIT_ARRAY_SIZE;
    array->count = 0;
    array->data = malloc(sizeof(void *) * INIT_ARRAY_SIZE);
    return 0;
}

int clearArray(Array *array) {
    unsigned int i;
    for (i = 0; i < array->count; i++) {
        array->data[i] = NULL;
    }
    array->count = 0;
    return 0;
}

int closeArray(struct Array *array) {
    free(array->data);
    return 0;
}

void *getDataArray(const Array *array, unsigned int index) {
    if (index >= array->count) {
        writeLog(LOG_GAME, "array::getDataArray(): Error, index out of range '%d'.", index);
        return NULL;
    }
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

void *deleteArrayIndex(Array *array, unsigned int index) {
    if (index >= array->count) {
        return NULL;
    }

    void *result = array->data[index];
    unsigned int i;
    array->count--;
    for (i = index; i < array->count; i++) {
        array->data[i] = array->data[i + 1];
    }
    array->data[i] = NULL;
    return result;
}
