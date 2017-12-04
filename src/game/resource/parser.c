#include "parser.h"

// Library
#include <stdlib.h>
#include <string.h>

// Engine
#include "log/log.h"

// Game
#include "../relix.h"

JsonNode *parseValue(FILE *file, char *c_buf);

JsonNode *createJsonNode() {
    JsonNode *new_node = malloc(sizeof(JsonNode));
    new_node->type = -1;
    new_node->data = NULL;

    return new_node;
}

void closeJsonNode(JsonNode *node) {
    writeLog(LOG_LOAD_V, "parser::closeJsonNode(): Closing Node.");
    int i;
    if (node->data != NULL) {
        if (node->type == JSON_OBJ) {
            JsonObjData *obj_data = node->data;
            for (i = 0; i < obj_data->props.count; i++) {
                JsonObjProp *obj_prop = getDataArray(&obj_data->props, i);
                closeJsonNode(obj_prop->value);
                free(obj_prop);
            }
            closeArray(&obj_data->props);
        }
        free(node->data);
    }
    free(node);
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
    writeLog(LOG_LOAD_V, "parser::parseString(): Beginning to parse string.");
    JsonNode *new_node = createJsonNode(); 
    char *result = malloc(sizeof(char) * LABEL_LONG);
    char *head = result;

    new_node->data = result;
    new_node->type = JSON_STRING;

    if (*c_buf != '"') {
        writeLog(LOG_LOAD, "parser::parseString(): ERROR: Expected '\"' at start of string.");
        free(result);
        return NULL;
    }

    while ((*c_buf = fgetc(file)) != EOF) {
        switch (*c_buf) {
            case '"':
                *head = '\0';
                *c_buf = fgetc(file);
                writeLog(LOG_LOAD_V, "parser::parseString(): Parsed string '%s'.", result);
                return new_node;
            default:
                *head++ = *c_buf;
                if (head - LABEL_LONG >= result) {
                    writeLog(LOG_LOAD, "parser::parseString(): ERROR: String too long '%s'.", result);
                    closeJsonNode(new_node);
                    return NULL;
                }
                break;
        }
    }
    writeLog(LOG_LOAD, "parser::parseString(): ERROR: Reached end of file reading string '%s'.", result);
    closeJsonNode(new_node);
    return NULL;
}

JsonNode *parseInt(FILE *file, char *c_buf) {
    writeLog(LOG_LOAD_V, "parser::parseInt(): Beginning to parse int.");
    JsonNode *new_node = createJsonNode(); 
    int *value = malloc(sizeof(int));

    char result[LABEL_SHORT];
    *result = *c_buf;
    char *head = result;

    new_node->data = value;
    new_node->type = JSON_INT;

    // Just reading ints right now
    while (*c_buf != EOF) {
        if ((*c_buf >= '0' && *c_buf <= '9') || *c_buf == '-') {
            *head++ = *c_buf;
            if (head - LABEL_SHORT >= result - 1) {
                writeLog(LOG_LOAD, "parser::parseInt(): ERROR: Int too long '%s'.", result);
                closeJsonNode(new_node);
                return NULL;
            }
        } else {
            *head = '\0';
            *value = atoi(result);
            writeLog(LOG_LOAD_V, "parser::parseInt(): Parsed int string '%s' to value %d.", result, *value);
            return new_node;
        }
        *c_buf = fgetc(file);
    }
    writeLog(LOG_LOAD, "parser::parseInt(): ERROR: Reached end of file reading int '%s'.", result);
    closeJsonNode(new_node);
    return NULL;
}

JsonNode *parseObject(FILE *file, char *c_buf) {
    writeLog(LOG_LOAD_V, "parser::parseObject(): Beginning to parse object.");
    JsonNode *new_node = createJsonNode(); 
    JsonObjData *result = malloc(sizeof(JsonObjData));
    initArray(&result->props);

    new_node->data = result;
    new_node->type = JSON_OBJ;

    while ((*c_buf = fgetc(file)) != EOF) {
        parseWhitespace(file, c_buf);

        if (*c_buf == '}') {
            writeLog(LOG_LOAD_V, "parser::parseObject(): Parsed empty object.");
            return new_node;
        }

        JsonObjProp *prop = malloc(sizeof(JsonObjProp));

        JsonNode *keyNode = parseString(file, c_buf);
        if (keyNode == NULL) {
            writeLog(LOG_LOAD, "parser::parseObject(): ERROR: Unable to parse string for object key.");
            closeJsonNode(new_node);
            free(prop);
            return NULL;
        }
        strcpy(prop->key, keyNode->data);
        closeJsonNode(keyNode);
        writeLog(LOG_LOAD_V, "parser::parseObject(): Parsed key '%s'.", prop->key);

        parseWhitespace(file, c_buf);

        if (*c_buf != ':') {
            writeLog(LOG_LOAD, "parser::parseObject(): ERROR:  Expected ':' after object key. Found '%c'.", *c_buf);
            closeJsonNode(new_node);
            free(prop);
            return NULL;
        }
        writeLog(LOG_LOAD_V, "parser::parseObject(): Found ':' after object key.");
        *c_buf = fgetc(file);

        JsonNode *valueNode = parseValue(file, c_buf);
        if (valueNode == NULL) {
            writeLog(LOG_LOAD, "parser::parseObject(): ERROR: Unable to parse value for object value.");
            closeJsonNode(new_node);
            free(prop);
            return NULL;
        }

        prop->value = valueNode;

        push(&result->props, prop);

        parseWhitespace(file, c_buf);

        switch (*c_buf) {
            case '}':
                writeLog(LOG_LOAD_V, "parser::parseObject(): Parsed object.");
                return new_node;
            case ',':
                continue;
            default:
                writeLog(LOG_LOAD_V, "parser::parseObject(): ERROR: Expected comma or end of object. Found '%c'.", *c_buf);
                closeJsonNode(new_node);
                free(prop);
                return NULL;
        }
    }

    writeLog(LOG_LOAD, "parser::readString(): ERROR: Reached end of file reading string '%s'.", result);
    free(result);
    return NULL;
}

JsonNode *parseValue(FILE *file, char *c_buf) {
    parseWhitespace(file, c_buf);
    while (*c_buf != EOF) {
        switch (*c_buf) {
            case '"':
                return parseString(file, c_buf);
            case '[':
                // return parseArray(file, c_buf);
            case '{':
                return parseObject(file, c_buf);
            case ']':
                writeLog(LOG_LOAD_V, "parser::readData(): Reached end of array.");
            default:
                if ((*c_buf >= '0' && *c_buf <= '9') || *c_buf == '-') {
                    // *data = readInt(file, c_buf);
                    return parseInt(file, c_buf);
                }
                writeLog(LOG_LOAD, "parser::parseValue(): ERROR Unexpected character at start of value '%c'.", *c_buf);
                return NULL;
        }
        *c_buf = fgetc(file);
    }
    writeLog(LOG_LOAD, "parser::readData(): ERROR: Reached end of file reading data.");
    return NULL;
}

JsonNode *parseFile(FILE *file) {
    writeLog(LOG_LOAD_V, "parser::parseFile(): Beginning to parse file.");
    char c = fgetc(file);
    return parseValue(file, &c);
}
