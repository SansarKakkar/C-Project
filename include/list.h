#ifndef LIST_H
#define LIST_H

#include "types.h"

int LPUSH(struct Entry *table[], char key[], char value[]);
int RPUSH(struct Entry *table[], char key[], char value[]);
int LPOP(struct Entry *table[], char key[]);
int RPOP(struct Entry *table[], char key[]);
void LRANGE(struct Entry *table[], char key[]);

#endif
