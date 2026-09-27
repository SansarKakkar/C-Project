#ifndef STORE_H
#define STORE_H

#include "types.h"

int isExpired(struct Entry *entry);
unsigned long hashFunction(char key[]);
int getIndex(char key[]);
int insertEntry(struct Entry *table[], char key[], char value[]);
void freeEntry(struct Entry *entry);
void freeTable(struct Entry *table[]);
int deleteEntry(struct Entry *table[], char key[]);
void getEntry(struct Entry *table[], char key[]);
void EXPIRE(struct Entry *table[], char key[], int seconds);
int TTL(struct Entry *table[], char key[]);

#endif
