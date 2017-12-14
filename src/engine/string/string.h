#ifndef __STRING_H__
#define __STRING_H__

#define INIT_STR_SIZE 8

typedef struct String {
    char *s;
    int size;
    int len;
} String;

String *createString();
void deleteString(String *s);
// Double the size of the string
int strInc(String *s);

int stringCompare(const String *a, const String *b);

char *stringCopy(String *dest, const String *src);


int sputf(String *s, const char *format, ...);
int spush(String *s, const char c);
int sgetc(String *dest, const String *src, const int index);
// status_label, " Loop: %5d Frame: %5d Obj rendered: %3d", loop_time_saved, frame_count, objectsRendered);

#endif

