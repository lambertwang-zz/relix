#ifndef __OBJECT_MANAGER_H__
#define __OBJECT_MANAGER_H__

// Engine
#include "object.h"
#include "manager/manager.h"

namespace rlx {
    class ObjectManager: public Manager {
    private:
        // TODO: Enable storing and retrieving game states
        static Tree<Object> *insert_queue;
        static Tree<Object> *object_list;
        static Tree<Object> *remove_queue; 

        static Tree<Tree<Listener<Object>>> *object_event_listeners;

        static Listener<Manager> *global_listener;
        static bool should_clear_game;

        static int ensureEventExists(char *function, int ev_type);
        static int clearObjects();
    public:
        static int registerEvent(int ev_id);
        static int sendEvent(Event *ev);

        // Register event listeners for specific objects
        static int registerListener(Listener<Object> *listener, int ev_type);
        static int unregisterListener(const Object *obj, int ev_type);
        
        // Register the global event listener.
        // Only one global event listener may be set.
        // The global event listener listens to all events.
        static void registerGlobalListener(Listener<Manager> *listener);
        static void clearGlobalListener();
        
        static int initObjects();
        static void clearGame();
        static int closeObjects();
        static void queueClear(void (next)());
        
        static int addObject(Object *obj);
        static int removeObject(Object *obj);
        
        static int updateObjects();
        static int renderObjectLights();
        static int renderObjects();
        
        // Exhaustive search
        static int getObjAt(Array<Object> *out_array, Point p, int solid);
    };
}


#endif

