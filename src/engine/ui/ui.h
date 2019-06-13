#ifndef __UI_H__
#define __UI_H__

// Engine
#include "event/event.h"
#include "constants.h"
#include "list/tree.h"
#include "term/screen.h"
#include "string/string.h"
#include "input/input.h"
#include "manager/manager.h"


namespace rlx {
    enum UiPositioning {
        // Positioned to top-left of parent
        PosStatic = 0,
        // Position in pixels
        PosAbsolute,
        // Position in % of parent
        PosRelative
    };

    enum UiSizing {
        SizeNone = 0,
        // Absolute size in pixels
        SizeAbsolute,
        // Size relative to parent in percentage
        SizeRelative,
        // Match size to parent
        SizeParent

    };

    class Element: public Unique {
    private:
        // Unique label for the element
        String *tag;

        // Text content of the element
        String *text;

        // Styling
        UiPositioning positioning;
        UiSizing sizing;

        int width;
        int height;

        // Top left coordinates for the element
        Point pos;

        // Function callbacks
        int (*onRender)(struct Element *, Screen *);
        int (*onEvent)(struct Element *, Event *);
        void (*onDelete)(struct Element *);
        Tree<Listener<Element>> event_listeners;

        Element *parent;
        Tree<Element> children;
        Rect bounds;
    protected:
        bool focusable;

        // Text and background color
        Color text_c;
        Color bg_c;
        Color bg_c_focus;
    public:
        Element();

        void setText(String *to_copy);

        virtual int onClick();
    };

    class UiManager: public Manager {
    private:
        Tree<Element> *ui_tree;
        Tree<Tree<Listener<Element>>> *event_listeners;
    public:
        static int initUi();
        static int closeUi();
        static int renderUi();

        // Ui Event functions
        static int registerUiListener(const Element *el, int ev_id);
        static int unregisterUiListener(const Element *el, int ev_id);
        static int sendEvent(Event *ev);

        // Element Functions
        static int onRenderDefaultElement(Element *e, Screen *s);
        static int deleteElement(Element *elem);
        static int listenUiEvent(Element *el, int ev_id, int (*listener)(Element *, Event *));

        static void setFocus(Element *elem);

        static int registerUiElement(Element *e);

        static Element *getFocus();
    };
}

#endif
