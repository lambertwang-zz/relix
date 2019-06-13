
#ifndef __KEYBOARD_EVENT_H__
#define __KEYBOARD_EVENT_H__

// Engine includes
#include "constants.h"
#include "event/event.h"

namespace rlx {
    class KeyboardEvent: public Event {
    private:
        char value;
        unsigned int type;
    public:
        KeyboardEvent(char value, unsigned int type);
    };


    class MouseEvent: public Event {
    private:
        unsigned char status;
        int x;
        int y;
        int value;
    public:
        MouseEvent(unsigned char status, int x, int y, int value);
        unsigned char getStatus() const;
    };
}

#endif

