#ifndef __PARSER_H__
#define __PARSER_H__

// Library
#include <stdio.h>
#include <string.h>

// Engine
#include "constants.h"
#include "list/array.h"
#include "string/string.h"

namespace rlx {
    class JsonNode;

    class JsonValue {
    protected:
        JsonValue();
    public:
        virtual ~JsonValue();
    };

    class JsonNull: public JsonValue {
    public:
        JsonNull() { }
    };

    class JsonString: public JsonValue {
    private:
        String *data;
    public:
        JsonString(FILE *file, char *c_buf);
        ~JsonString() {
            delete data;
        }

        char *getString() { return data->getBuffer(); }
    };

    class JsonInt: public JsonValue {
    private:
        int data;
    public:
        JsonInt(FILE *file, char *c_buf);
        ~JsonInt() { }
    };

    class JsonNode {
    private:
        JsonString *key;
        JsonValue *value;

        JsonNode();
    public:
        JsonNode(JsonString *initial_key, JsonValue *initial_value) {
            key = initial_key;
            value = initial_value;
        }

        ~JsonNode() {
            delete key;
            delete value;
        }

        JsonNode *getObjValue(char *key);

        JsonString *getKey() { return key; }
        JsonValue *getValue() { return value; }
    };

    class JsonObject: public JsonValue {
    private:
        Array<JsonNode> *data;
    public:
        JsonObject(FILE *file, char *c_buf);

        ~JsonObject() {
            unsigned int i;
            for (i = 0; i < data->getCount(); i++) {
                delete data->get(i);
            }
            delete data;
        }

        JsonNode *getItem(char *key) {
            unsigned int i;
            for (i = 0; i < data->getCount(); i++) {
                JsonNode *node= data->get(i);
                if (!strcmp(node->getKey()->getString(), key)) {
                    return node;
                }
            }

            return NULL;
        }
    };

    class JsonArray: public JsonValue {
    private:
        Array<JsonValue> *data;
    public:
        JsonArray(FILE *file, char *c_buf);

        ~JsonArray() {
            unsigned int i;
            for (i = 0; i < data->getCount(); i++) {
                delete data->get(i);
            }
            delete data;
        }
    };
}

#endif

