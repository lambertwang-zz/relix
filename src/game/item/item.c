#include "item.h"

// Library
#include <string.h>

int elementStrToInt(char *src) {
    if (strcmp(src, ELEM_TRUE_STR)) {
        return ELEM_TRUE;
    }
    if (strcmp(src, ELEM_PHYS_STR)) {
        return ELEM_PHYS;
    }
    if (strcmp(src, ELEM_FIRE_STR)) {
        return ELEM_FIRE;
    }
    if (strcmp(src, ELEM_ICE_STR)) {
        return ELEM_ICE;
    }
    if (strcmp(src, ELEM_LIGHTNING_STR)) {
        return ELEM_LIGHTNING;
    }
    if (strcmp(src, ELEM_HOLY_STR)) {
        return ELEM_HOLY;
    }
    return -1;
}

int elementIntToStr(int src, char *dest) {
    switch (src) {
        case ELEM_TRUE:
            strcpy(dest, ELEM_TRUE_STR);
            return 0;
        case ELEM_PHYS:
            strcpy(dest, ELEM_PHYS_STR);
            return 0;
        case ELEM_FIRE:
            strcpy(dest, ELEM_FIRE_STR);
            return 0;
        case ELEM_ICE:
            strcpy(dest, ELEM_ICE_STR);
            return 0;
        case ELEM_LIGHTNING:
            strcpy(dest, ELEM_LIGHTNING_STR);
            return 0;
        case ELEM_HOLY:
            strcpy(dest, ELEM_HOLY_STR);
            return 0;
        default:
            return 1;
    }
}

