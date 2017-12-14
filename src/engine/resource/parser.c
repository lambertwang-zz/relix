#include "parser.h"

// Library
#include <stdlib.h>
#include <string.h>

// Engine
#include "log/log.h"

JsonNode *parseValue(FILE *file, char *c_buf);

JsonNode *createJsonNode() {
    JsonNode *new_node = malloc(sizeof(JsonNode));
    new_node->type = -1;
    new_node->data = NULL;

    return new_node;
}

int closeJsonNode(JsonNode *node) {
    if (node == NULL) {
        return 1;
    }

    // writeLog(LOG_RESOURCE_V, "parser::closeJsonNode(): Closing Node.");
    unsigned int i;
    if (node->data != NULL) {
        JsonObjData *obj_data = node->data;
        Array *array_data = node->data;
        switch (node->type) {
            case JSON_OBJ:
                for (i = 0; i < obj_data->props.count; i++) {
                    JsonObjProp *obj_prop = getDataArray(&obj_data->props, i);
                    if (obj_prop->value != NULL) {
                        closeJsonNode(obj_prop->value);
                    }
                    free(obj_prop);
                }
                closeArray(&obj_data->props);
                free(node->data);
                break;
            case JSON_ARRAY:
                for (i = 0; i < array_data->count; i++) {
                    JsonNode *array_item = getDataArray(array_data, i);
                    closeJsonNode(array_item);
                }
                closeArray(array_data);
                free(node->data);
                break;
            case JSON_STRING:
                deleteString(node->data);
                break;
            default:
                free(node->data);
                break;
        }
    }
    free(node);
    return 0;
}

int parseWhitespace(FILE *file, char *c_buf) {
    while (*c_buf != EOF) {
        switch (*c_buf) {
            case ' ':
            case '\t':
            case '\n':
                *c_buf = fgetc(file);
                continue;
            default:
                return 0;
        }
    }
    return 1;
}

JsonNode *parseString(FILE *file, char *c_buf) {
    // writeLog(LOG_RESOURCE_V, "parser::parseString(): Beginning to parse string.");
    JsonNode *new_node = createJsonNode(); 
    String *result = createString();

    new_node->data = result;
    new_node->type = JSON_STRING;

    if (*c_buf != '"') {
        writeLog(LOG_RESOURCE, "parser::parseString(): ERROR: Expected '\"' at start of string.");
        deleteString(result);
        return NULL;
    }

    while ((*c_buf = fgetc(file)) != EOF) {
        switch (*c_buf) {
            case '"':
                *c_buf = fgetc(file);
                writeLog(LOG_RESOURCE_V, "parser::parseString(): Parsed string '%s'.", result->s);
                return new_node;
            default:
                spush(result, *c_buf);
                break;
        }
    }
    writeLog(LOG_RESOURCE, "parser::parseString(): ERROR: Reached end of file reading string '%s'.", result->s);
    closeJsonNode(new_node);
    return NULL;
}

JsonNode *parseInt(FILE *file, char *c_buf) {
    // writeLog(LOG_RESOURCE_V, "parser::parseInt(): Beginning to parse int.");
    JsonNode *new_node = createJsonNode(); 
    int *value = malloc(sizeof(int));

    String *result = createString();
    spush(result, *c_buf);

    new_node->data = value;
    new_node->type = JSON_INT;

    // Only supports integers
    while (*c_buf != EOF) {
        if ((*c_buf >= '0' && *c_buf <= '9') || *c_buf == '-') {
            spush(result, *c_buf);
        } else {
            *value = atoi(result->s);
            writeLog(LOG_RESOURCE_V, "parser::parseInt(): Parsed int string '%s' to value %d.", result->s, *value);
            deleteString(result);
            return new_node;
        }
        *c_buf = fgetc(file);
    }
    writeLog(LOG_RESOURCE, "parser::parseInt(): ERROR: Reached end of file reading int '%s'.", result->s);
    closeJsonNode(new_node);
    deleteString(result);
    return NULL;
}

