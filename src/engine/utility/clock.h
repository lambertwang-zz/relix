#ifndef __CLOCK_H__
#define __CLOCK_H__

namespace rlx {
    class Clock {
    private:
        unsigned long previous_time;
    public:
        Clock();

        unsigned long split() const;
        unsigned long delta();
    };

    char *timeString();
}

#endif

