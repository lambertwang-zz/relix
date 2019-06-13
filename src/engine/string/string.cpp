#include "string.h"

// Library
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Engine
#include "log/log.h"

namespace rlx {

String::String() {
    buffer = (char *)malloc(sizeof(char) * INIT_STR_SIZE);
    size = INIT_STR_SIZE;
    clear();
}

String::~String() {
    free(buffer);
}

void String::clear() {
    buffer[0] = '\0';
    len = 0;
}

int String::incrementSize() {
    char *tmp = (char *)malloc(sizeof(char) * size * 2);
    strcpy(tmp, buffer);
    free(buffer);
    size = size * 2;
    buffer = tmp;

    return size;
}

int String::compare(const String *other) const {
    return stringCompare(this, other);
}

int stringCompare(const String *a, const String *b) {
    if (a == NULL && b == NULL) {
        return 0;
    }
    if (a == NULL || b == NULL) {
        return -1;
    }
    return strcmp(a->getBuffer(), b->getBuffer());
}

char *String::copy(const String *src) {
    if (size <= src->len) {
        size = src->len + 1;
        free(buffer);
        buffer = (char *)malloc(sizeof(char) * size);
    }

    len = src->len;

    return strcpy(buffer, src->buffer);
}

int String::sputf(const char *format, ...) {
    do {
        if (len > 0) {
            incrementSize();
        }
        va_list args;
        va_start(args, format);
        len = vsnprintf(buffer, size, format, args);
        va_end(args);
    } while (len >= size - 1);
    fflush(stderr);

    return len;
}

int String::spush(const char c) {
    if (len >= size - 1) {
        incrementSize();
    }

    buffer[len++] = c;
    buffer[len] = '\0';

    return len;
}

int String::sgetc(String *dest, const int index) const {
    int i = 0;
    char *head = buffer;
    while (i < index && *head != '\0') {
        if (head >= buffer + len) {
            // return 0;
        }
        // Detect if leading bit is high (utf-8)
        if (*head & 0x80) {
            // Found start of unicode
            if ((*head & 0xa0) == 0x80) {
                i++;
            }
        } else {
            // Found ascii char
            i++;
        }
        head++;
    }

    int new_len = 0;
    do {
        if (new_len >= dest->size - 1) {
            dest->incrementSize();
        }
        dest->buffer[new_len++] = *head;
        head++;
    } while (head < buffer + len - 1 && 
            *head != '\0' && 
            (*head & 0xa0) == 0x80);
    // } while (*head != '\0' && (*head & 0xa0) == 0x80);

    dest->buffer[new_len] = '\0';
    dest->len = new_len;

    return new_len;
}

int String::print() const {
    return printf(buffer);
}

}
