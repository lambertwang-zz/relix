#ifndef __STRING_H__
#define __STRING_H__

#define INIT_STR_SIZE 8

namespace rlx {
    class String {
    private:
        char *buffer;
        int size;
        int len;

        // Double the size of the string
        int incrementSize();

    public:
        String();
        ~String();

        char *getBuffer() const {
            return buffer;
        }

        void clear();

        int compare(const String *other) const;
        char *copy(const String *src);

        int sputf(const char *format, ...);
        int spush(const char c);
        int sgetc(String *dest, const int index) const;

        int print() const;
    };

    int stringCompare(const String *a, const String *b);
}

#endif

