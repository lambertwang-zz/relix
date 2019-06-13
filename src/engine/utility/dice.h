#ifndef __DICE_H__
#define __DICE_H__

// Engine
#include "constants.h"
#include "string/string.h"

#define NULL_DICE (Dice){0, 0, 0}

namespace rlx {
    class Dice {
    private:
        // [count]d[sides]+[flat]
        int count;
        int sides;
        int flat;
    public:
        Dice(int init_count,
            int init_sides,
            int init_flat) {

            count = init_count,
            sides = init_sides;
            flat = init_flat;
        }

        Dice(String *label);

        int roll() const;
        int max() const;
        int sprint(String *dest) const;
        int print() const;
    };
}

#endif

