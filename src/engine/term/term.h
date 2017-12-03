#ifndef __TERM_H__
#define __TERM_H__

typedef struct TermSize {
    int lines;
    int cols;
} TermSize;

int getTermSize(TermSize *ts);

#endif

