#ifndef __STATUSLINE_H__
#define __STATUSLINE_H__

#include "ui.h"

namespace rlx {
    class StatusLine: public Element {
    private:
        unsigned int status_mode;
    public:
        StatusLine();

        int onClick();
    };
}

#endif
