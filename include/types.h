#ifndef TYPES_H
#define TYPES_H

#include <time.h>

#define TABLE_SIZE 10

struct ListNode {
    char value[100];
    struct ListNode *next;
};

struct SetNode {
    char value[100];
    struct SetNode *next;
};

struct HashNode {
    char field[50];
    char value[100];
    struct HashNode *next;
};

enum DataType {
    TYPE_STRING,
    TYPE_LIST,
    TYPE_SET,
    TYPE_HASH
};

struct Entry {
    char key[50];
    enum DataType type;
    void *value;
    time_t expiry;
    struct Entry *next;
};

struct Transaction {
    int active;
    struct Entry *snapshot[TABLE_SIZE];
};

#endif
