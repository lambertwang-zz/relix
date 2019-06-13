#include "parser.h"

// Library
#include <stdlib.h>

// Engine
#include "log/log.h"

namespace rlx {

JsonValue *parseValue(FILE *file, char *c_buf);

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

JsonString::JsonString(FILE *file, char *c_buf) {
    data = new String();

    if (*c_buf != '"') {
        writeLog(LOG_RESOURCE,
            "parser::parseString(): ERROR: Expected '\"' at start of string.");

        throw "Expected \" at start of string.";
    }

    while ((*c_buf = fgetc(file)) != EOF) {
        switch (*c_buf) {
            case '"':
                *c_buf = fgetc(file);
                writeLog(LOG_RESOURCE_V,
                    "parser::parseString(): Parsed string '%s'.",
                    data->getBuffer());
                return;
            default:
                data->spush(*c_buf);
                break;
        }
    }

    writeLog(LOG_RESOURCE,
        "parser::parseString(): ERROR: Reached end of file reading string "
        "'%s'.",
        data->getBuffer());

    throw "Reached end of file while parsing string.";
}

JsonInt::JsonInt(FILE *file, char *c_buf) {
    String *result = new String();

    // Only supports integers
    while (*c_buf != EOF) {
        if ((*c_buf >= '0' && *c_buf <= '9') || *c_buf == '-') {
            result->spush(*c_buf);
        } else {
            data = atoi(result->getBuffer());
            writeLog(LOG_RESOURCE_V,
                "parser::parseInt(): Parsed int string '%s' to value %d.",
                result->getBuffer(),
                data);

            delete result;
            return;
        }
        *c_buf = fgetc(file);
    }
    writeLog(LOG_RESOURCE,
        "parser::parseInt(): ERROR: Reached end of file reading int '%s'.",
        result->getBuffer());

    throw "Reached end of file while parsing int.";
}

JsonObject::JsonObject(FILE *file, char *c_buf) {
    data = new Array<JsonNode>();

    while ((*c_buf = fgetc(file)) != EOF) {
        parseWhitespace(file, c_buf);

        if (*c_buf == '}') {
            writeLog(LOG_RESOURCE_V,
                "parser::parseObject(): Parsed empty object.");

            return;
        }

        writeLog(LOG_RESOURCE_V,
            "parser::parseObject(): Attempting to parse object prop key.");

        JsonString *key = new JsonString(file, c_buf);
        if (key == NULL) {
            writeLog(LOG_RESOURCE,
                "parser::parseObject(): ERROR: Unable to parse string for object key.");
                
            throw "Failed to parse json node key.";
        }

        parseWhitespace(file, c_buf);

        if (*c_buf != ':') {
            writeLog(LOG_RESOURCE,
                "parser::parseObject(): ERROR:  Expected ':' after object "
                "key. Found '%c'.",
                *c_buf);

            throw "Expected \":\" after parsing json node key.";
        }

        *c_buf = fgetc(file);

        JsonValue *value = parseValue(file, c_buf);
        if (value == NULL) {
            writeLog(LOG_RESOURCE,
                "parser::parseObject(): ERROR: Unable to parse value for "
                "object value.");

            throw "Failed to parse json object value.";
        }

        JsonNode *node = new JsonNode(key, value);
        data->push(node);

        parseWhitespace(file, c_buf);

        switch (*c_buf) {
            case '}':
                *c_buf = fgetc(file);
                writeLog(LOG_RESOURCE_V,
                    "parser::parseObject(): Parsed object.");

                return;
            case ',':
                writeLog(LOG_RESOURCE_V,
                    "parser::parseObject(): Found ',' after obj property.");

                continue;
            default:
                writeLog(LOG_RESOURCE,
                    "parser::parseObject(): ERROR: Expected comma or end of "
                    "object. Found '%c'.",
                    *c_buf);

                throw "Expected comma while parsing Json object.";
        }
    }

    writeLog(LOG_RESOURCE,
        "parser::parseObject(): ERROR: Reached end of file reading object.");

    throw "Reached end of file while parsing object.";
}

JsonArray::JsonArray(FILE *file, char *c_buf) {
    data = new Array<JsonValue>();

    while ((*c_buf = fgetc(file)) != EOF) {
        parseWhitespace(file, c_buf);

        if (*c_buf == ']') {
            writeLog(LOG_RESOURCE_V,
                "parser::parseArray(): Parsed empty array.");

            return;
        }

        data->push(parseValue(file, c_buf));
        parseWhitespace(file, c_buf);
        switch (*c_buf) {
            case ']':
                *c_buf = fgetc(file);
                writeLog(LOG_RESOURCE_V, "parser::parseArray(): Parsed array.");
                return;
            case ',':
                continue;
            default:
                writeLog(LOG_RESOURCE_V,
                    "parser::parseArray(): ERROR: Expected comma or end of "
                    "array. Found '%c'.",
                    *c_buf);

                throw "Expected end of array or comma after array item.";
        }
    }

    writeLog(LOG_RESOURCE,
        "parser::parseArray(): ERROR: Reached end of file reading array.");

    throw "Reached end of file while parsing array.";
}

JsonValue *parseValue(FILE *file, char *c_buf) {
    parseWhitespace(file, c_buf);
    while (*c_buf != EOF) {
        switch (*c_buf) {
            case '"':
                return new JsonString(file, c_buf);
            case '[':
                return new JsonArray(file, c_buf);
            case '{':
                return new JsonObject(file, c_buf);
            default:
                if ((*c_buf >= '0' && *c_buf <= '9') || *c_buf == '-') {
                    return new JsonInt(file, c_buf);
                }
                writeLog(LOG_RESOURCE,
                    "parser::parseValue(): ERROR Unexpected character at "
                    "start of value '%c'.",
                    *c_buf);

                throw "Unexpected character at start of value.";
        }
        *c_buf = fgetc(file);
    }
    writeLog(LOG_RESOURCE,
        "parser::readValue(): ERROR: Reached end of file reading data.");

    throw "Reached end of file while parsing value.";
}

JsonValue *parseFile(FILE *file) {
    writeLog(LOG_RESOURCE_V, "parser::parseFile(): Beginning to parse file.");
    char c = fgetc(file);
    return parseValue(file, &c);
}

JsonNode *JsonNode::getObjValue(char *key) {
    JsonObject *obj = dynamic_cast<JsonObject*>(value);

    if (obj == NULL) {
        return NULL;
    }

    return obj->getItem(key);
}

}
