#ifndef __SCREEN_H__
#define __SCREEN_H__

// Library
#if defined __linux__
#include <semaphore.h>
#elif defined _WIN32 || defined _WIN64
#include <windows.h>
#else
#error "unknown platform"
#endif
#include <SFML/Graphics.hpp>

// Engine
#include "color.h"
#include "geometry/geometry.h"
#include "list/tree.h"
#include "manager/manager.h"

namespace rlx {
    enum WindowWriteState {
        WRITES_NOT_ALLOWED = 0,
        WRITES_ALLOWED
    };

    // Set this flag to 1 when the window has been resized
    int resize_flag;

    class WindowManager: public Manager {
    private:
    #if defined __linux__
        sem_t writes_allowed;
        sem_t reads_allowed;
    #elif defined _WIN32 || defined _WIN64
        HANDLE writes_allowed;
        HANDLE reads_allowed;
    #endif

        sf::Window window;
    public:
        void initScreen();
        void closeScreen();
        int render();

        int swapScreen();

        WindowWriteState waitForWrites();
        void allowReads();
    };
}

#endif