JsonNode *parseObject(FILE *file, char *c_buf) {
    // writeLog(LOG_RESOURCE_V, "parser::parseObject(): Beginning to parse object.");
    JsonNode *new_node = createJsonNode(); 
    JsonObjData *result = malloc(sizeof(JsonObjData));
    initArray(&result->props);

    new_node->data = result;
    new_node->type = JSON_OBJ;

    while ((*c_buf = fgetc(file)) != EOF) {
        parseWhitespace(file, c_buf);

        if (*c_buf == '}') {
            writeLog(LOG_RESOURCE_V, "parser::parseObject(): Parsed empty object.");
            return new_node;
        }

        JsonObjProp *prop = malloc(sizeof(JsonObjProp));
        push(&result->props, prop);

        JsonNode *keyNode = parseString(file, c_buf);
        if (keyNode == NULL) {
            writeLog(LOG_RESOURCE, "parser::parseObject(): ERROR: Unable to parse string for object key.");
            closeJsonNode(new_node);
            return NULL;
        }
        stringCopy(prop->key, keyNode->data);
        closeJsonNode(keyNode);
        // writeLog(LOG_RESOURCE_V, "parser::parseObject(): Parsed key '%s'.", prop->key);

        parseWhitespace(file, c_buf);

        if (*c_buf != ':') {
            writeLog(LOG_RESOURCE, "parser::parseObject(): ERROR:  Expected ':' after object key. Found '%c'.", *c_buf);
            closeJsonNode(new_node);
            return NULL;
        }
        // writeLog(LOG_RESOURCE_V, "parser::parseObject(): Found ':' after object key.");
        *c_buf = fgetc(file);

        JsonNode *valueNode = parseValue(file, c_buf);
        if (valueNode == NULL) {
            writeLog(LOG_RESOURCE, "parser::parseObject(): ERROR: Unable to parse value for object value.");
            closeJsonNode(new_node);
            return NULL;
        }

        prop->value = valueNode;

        parseWhitespace(file, c_buf);

        switch (*c_buf) {
            case '}':
                *c_buf = fgetc(file);
                writeLog(LOG_RESOURCE_V, "parser::parseObject(): Parsed object.");
                return new_node;
            case ',':
                continue;
            default:
                writeLog(LOG_RESOURCE_V, "parser::parseObject(): ERROR: Expected comma or end of object. Found '%c'.", *c_buf);
                closeJsonNode(new_node);
                return NULL;
        }
    }

    writeLog(LOG_RESOURCE, "parser::parseObject(): ERROR: Reached end of file reading object.");
    closeJsonNode(new_node);
    return NULL;
}

JsonNode *parseArray(FILE *file, char *c_buf) {
    // writeLog(LOG_RESOURCE_V, "parser::parseArray(): Beginning to parse array.");
    JsonNode *new_node = createJsonNode(); 
    Array *result = malloc(sizeof(Array));
    initArray(result);

    new_node->data = result;
    new_node->type = JSON_ARRAY;

    while ((*c_buf = fgetc(file)) != EOF) {
        parseWhitespace(file, c_buf);

        if (*c_buf == ']') {
            writeLog(LOG_RESOURCE_V, "parser::parseArray(): Parsed empty array.");
            return new_node;
        }


        JsonNode *itemNode = parseValue(file, c_buf);
        if (itemNode == NULL) {
            writeLog(LOG_RESOURCE, "parser::parseArray(): ERROR: Unable to parse value for array item.");
            closeJsonNode(new_node);
            return NULL;
        }
        push(result, itemNode);

        parseWhitespace(file, c_buf);

        switch (*c_buf) {
            case ']':
                *c_buf = fgetc(file);
                writeLog(LOG_RESOURCE_V, "parser::parseArray(): Parsed array.");
                return new_node;
            case ',':
                continue;
            default:
                writeLog(LOG_RESOURCE_V, "parser::parseArray(): ERROR: Expected comma or end of array. Found '%c'.", *c_buf);
                closeJsonNode(new_node);
                return NULL;
        }
    }

    writeLog(LOG_RESOURCE, "parser::parseArray(): ERROR: Reached end of file reading array.");
    closeJsonNode(new_node);
    return NULL;
}

JsonNode *parseValue(FILE *file, char *c_buf) {
    parseWhitespace(file, c_buf);
    while (*c_buf != EOF) {
        switch (*c_buf) {
            case '"':
                return parseString(file, c_buf);
            case '[':
                return parseArray(file, c_buf);
            case '{':
                return parseObject(file, c_buf);
            default:
                if ((*c_buf >= '0' && *c_buf <= '9') || *c_buf == '-') {
                    // *data = readInt(file, c_buf);
                    return parseInt(file, c_buf);
                }
                writeLog(LOG_RESOURCE, "parser::parseValue(): ERROR Unexpected character at start of value '%c'.", *c_buf);
                return NULL;
        }
        *c_buf = fgetc(file);
    }
    writeLog(LOG_RESOURCE, "parser::readValue(): ERROR: Reached end of file reading data.");
    return NULL;
}

JsonNode *parseFile(FILE *file) {
    writeLog(LOG_RESOURCE_V, "parser::parseFile(): Beginning to parse file.");
    char c = fgetc(file);
    return parseValue(file, &c);
}

JsonNode *getObjValue(JsonNode *node, char *key) {
    writeLog(LOG_RESOURCE_V, "parser::getObjValue(): ERROR Cannot get value from non-object.");
    if (node->type != JSON_OBJ) {
        return NULL;
    }

    unsigned int i;
    JsonObjData *obj_data = node->data;
    for (i = 0; i < obj_data->props.count; i++) {
        JsonObjProp *obj_prop = getDataArray(&obj_data->props, i);
        if (!strcmp(obj_prop->key->s, key)) {
            return obj_prop->value;
        }
    }

    writeLog(LOG_RESOURCE_V, "parser::getObjValue(): ERROR Object has no key '%s'.", key);
    return NULL;
}


