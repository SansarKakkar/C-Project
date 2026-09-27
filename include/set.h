#ifndef SET_H
#define SET_H

#include "types.h"

int SADD(struct Entry *table[], char key[], char value[]);
int SREM(struct Entry *table[], char key[], char value[]);
void SISMEMBER(struct Entry *table[], char key[], char value[]);
void SMEMBERS(struct Entry *table[], char key[]);

#endif
