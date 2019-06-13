#include "objectManager.h"

// Library
#include <stdlib.h>
#include <string.h>

// Engine
#include "constants.h"
#include "log/log.h"
#include "ui/ui.h"


namespace rlx {

int ObjectManager::ensureEventExists(char *function, int ev_id) {
    if (object_event_listeners->get(ev_id) == NULL) {
        writeLog(LOG_ObjectManager,
            "ObjectManager::%s(): ERROR: Event id %d not registered",
            function,
            ev_id);

        return 2;
    }

    return 0;
}

int ObjectManager::sendEvent(Event *ev) {
    if (ensureEventExists((char *)"sendEvent", ev->getType())) {
        return 1;
    }

    if (global_listener != NULL) {
        writeLog(LOG_ObjectManager_V,
            "ObjectManager::sendEvent(): Sending event to global event "
            "listener.");

        global_listener->apply(ev);
    }

    writeLog(LOG_ObjectManager_V,
        "ObjectManager::sendEvent(): Sending event "
        "to UI.");

    UiManager::sendEvent(ev);
    if (ev->shouldStopPropagation()) {
        delete ev;
        return 0;
    }


    Iterator<Listener<Object>> it(object_event_listeners->get(ev->getType()));
    while (!it.done() && !ev->shouldStopPropagation()) {
        Listener<Object> *listener = it.getNext();
        listener->apply(ev);
    }

    delete ev;
    return 0;
}

int ObjectManager::registerListener(Listener<Object> *listener, int ev_type) {
    if (ensureEventExists((char *)"registerListener", ev_type)) {
        return 1;
    }

    if (object_event_listeners->get(ev_type)->insert(listener)) {
        writeLog(LOG_ObjectManager,
            "ObjectManager::registerListener(): WARN: Object id %d already registered to event id %d",
            listener->getId(),
            ev_type);

        return 1;
    }
    
    return 0;
}

int ObjectManager::unregisterListener(const Object *obj, int ev_type) {
    if (ensureEventExists((char *)"unregisterListener", ev_type)) {
        return 1;
    }

    if (object_event_listeners->get(ev_type)->removeId(obj->getId())) {
        writeLog(LOG_ObjectManager,
            "ObjectManager::unregisterListener(): WARN: Object id %d already "
            "unregistered from event id %d",
            obj->getId(),
            ev_type);
        return 1;
    }
    
    return 0;
}

void ObjectManager::registerGlobalListener(Listener<Manager> *listener) {
    global_listener = listener;
}

void ObjectManager::clearGlobalListener() {
    delete global_listener;
    global_listener = NULL;
}

int ObjectManager::registerEvent(int ev_type) {
    Tree<Listener<Object>> *n = object_event_listeners->get(ev_type);
    if (n == NULL) {
        writeLog(LOG_ObjectManager,
            "ObjectManager::registerEvent(): Registering event type %d",
            ev_type);

        // Object event listener
        Tree<Listener<Object>> *new_tree = new Tree<Listener<Object>>();
        new_tree->setId(ev_type);
        object_event_listeners->insert(new_tree);
        return 0;
    }

    writeLog(LOG_ObjectManager,
        "ObjectManager::registerEvent(): WARNING: event id %d already "
        "registered",
        ev_type);
    return 1;
}

int ObjectManager::initObjects() {
    writeLog(LOG_ObjectManager, "ObjectManager::initObjects(): Initializing object manager");
    should_clear_game = false;

    insert_queue = new Tree<Object>();
    object_list = new Tree<Object>();
    remove_queue = new Tree<Object>();

    object_event_listeners = new Tree<Tree<Listener<Object>>>();

    return 0;
}

void ObjectManager::clearGame() {
    should_clear_game = true;
}

int ObjectManager::clearObjects() {
    writeLog(LOG_ObjectManager, "ObjectManager::clearObjects(): Clearing game state");

    Iterator<Object> it = Iterator<Object>(insert_queue);
    while (!it.done()) {
        delete it.getNext();
    }

    it = Iterator<Object>(remove_queue);
    while (!it.done()) {
        delete it.getNext();
    }

    it = Iterator<Object>(object_list);
    while (!it.done()) {
        delete it.getNext();
    }

    Iterator<Tree<Listener<Object>>> listeners_it(object_event_listeners);
    while (!listeners_it.done()) {
        Tree<Listener<Object>> *tree = listeners_it.getNext();
        Iterator<Listener<Object>> l_it(tree);
        while (!l_it.done()) {
            delete l_it.getNext();
        }

        delete tree;
    }

    insert_queue->clear();
    object_list->clear();
    remove_queue->clear();

    return 0;
}

int ObjectManager::closeObjects() {
    writeLog(LOG_ObjectManager, "ObjectManager::closeObjects(): Closing object manager");
    clearObjects();
    delete insert_queue;
    delete object_list;
    delete remove_queue;

    Iterator<Tree<Listener<Object>>> it(object_event_listeners);
    while (!it.done()) {
        delete it.getNext();
    }
    delete object_event_listeners;
    
    return 0;
}

int ObjectManager::addObject(struct Object *obj) {
    return insert_queue->insert(obj);
}

int ObjectManager::removeObject(struct Object *obj) {
    return remove_queue->insert(obj);
}

int ObjectManager::updateObjects() {
    if (should_clear_game) {
        clearObjects();
        should_clear_game = false;
    }

    object_list->merge(insert_queue);
    insert_queue->clear();

    Iterator<Object> rem_it = Iterator<Object>(remove_queue);
    while (!rem_it.done()) {
        Object *obj = rem_it.getNext();
        if (object_list->remove(obj)) {
            writeLog(LOG_ObjectManager,
                "ObjectManager::updateObjects(): Error attempting to remove object id %d",
                obj->getId());
        }
        delete obj;
    }
    remove_queue->clear();

    Iterator<Object> obj_it = Iterator<Object>(object_list);
    while (!obj_it.done()) {
        Object *obj = obj_it.getNext();
        obj->update();
    }

    return 0;
}

int ObjectManager::renderObjectLights() {
    int lights_rendered = 0;

    // Render lights
    Iterator<Object> it(object_list);
    while (!it.done()) {
        Object *obj = it.getNext();
        lights_rendered += obj->renderLight(ScreenManager::getMainScreen());
    }

    return lights_rendered;
}

int ObjectManager::renderObjects() {
    Tree<Array<Object>> *depth_tree = new Tree<Array<Object>>();
    int objects_rendered = 0;

    // Insert objects into the depth tree
    Iterator<Object> it(object_list);
    while (!it.done()) {
        Object *obj = it.getNext();
        Array<Object> *depth_node = depth_tree->get(obj->getPosition().z);
        if (depth_node == NULL) {
            depth_node = new Array<Object>();
            depth_tree->insert(depth_node);
        }
        depth_node->push(obj);
    }

    // Render objects by sorted by depth
    Iterator<Array<Object>> depth_it(depth_tree);
    while (!depth_it.done()) {
        Array<Object> *depth_node = depth_it.getNext();
        unsigned int i;
        for (i = 0; i < depth_node->getCount(); i++) {
            Object *obj = depth_node->get(i);
            objects_rendered += obj->render(ScreenManager::getMainScreen());
        }
        delete depth_node;
    }

    delete depth_tree;

    return objects_rendered;
}

int ObjectManager::getObjAt(Array<Object> *out_array, Point p, int solid) {
    Iterator<Object> it(object_list);
    while (!it.done()) {
        Object *obj = it.getNext();
        Point obj_pos = obj->getPosition();
        if ((solid && obj->getIsSolid()) || (!solid && !obj->getIsSolid())) {
            if (p.x == obj_pos.x && p.y == obj_pos.y) {
                out_array->push(obj);
            }
        }
    }

    return out_array->getCount();
}

}
