#include "objectManager.h"

int initObjects() {
    initTree(&object_manager.insert_queue);
    initTree(&object_manager.object_list);
    initTree(&object_manager.remove_queue);
}
