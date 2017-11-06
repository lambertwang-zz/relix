#ifndef __TERM_H__
#define __TERM_H__

struct TermSize {
    int lines;
    int cols;
};

int getTermSize(struct TermSize *ts);

#endif

