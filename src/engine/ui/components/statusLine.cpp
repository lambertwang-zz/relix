#include "statusLine.h"

namespace rlx {

StatusLine::StatusLine() {
    status_mode = 0;
    bg_c = COLOR_BLACK;
    bg_c_focus = COLOR_RED;
    focusable = 1;

    UiManager::registerUiElement(this);
}

int StatusLine::onClick() {
    return 0;
}

}
