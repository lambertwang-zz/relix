#include "objectManager.h"

// Library
#include <stdlib.h>
#include <string.h>

// Engine
#include "constants.h"
#include "log/log.h"

static void (*clearGame)();

int ensureEventExists(char *function, int ev_id) {
    if (getData(&object_manager.event_listeners, ev_id) == NULL) {
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

    // it = initIterator(getData(object_manager.event_listeners[ev.id]);
    it = initIterator(getData(&object_manager.event_listeners, ev.id));
    while (!done(it)) {
        struct Object *obj= getNext(it)->data;

        int (*listener)(struct Object *m, Event ev) = getData(&obj->event_listeners, ev.id);
        if (listener == NULL) {
            writeLog(LOG_OBJECTMANAGER, "objectManager::sendEvent(): WARNING: No listener for Event id %d for object", ev.id, obj->id);
            continue;
        }
        listener(obj, ev);
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

    if (insert(getData(&object_manager.event_listeners, ev_id), (void *)obj, obj->id)) {
        writeLog(LOG_OBJECTMANAGER, "objectManager::registerListener(): WARN: Object id %d already registered to event id %d", obj->id, ev_id);
        return 1;
    }
    
    return 0;
}

int unregisterListener(const struct Object *obj, int ev_id) {
    if (ensureEventExists("unregisterListener", ev_id)) {
        return 1;
    }

    if (removeId(getData(&object_manager.event_listeners, ev_id), obj->id)) {
        writeLog(LOG_OBJECTMANAGER, "objectManager::unregisterListener(): WARN: Object id %d already unregistered from event id %d", obj->id, ev_id);
        return 1;
    }
    
    return 0;
}

int registerEvent(int ev_id) {
    Node *n = getTreeNode(&object_manager.event_listeners, ev_id);
    if (n == NULL) {
        Tree *new_tree = malloc(sizeof(Tree));
        initTree(new_tree);
        insert(&object_manager.event_listeners, new_tree, ev_id);
        writeLog(LOG_OBJECTMANAGER, "objectManager::registerEvent(): Registering event id %d", ev_id);
        return 0;
    }
    writeLog(LOG_OBJECTMANAGER, "objectManager::registerEvent(): WARNING: event id %d already registered", ev_id);
    return 1;
}

int initObjects() {
    writeLog(LOG_OBJECTMANAGER, "objectManager::initObjects(): Initializing object manager");
    clearGame = NULL;

    initTree(&object_manager.insert_queue);
    initTree(&object_manager.object_list);
    initTree(&object_manager.remove_queue);

    initTree(&object_manager.event_listeners);

    return 0;
}

int clearObjects() {
    writeLog(LOG_OBJECTMANAGER, "objectManager::clearObjects(): Clearing game state");
    struct Iterator *it;

    it = initIterator(&object_manager.insert_queue);
    while (!done(it)) {
        struct Object *obj = getNext(it)->data;
        obj->close(obj);
    }
    closeIterator(it);

    it = initIterator(&object_manager.remove_queue);
    while (!done(it)) {
        struct Object *obj = getNext(it)->data;
        obj->close(obj);
    }
    closeIterator(it);

    it = initIterator(&object_manager.object_list);
    while (!done(it)) {
        struct Object *obj = getNext(it)->data;
        obj->close(obj);
    }
    closeIterator(it);

    it = initIterator(&object_manager.event_listeners);
    while (!done(it)) {
        Tree *tree = getNext(it)->data;
        clearTree(tree);
    }
    closeIterator(it);


    clearTree(&object_manager.insert_queue);
    clearTree(&object_manager.object_list);
    clearTree(&object_manager.remove_queue);

    return 0;
}

int closeObjects() {
    writeLog(LOG_OBJECTMANAGER, "objectManager::closeObjects(): Closing object manager");
    clearObjects();
    closeTree(&object_manager.insert_queue);
    closeTree(&object_manager.object_list);
    closeTree(&object_manager.remove_queue);

    Iterator *it = initIterator(&object_manager.event_listeners);
    while (!done(it)) {
        Tree *tree = getNext(it)->data;
        closeTree(tree);
        free(tree);
    }
    closeIterator(it);
    closeTree(&object_manager.event_listeners);
    
    return 0;
}

void queueClear(void (*next)()) {
    clearGame = next;
}

int addObject(struct Object *obj) {
    return insert(&object_manager.insert_queue, obj, obj->id);
}

int removeObject(struct Object *obj) {
    return insert(&object_manager.remove_queue, obj, obj->id);
}

int updateObjects() {
    struct Iterator *it;

    if (clearGame != NULL) {
        clearObjects();
        clearGame();
        clearGame = NULL;
    }

    it = initIterator(&object_manager.insert_queue);
    while (!done(it)) {
        struct Node *node = getNext(it);
        if (insert(&object_manager.object_list, node->data, node->id)) {
            writeLog(LOG_OBJECTMANAGER, "objectManager::updateObjects(): Error attempting to remove object id %d", node->id);
        }
    }
    closeIterator(it);
    clearTree(&object_manager.insert_queue);

    it = initIterator(&object_manager.remove_queue);
    while (!done(it)) {
        struct Object *obj = getNext(it)->data;
        if (removeId(&object_manager.object_list, obj->id)) {
            writeLog(LOG_OBJECTMANAGER, "objectManager::updateObjects(): Error attempting to remove object id %d", obj->id);
        }
        obj->close(obj);
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

int renderObjectLights() {
    struct Iterator *it;
    int lights_rendered = 0;

    // Render lights
    it = initIterator(&object_manager.object_list);
    while (!done(it)) {
        struct Object *obj = getNext(it)->data;
        lights_rendered += obj->renderLight(obj, &screen_manager.main_screen);
    }
    closeIterator(it);

    return lights_rendered;
}

int renderObjects() {
    struct Iterator *it;
    int objects_rendered = 0;

    struct Tree depth_tree;
    initTree(&depth_tree);

    // Insert objects into the depth tree
    it = initIterator(&object_manager.object_list);
    while (!done(it)) {
        struct Object *obj = getNext(it)->data;
        struct Array *depth_node = getData(&depth_tree, obj->pos.z);
        if (depth_node == NULL) {
            depth_node = malloc(sizeof(struct Array));
            initArray(depth_node);
            insert(&depth_tree, depth_node, obj->pos.z);
        }
        push(depth_node, obj);
    }
    closeIterator(it);


    // Render objects by sorted by depth
    it = initIterator(&depth_tree);
    while (!done(it)) {
        Node *node = getNext(it);
        // Array *depth_node = getNext(it)->data;
        Array *depth_node = node->data;
        int i;
        for (i = 0; i < depth_node->count; i++) {
            Object *obj = depth_node->data[i];
            objects_rendered += obj->render(obj, &screen_manager.main_screen);
        }
        closeArray(depth_node);
        free(depth_node);
    }
    closeIterator(it);

    closeTree(&depth_tree);

    return objects_rendered;
}

int getObjAt(Array *array, Point p, int solid) {
    struct Iterator *it;
    it = initIterator(&object_manager.object_list);
    while (!done(it)) {
        struct Object *obj = getNext(it)->data;
        if ((solid && obj->solid) || (!solid && !obj->solid)) {
            if (p.x == obj->pos.x && p.y == obj->pos.y) {
                push(array, obj);
            }
        }
    }
    closeIterator(it);

    return array->count;
}

