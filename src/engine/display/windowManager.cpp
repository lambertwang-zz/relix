#include "windowManager.h"

// Library
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#if defined __linux__
#include <pthread.h>
#elif defined _WIN32 || defined _WIN64
#include <windows.h>
#else
#error "unknown platform"
#endif

#include "utility/utility.h"
#include "log/log.h"

namespace rlx {

static int abort_output_thread_flag;

#if defined __linux__
static pthread_t output_thread;
#elif defined _WIN32 || defined _WIN64
static DWORD output_thread_id;
static HANDLE output_thread;
#endif

void *outputThreadWork() {
    writeLog(LOG_SCREEN, "screenManager::outputThreadWork(): Starting output thread.");
    while (!abort_output_thread_flag) {
        dynamic_cast<WindowManager*>(WindowManager::getInstance())->render();
    }
    writeLog(LOG_SCREEN, "screenManager::outputThreadWork(): Exiting output thread.");
    return 0;
}

WindowManager::WindowManager() {
    writeLog(LOG_SCREEN, "WindowManager::WindowManager(): Initializing window manager.");

#if defined __linux__
    sem_init(&screen_manager.writes_allowed, 0, 1);
    sem_init(&screen_manager.reads_allowed, 0, 0);
#elif defined _WIN32 || defined _WIN64
    screen_manager.writes_allowed = CreateSemaphore(NULL, 1, 1, NULL);
    screen_manager.reads_allowed = CreateSemaphore(NULL, 0, 1, NULL);
#endif

    abort_output_thread_flag = 0;

    window = sd::Window(sf::VideoMode(800, 600),
        DEFAULT_WINDOW_NAME,
        sf::Style::Default);

#if defined __linux__
    pthread_create(&output_thread, NULL, outputThreadWork, NULL);
#elif defined _WIN32 || defined _WIN64
    output_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) outputThreadWork, NULL, 0, &output_thread_id);
#endif

    writeLog(LOG_SCREEN, "WindowManager::WindowManager(): Successfully initialized window manager.");
}

WindowManager::~WindowManager() {
    writeLog(LOG_SCREEN, "WindowManager::~WindowManager(): Closing the screen manager.");
    // Safely terminate the output thread
    // There's no way that the engine can write to the buffer anymore, so wake
    // up the output thread to flush the last frame and check the exit flag.
    abort_output_thread_flag = 1;
#if defined __linux__
    sem_post(&screen_manager.reads_allowed);
    pthread_join(output_thread, NULL);
#elif defined _WIN32 || defined _WIN64
    ReleaseSemaphore(screen_manager.reads_allowed, 1, NULL);
    WaitForSingleObject(output_thread, INFINITE);
#endif

#if defined __linux__
    sem_destroy(&screen_manager.reads_allowed);
    sem_destroy(&screen_manager.writes_allowed);
#elif defined _WIN32 || defined _WIN64
    CloseHandle(screen_manager.reads_allowed);
    CloseHandle(screen_manager.writes_allowed);
#endif

    writeLog(LOG_SCREEN, "WindowManager::~WindowManager(): Successfully closed the screen manager.");
}

int WindowManager::render() {
    // Wait for the engine to finish preparing this frame
#if defined __linux__
    sem_wait(&screen_manager.reads_allowed);
#elif defined _WIN32 || defined _WIN64
    WaitForSingleObject(screen_manager.reads_allowed, INFINITE);
#endif

    // Resizing can only happen when the engine isn't touching the buffer
    if (resize_flag) {
        resize_flag = 0;
    }

    // Copy the prepared frame into our thread's internal buffer
    // This will result in a blank frame if the window was resized
    writeLog(LOG_SCREEN_V, "screenManager::renderScreens(): Copying prepared frame to screen buffer.");
    // memcpy(screen->current_pixel_buffer, screen->pixel_buffer, sizeof(Pixel) * screen->ts.cols * screen->ts.lines);

    // Let the engine know that it's safe to continue rendering
#if defined __linux__
    sem_post(&screen_manager.writes_allowed);
#elif defined _WIN32 || defined _WIN64
    ReleaseSemaphore(screen_manager.writes_allowed, 1, NULL);
#endif

    // Copy buffers
    // memcpy(screen->prev_pixel_buffer, screen->current_pixel_buffer, sizeof(Pixel) * screen->ts.cols * screen->ts.lines);

    return 0;
}

WindowWriteState WindowManager::waitForWrites() {
#if defined __linux__
    if (sem_trywait(&writes_allowed)) {
        return WRITES_ALLOWED;
    }
#elif defined _WIN32 || defined _WIN64
    DWORD dwWaitResult; 

    // zero-second time-out interval
    dwWaitResult = WaitForSingleObject(writes_allowed, 0L);

    switch (dwWaitResult) 
    { 
        // The semaphore object was signaled.
        case WAIT_OBJECT_0: 
            return WRITES_ALLOWED;
        case WAIT_TIMEOUT:
        default:
            return WRITES_NOT_ALLOWED;
    }
#endif
    return WRITES_NOT_ALLOWED;
}

void WindowManager::allowReads() {
#if defined __linux__
    sem_post(&reads_allowed);
#elif defined _WIN32 || defined _WIN64
    ReleaseSemaphore(reads_allowed, 1, NULL);
#endif
}

}
