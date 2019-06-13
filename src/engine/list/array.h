#ifndef __ARRAY_H__
#define __ARRAY_H__

// Engine includes
#include "unique.h"

#define INIT_ARRAY_SIZE 64

namespace rlx {
    template <class T>
    class Array: private Unique {
    private:
        T *data;
        unsigned int size;
        unsigned int count;

    public:
        Array();
        ~Array();

        void clear();

        T *get(unsigned int index) const;
        int push(T *data);
        T *pop();
        T *deleteIndex(unsigned int index);

        unsigned int getCount() const { return count; }
    };
}

#endif
