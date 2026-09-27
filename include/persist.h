#ifndef PERSIST_H
#define PERSIST_H

#include "types.h"

void SaveCommands(char *command);
void LoadDatabase(struct Entry *table[]);

#endif
