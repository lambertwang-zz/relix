#ifndef __UNIQUE_H__
#define __UNIQUE_H__

namespace rlx {
    class Unique {
    protected:
        unsigned long id;

        void setInitialId() {
            static unsigned long id_iterator = 0;
            id = ++id_iterator;
        }

        Unique(unsigned long initialId) {
            id = initialId;
        }

    public:
        Unique() {
            setInitialId();
        }

        unsigned long getId() const { return id; };
        void setId(unsigned long new_id) { id = new_id; };
    };
}

#endif
