#ifndef HASH_H
#define HASH_H

#include "types.h"

int HSET(struct Entry *table[], char key[], char field[], char value[]);
void HGET(struct Entry *table[], char key[], char field[]);
int HDEL(struct Entry *table[], char key[], char field[]);
void HGETALL(struct Entry *table[], char key[]);

#endif
