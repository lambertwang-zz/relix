#ifndef __EVENTS_H__
#define __EVENTS_H__

// Engine includes
#include "list/unique.h"

namespace rlx {
    class Event {
    private:
        // Set by ObjectManager on initialization
        // If handler sets to 0, event is not passed on to next objects.
        // Note: There is no ordering for non-ui objects 
        bool stop_propagation;

        unsigned int type;

        // Not allowed
        Event();
    public:
        Event(unsigned int new_type);

        unsigned int getType() const { return type; }
        void stopPropagation() { stop_propagation = true; }
        bool shouldStopPropagation() { return stop_propagation; }
    };

    template <class T>
    class Listener: public Unique {
    private:
        T *target;
    public:
        Listener(T *new_target): Unique(new_target->getId()) {
            target = new_target;
        };
        virtual ~Listener();

        virtual int apply(Event *ev);
    };
}

#endif

