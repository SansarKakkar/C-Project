#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "transaction.h"
#include "store.h"

static struct Entry *copyEntry(struct Entry *original)
{
    if (original == NULL)
    {
        return NULL;
    }

    struct Entry *newEntry = malloc(sizeof(struct Entry));

    if (newEntry == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }

    strcpy(newEntry -> key, original -> key);
    newEntry -> type = original->type;
    newEntry -> next = NULL;
    newEntry->expiry = original->expiry;
    if (original -> type == TYPE_STRING)
    {
        newEntry->value = malloc(strlen((char *)original->value) + 1);

        if (newEntry->value == NULL)
        {
            printf("Memory allocation failed\n");
            free(newEntry);
            return NULL;
        }

        strcpy((char *)newEntry->value,(char *)original->value);
    }

    else if (original -> type == TYPE_LIST)
    {
        struct ListNode *originalNode =(struct ListNode *)original->value;
        struct ListNode *newHead = NULL;
        struct ListNode *newTail = NULL;

        while (originalNode != NULL)
        {
            struct ListNode *newNode =malloc(sizeof(struct ListNode));

            if (newNode == NULL)
            {
                printf("Memory allocation failed\n");

                struct ListNode *temp = newHead;

                while (temp != NULL)
                {
                    struct ListNode *next = temp->next;
                    free(temp);
                    temp = next;
                }
                free(newEntry);
                return NULL;
            }

            strcpy(newNode -> value, originalNode -> value);
            newNode->next = NULL;

            if (newHead == NULL)
            {
                newHead = newNode;
                newTail = newNode;
            }
            else
            {
                newTail -> next = newNode;
                newTail = newNode;
            }
            originalNode = originalNode->next;
        }
        newEntry->value = newHead;
    }

    else if (original -> type == TYPE_SET)
    {
        struct SetNode *originalNode =(struct SetNode *)original->value;
        struct SetNode *newHead = NULL;
        struct SetNode *newTail = NULL;

        while (originalNode != NULL)
        {
            struct SetNode *newNode =malloc(sizeof(struct SetNode));

            if (newNode == NULL)
            {
                printf("Memory allocation failed\n");
                struct SetNode *temp = newHead;

                while (temp != NULL)
                {
                    struct SetNode *next = temp->next;
                    free(temp);
                    temp = next;
                }
                free(newEntry);
                return NULL;
            }
            strcpy(newNode -> value, originalNode -> value);
            newNode -> next = NULL;
            if (newHead == NULL)
            {
                newHead = newNode;
                newTail = newNode;
            }
            else
            {
                newTail -> next = newNode;
                newTail = newNode;
            }

            originalNode = originalNode->next;
        }

        newEntry -> value = newHead;
    }

    else if (original -> type == TYPE_HASH)
    {
        struct HashNode *originalNode = (struct HashNode *)original -> value;
        struct HashNode *newHead = NULL;
        struct HashNode *newTail = NULL;

        while (originalNode != NULL)
        {
            struct HashNode *newNode = malloc(sizeof(struct HashNode));

            if (newNode == NULL)
            {
                printf("Memory allocation failed\n");
                struct HashNode *temp = newHead;

                while (temp != NULL)
                {
                    struct HashNode *next = temp->next;
                    free(temp);
                    temp = next;
                }
                free(newEntry);
                return NULL;
            }
            strcpy(newNode -> field, originalNode -> field);
            strcpy(newNode -> value, originalNode -> value);
            newNode -> next = NULL;

            if (newHead == NULL)
            {
                newHead = newNode;
                newTail = newNode;
            }

            else
            {
                newTail->next = newNode;
                newTail = newNode;
            }
            originalNode = originalNode->next;
        }
        newEntry->value = newHead;
    }
    return newEntry;
}

static int copyTable(struct Entry *table[], struct Entry *snapshot[])
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        snapshot[i] = NULL;

        struct Entry *current = table[i];
        struct Entry *prevCopy = NULL;

        while (current != NULL)
        {
            struct Entry *newEntry = copyEntry(current);

            if (newEntry == NULL)
            {
                return -1;
            }

            if (snapshot[i] == NULL)
            {
                snapshot[i] = newEntry;
            }

            else
            {
                prevCopy -> next = newEntry;
            }

            prevCopy = newEntry;
            current = current -> next;
        }
    }

    return 1;
}

void BEGIN(struct Entry *table[], struct Transaction *transaction)
{
    if (transaction -> active)
    {
        printf("Transaction already active\n");
        return;
    }
    int result = copyTable(table, transaction -> snapshot);

    if (result == -1)
    {
        printf("Could not start transaction\n");
        return;
    }
    transaction -> active = 1;
    printf("Transaction started\n");
}

void COMMIT(struct Transaction *transaction)
{
    if (!transaction -> active)
    {
        printf("No active transaction\n");
        return;
    }

    freeTable(transaction -> snapshot);

    transaction -> active = 0;

    printf("Transaction committed\n");
}

void ROLLBACK(struct Entry *table[], struct Transaction *transaction)
{
    if (!transaction -> active)
    {
        printf("No active transaction\n");
        return;
    }
    freeTable(table);
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        table[i] = transaction->snapshot[i];
        transaction -> snapshot[i] = NULL;
    }

    transaction -> active = 0;
    printf("Transaction rolled back\n");
}
