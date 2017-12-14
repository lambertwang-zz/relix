#include "string.h"

// Library
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Engine
#include "log/log.h"

String *createString() {
    String *new_str = malloc(sizeof(String));
    new_str->s = malloc(sizeof(char) * INIT_STR_SIZE);
    new_str->size = INIT_STR_SIZE;
    new_str->len = 0;
    new_str->s[0] = '\0';
    return new_str;
}

void clearString(String *s) {
    s->s[0] = '\0';
    s->len = 0;
}

void deleteString(String *s) {
    if (s != NULL) {
        free(s->s);
        free(s);
    }
}

int strInc(String *s) {
    writeLog(10, "HEY >>>>>>>>>>>>>>>>>>> Doubling string");

    s->size = s->size * 2;
    free(s->s);
    s->s = malloc(sizeof(char) * s->size);

    return s->size;
}

int stringCompare(const String *a, const String *b) {
    if (a == NULL && b == NULL) {
        return 0;
    }
    if (a == NULL || b == NULL) {
        return -1;
    }
    return strcmp(a->s, b->s);
}

char *stringCopy(String *dest, const String *src) {
    if (dest->size <= src->len) {
        dest->size = src->len + 1;
        free(dest->s);
        dest->s = malloc(sizeof(char) * dest->size);
    }

    return strcpy(dest->s, src->s);
}

int sputf(String *s, const char *format, ...) {

    s->len = 0;
    free(s->s);
    s->s = malloc(sizeof(char) * INIT_STR_SIZE);
    s->size = INIT_STR_SIZE;

    do {
        if (s->len > 0) {
            strInc(s);
        }
        va_list args;
        va_start(args, format);
        s->len = vsnprintf(s->s, s->size, format, args);
        va_end(args);
    } while (s->len >= s->size - 1);
    fflush(stderr);
    

    return s->len;
}

int spush(String *s, const char c) {
    if (s->len >= s->size - 1) {
        strInc(s);
    }

    s->s[s->len++] = c;
    s->s[s->len] = '\0';

    return s->len;
}

int sgetc(String *dest, const String *src, const int index) {
    int i = 0;
    char *head = src->s;
    while (i < index && *head != '\0') {
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
            strInc(dest);
        }
        dest->s[new_len++] = *head;
        head++;
    } while (*head != '\0' && (*head & 0xa0) == 0x80);

    dest->s[new_len] = '\0';
    dest->len = new_len;

    // sputf(dest, "%1d", (head - src->s) % 10);
    // sputf(dest, "%1d", (index - i + 10) % 10);

    return new_len;
}

