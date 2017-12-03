#include "parser.h"

// Library
#include <stdlib.h>
#include <string.h>

// Engine
#include "log/log.h"

// Game
#include "../relix.h"

RNode *parseObject(FILE *file);
int readData(void **data, FILE *file);

RNode *createRNode() {
    RNode *new = malloc(sizeof(RNode));

    initArray(&new->values);

    return new;
}

int closeNodeVal(NodeVal *val) {
    int i;
    if (val->data == NULL) {
        writeLog(LOG_LOAD, "parser::closeNodeVal(): ERROR: Attempting to close node value NULL.");
        free(val);
        return -1;
    }
    switch (val->type) {
        case NODE_NULL:
            writeLog(LOG_LOAD, "parser::closeNodeVal(): ERROR: Attempting to close node type NULL.");
            break;
        case NODE_OBJ:
            writeLog(LOG_LOAD_V, "parser::closeNodeVal(): Closing Obj.");
            closeRNode(val->data);
            break;
        case NODE_ARRAY:
            writeLog(LOG_LOAD_V, "parser::closeNodeVal(): Closing Array with %d elements.", ((Array *)val->data)->count);
            for (i = 0; i < ((Array *)val->data)->count; i++) {
                closeNodeVal(getDataArray((Array *)val->data, i));
            }
            closeArray((Array *)val->data);
            free(val->data);
            break;
        default:
            writeLog(LOG_LOAD_V, "parser::closeNodeVal(): Closing primitive.");
            free(val->data);
            break;
    }
    free(val);
    return 0;
}

void closeRNode(RNode *node) {
    writeLog(LOG_LOAD_V, "parser::closeRNode(): Closing Node.");
    int i;
    for (i = 0; i < node->values.count; i++) {
        NodeVal *node_val = getDataArray(&node->values, i);
        closeNodeVal(node_val);
    }
    closeArray(&node->values);
    free(node);
}

int pushNodeValue(char label[LABEL_MED], int type, void *data, RNode *node) {
    if (data == NULL) {
        writeLog(LOG_LOAD, "parser::pushNodeValue(): ERROR: Attempting to push NULL data to node.");
        return -1;
    }

    NodeVal *new_val = malloc(sizeof(NodeVal));

    new_val->type = type;
    strcpy(new_val->label, label);

    new_val->data = data;
    push(&node->values, new_val);
    return 0;
}

int readLabel(char *label, FILE *file, char *c_buf) {
    char *head = label;
    *head++ = *c_buf;
    while ((*c_buf = fgetc(file)) != EOF) {
        switch (*c_buf) {
            case ':':
                *head = '\0';
                return 0;
            default:
                *head++ = *c_buf;
                break;
        }
    }
    *head = '\0';
    return -1;
}


char *readString(FILE *file) {
    char *result = malloc(sizeof(char) * LABEL_LONG);
    char *head = result;
    char c;

    while ((c = fgetc(file)) != EOF) {
        switch (c) {
            case '"':
                writeLog(LOG_LOAD_V, "parser::readString(): Read string string '%s'.", result);
                return result;
            default:
                *head++ = c;
                if (head - LABEL_LONG >= result) {
                    writeLog(LOG_LOAD, "parser::readString(): ERROR: String too long '%s'.", result);
                    return result;
                }
                break;
        }
    }
    writeLog(LOG_LOAD, "parser::readString(): ERROR: Reached end of file reading string '%s'.", result);
    return result;
}

void *readArray(FILE *file) {
    writeLog(LOG_LOAD_V, "parser::readArray(): Reading array.");
    Array *new_array = malloc(sizeof(Array));
    initArray(new_array);
    void *data_tmp = NULL;

    int data_type = readData(&data_tmp, file);

    while (data_type != NODE_NULL) {
        NodeVal *new_val = malloc(sizeof(NodeVal));
        new_val->type = data_type;
        new_val->data = data_tmp;
        strcpy(new_val->label, "\0");

        writeLog(LOG_LOAD_V, "parser::readArray(): Found array element of type %d.", data_type);

        push(new_array, new_val);
        data_type = readData(&data_tmp, file);
    }
    writeLog(LOG_LOAD_V, "parser::readArray(): Parsed array with %d elements.", new_array->count);

    return new_array;
}

