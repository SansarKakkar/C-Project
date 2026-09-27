#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "types.h"

void BEGIN(struct Entry *table[], struct Transaction *transaction);
void COMMIT(struct Transaction *transaction);
void ROLLBACK(struct Entry *table[], struct Transaction *transaction);

#endif
