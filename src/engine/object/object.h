#ifndef __OBJECT_H__
#define __OBJECT_H__

// Engine
#include "list/unique.h"
#include "event/event.h"
#include "term/screen.h"

#define TYPE_MAX_LEN 32

#define OBJ_SOLID 1
#define OBJ_ETHER 0 // Ethereal
#define OBJ_SOFT 0

namespace rlx {
    class Object: public Unique {
    private:
        String *type;

        // pos.z is used for depth rendering and checking.
        // Note: depth 1024 is reserved for UI elements
        Point position;
        Pixel pix;

        // Whether or not the object should generate collisions
        bool isSolid;
    public:
        Object();
        virtual ~Object();

        Point getPosition() const { return position; }
        bool getIsSolid() const { return isSolid; }

        int render(Screen *s);
        int renderLight(Screen *s);

        virtual int update();
    };
}

#endif