void *readInt(FILE *file, char *c_buf) {
    char *result = malloc(sizeof(char) * LABEL_SHORT);
    *result = *c_buf;
    char *head = result;
    int *value = malloc(sizeof(int));

    while ((*c_buf = fgetc(file)) != EOF) {
        switch (*c_buf) {
            case ' ':
            case '\t':
            case '\n':
                *value = atoi(result);
                writeLog(LOG_LOAD_V, "parser::readInt(): Parsed int string '%s' to value '%d'.", result, *value);
                free(result);
                return value;
            default:
                *head++ = *c_buf;
                if (head - LABEL_SHORT>= result) {
                    writeLog(LOG_LOAD, "parser::readInt(): ERROR: Int too long '%s'.", result);
                    return result;
                }
                break;
        }
    }
    writeLog(LOG_LOAD, "parser::readString(): ERROR: Reached end of file reading string '%s'.", result);
    return result;
}

int readData(void **data, FILE *file) {
    char c;
    while ((c = fgetc(file)) != EOF) {
        switch (c) {
            case ' ':
            case '\t':
            case '\n':
                continue;
            case '"':
                *data = readString(file);
                return NODE_STRING;
            case '[':
                *data = readArray(file);
                return NODE_ARRAY;
            case '{':
                *data = parseObject(file);
                return NODE_OBJ;
            case ']':
                writeLog(LOG_LOAD_V, "parser::readData(): Reached end of array.");
                return NODE_NULL;
            default:
                if ((c >= '0' && c <= '9') || c == '-' || c == '+') {
                    *data = readInt(file, &c);
                    return NODE_INT;
                }
                break;
        }
    }
    writeLog(LOG_LOAD, "parser::readData(): ERROR: Reached end of file reading data.");
    return NODE_NULL;
}

RNode *parseValues(FILE *file, RNode *node) {
    char c;
    char label_tmp[LABEL_MED];
    int data_type = NODE_NULL;
    void *data_tmp = NULL;

    while ((c = fgetc(file)) != EOF) {
        switch (c) {
            case ' ':
            case '\t':
            case '\n':
                continue;
            case '}':
                writeLog(LOG_LOAD_V, "parser::parseValues(): Reached end of object.");
                return node;
            default:
                readLabel(label_tmp, file, &c);
                writeLog(LOG_LOAD_V, "parser::parseValues(): Found value with label '%s'.", label_tmp);
                data_type = readData(&data_tmp, file);
                pushNodeValue(label_tmp, data_type, data_tmp, node);
        }
    }
    writeLog(LOG_LOAD, "parser::parseValues(): ERROR: Reached end of file while parsing object.");
    return node;
}

RNode *parseObject(FILE *file) {
    writeLog(LOG_LOAD_V, "parser::parseObject(): Parsing Object.");
    RNode *result = createRNode();
    writeLog(LOG_LOAD_V, "parser::parseObject(): Created new RNode.");
    return parseValues(file, result);
}

RNode *parseFile(FILE *file) {
    writeLog(LOG_LOAD_V, "parser::parseFile(): Beginning to parse file.");
    char c;

    while ((c = fgetc(file)) != EOF) {
        switch (c) {
            case ' ':
            case '\t':
            case '\n':
                continue;
            case '{':
                return parseObject(file);
            default:
                writeLog(LOG_LOAD, "parser::parseFile(): ERROR: Found value before beginning of object.");
                return NULL;
        }
    }
    writeLog(LOG_LOAD, "parser::parseFile(): ERROR: Unable to find object in file.");
    return NULL;
}
