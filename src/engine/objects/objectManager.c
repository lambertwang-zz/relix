#include "objectManager.h"

#include "../constants.h"
#include "../log/log.h"

#include <stdlib.h>
#include <string.h>

int ensureEventExists(char *function, int ev_id) {
    if (ev_id >= object_manager.event_count) {
        writeLog(LOG_OBJECTMANAGER, "objectManager::%s(): ERROR: Event id %d not in range", function, ev_id);
        return 1;
    }
    if (object_manager.event_listeners[ev_id] == NULL) {
        writeLog(LOG_OBJECTMANAGER, "objectManager::%s(): ERROR: Event id %d not registered", function, ev_id);
        return 2;
    }

    return 0;
}

int sendEvent(Event ev) {
    if (ensureEventExists("sendEvent", ev.id)) {
        return 1;
    }
    // writeLog(LOG_OBJECTMANAGER, "objectManager::registerEvent(): Registering event id %d", ev_id);
    struct Iterator *it;

    it = initIterator(object_manager.event_listeners[ev.id]);
    while (!done(it)) {
        struct Object *obj= getNext(it)->data;
        obj->event_listeners[ev.id](obj, ev);
    }
    closeIterator(it);

    if (ev.data != NULL) {
        free(ev.data);
    }

    return 0;
}

int registerListener(const struct Object *obj, int ev_id) {
    if (ensureEventExists("registerListener", ev_id)) {
        return 1;
    }

    if (insert(object_manager.event_listeners[ev_id], (void *)obj, obj->id)) {
        writeLog(LOG_OBJECTMANAGER, "objectManager::registerListener(): WARN: Object id %d already registered to event id %d", obj->id, ev_id);
        return 1;
    }
    
    return 0;
}

int unregisterListener(const struct Object *obj, int ev_id) {
    if (ensureEventExists("unregisterListener", ev_id)) {
        return 1;
    }

    if (removeId(object_manager.event_listeners[ev_id], obj->id)) {
        writeLog(LOG_OBJECTMANAGER, "objectManager::unregisterListener(): WARN: Object id %d already unregistered from event id %d", obj->id, ev_id);
        return 1;
    }
    
    return 0;
}

int registerEvent(int ev_id) {
    if (ev_id >= object_manager.event_count) {
        writeLog(LOG_OBJECTMANAGER, "objectManager::registerEvent(): Extending event range event id %d event count %d",
                        ev_id, object_manager.event_count);
        struct Tree **tmp = malloc(sizeof(struct Tree *) * object_manager.event_count * 2);
        memcpy(tmp, object_manager.event_listeners, sizeof(struct Tree *) * object_manager.event_count);
        free(object_manager.event_listeners);
        object_manager.event_listeners = tmp;
        int i;
        for (i = object_manager.event_count; i < object_manager.event_count * 2; i++) {
            object_manager.event_listeners[i] = NULL;
        }
        object_manager.event_count *= 2;
    }
    if (object_manager.event_listeners[ev_id] == NULL) {
        object_manager.event_listeners[ev_id] = malloc(sizeof(struct Tree *));
        initTree(object_manager.event_listeners[ev_id]);
        writeLog(LOG_OBJECTMANAGER, "objectManager::registerEvent(): Registering event id %d", ev_id);
        return 0;
    }
    writeLog(LOG_OBJECTMANAGER, "objectManager::registerEvent(): WARNING: event id %d already registered", ev_id);
    return 1;
}

int initObjects() {
    writeLog(LOG_OBJECTMANAGER, "objectManager::initObjects(): Initializing object manager");
    initTree(&object_manager.insert_queue);
    initTree(&object_manager.object_list);
    initTree(&object_manager.remove_queue);

    object_manager.event_listeners = malloc(sizeof(struct Tree *) * INIT_EVENT_COUNT);
    object_manager.event_count = INIT_EVENT_COUNT;
    int i;
    for (i = 0; i < INIT_EVENT_COUNT; i++) {
        object_manager.event_listeners[i] = NULL;
    }

    return 0;
}

int closeObjects() {
    writeLog(LOG_OBJECTMANAGER, "objectManager::closeObjects(): Closing object manager");
    clearObjects();
    closeTree(&object_manager.insert_queue);
    closeTree(&object_manager.object_list);
    closeTree(&object_manager.remove_queue);

    int i;
    for (i = 0; i < object_manager.event_count; i++) {
        if (object_manager.event_listeners[i] != NULL) {
            closeTree(object_manager.event_listeners[i]);
            free(object_manager.event_listeners[i]);
        }
    }
    free(object_manager.event_listeners);
    
    return 0;
}

int clearObjects() {
    writeLog(LOG_OBJECTMANAGER, "objectManager::clearObjects(): Clearing game state");
    struct Iterator *it;

    it = initIterator(&object_manager.insert_queue);
    while (!done(it)) {
        struct Node *node = getNext(it);
        closeObject(node->data);
    }
    closeIterator(it);

    it = initIterator(&object_manager.remove_queue);
    while (!done(it)) {
        struct Node *node = getNext(it);
        closeObject(node->data);
    }
    closeIterator(it);

    it = initIterator(&object_manager.object_list);
    while (!done(it)) {
        struct Node *node = getNext(it);
        closeObject(node->data);
    }
    closeIterator(it);

    clearTree(&object_manager.insert_queue);
    clearTree(&object_manager.object_list);
    clearTree(&object_manager.remove_queue);

    int i;
    for (i = 0; i < object_manager.event_count; i++) {
        if (object_manager.event_listeners[i] != NULL) {
            clearTree(object_manager.event_listeners[i]);
        }
    }
    
    return 0;
}


int addObject(struct Object *obj) {
    return insert(&object_manager.insert_queue, obj, obj->id);
}

int removeObject(struct Object *obj) {
    return insert(&object_manager.remove_queue, obj, obj->id);
}

int updateObjects() {
    struct Iterator *it;

    it = initIterator(&object_manager.insert_queue);
    while (!done(it)) {
        struct Node *node = getNext(it);
        insert(&object_manager.object_list, node->data, node->id);
    }
    closeIterator(it);
    clearTree(&object_manager.insert_queue);

    it = initIterator(&object_manager.remove_queue);
    while (!done(it)) {
        struct Node *node = getNext(it);
        closeObject(node->data);
        removeId(&object_manager.object_list, node->id);
    }
    closeIterator(it);
    clearTree(&object_manager.remove_queue);

    it = initIterator(&object_manager.object_list);
    while (!done(it)) {
        struct Object *obj = getNext(it)->data;
        obj->update(obj);
    }
    closeIterator(it);

    return 0;
}

int renderObjects() {
    struct Iterator *it;
    int objects_rendered = 0;

    it = initIterator(&object_manager.object_list);
    while (!done(it)) {
        struct Object *obj = getNext(it)->data;
        objects_rendered += obj->render(obj);
    }
    closeIterator(it);

    return objects_rendered;
}

