#include "array.h"

// Library
#include <stdlib.h>
#include <string.h>

// Engine
#include "log/log.h"

namespace rlx {

template <class T>
Array<T>::Array() {
    size = INIT_ARRAY_SIZE;
    count = 0;
    data = malloc(sizeof(void *) * INIT_ARRAY_SIZE);
}

template <class T>
Array<T>::~Array() {
    free(data);
}

template <class T>
void Array<T>::clear() {
    unsigned int i;
    for (i = 0; i < count; i++) {
        data[i] = NULL;
    }
    count = 0;
}

template <class T>
T *Array<T>::get(unsigned int index) const {
    if (index >= count) {
        writeLog(LOG_GAME, "array::getDataArray(): Error, index out of range '%d'.", index);
        return NULL;
    }
    return data[index];
}

template <class T>
int Array<T>::push(T *data) {
    if (count >= size) {
        void **tmp = malloc(sizeof(void *) * size * 2);
        memcpy(tmp, data, sizeof(void *) * size);
        free(data);
        data = tmp;
        size *= 2;
    }
    data[count++] = data;
    return count;
}

template <class T>
T *Array<T>::pop() {
    T *ret = data[--count];
    data[count] == NULL;
    return ret;
}

template <class T>
T *Array<T>::deleteIndex(unsigned int index) {
    if (index >= count) {
        return NULL;
    }

    void *result = data[index];
    unsigned int i;
    count--;
    for (i = index; i < count; i++) {
        data[i] = data[i + 1];
    }
    data[i] = NULL;
    return result;
}

}
