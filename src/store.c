#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "store.h"

int isExpired(struct Entry *entry)
{
    if (entry -> expiry == 0)
    {
        return 0;
    }

    if (time(NULL) >= entry->expiry)
    {
        return 1;
    }

    return 0;
}

unsigned long hashFunction(char key[])
{
    unsigned long hash = 0;

    for (int i = 0; key[i] != '\0'; i++) {
        hash = hash * 31 + key[i];
    }

    return hash;
}

int getIndex(char key[])
{
    unsigned long hash=hashFunction(key);
    return hash % TABLE_SIZE;
}

int insertEntry(struct Entry *table[], char key[], char value[])
{
    int x = getIndex(key);
    struct Entry *current = table[x];

    while(current!=NULL){
        if (strcmp(current -> key, key) == 0)
        {
            if (current -> type != TYPE_STRING)
            {
                printf("Wrong data type\n");
                return -1;
            }

            char *temp = realloc(current -> value, strlen(value) + 1);
            if (temp == NULL)
            {
                printf("Memory allocation failed\n");
                return -1;
            }

            current -> value = temp;
            strcpy((char *)current->value, value);
            return 1;
        }
        current = current -> next;
    }
    struct Entry *newEntry = malloc(sizeof(struct Entry));

    if (newEntry == NULL) {
        printf("Memory allocation failed\n");
        return -1;
    }

    strcpy(newEntry -> key, key);
    newEntry -> value=malloc(strlen(value)+1);

    if (newEntry -> value == NULL)
    {
        printf("Memory allocation failed\n");
        free(newEntry);
        return -1;
    }
    strcpy((char *)newEntry->value, value);
    newEntry -> type=TYPE_STRING;
    newEntry -> next = table[x];
    newEntry->expiry = 0;
    table[x] = newEntry;
    return 1;
}

void freeEntry(struct Entry *entry)
{
    if (entry == NULL)
    {
        return;
    }

    if (entry -> type == TYPE_STRING)
    {
        free(entry -> value);
    }

    else if (entry -> type == TYPE_LIST)
    {
        struct ListNode *current =(struct ListNode *)entry->value;

        while (current != NULL)
        {
            struct ListNode *next = current->next;
            free(current);
            current = next;
        }
    }

    else if (entry -> type == TYPE_SET)
    {
        struct SetNode *current =(struct SetNode *)entry->value;

        while (current != NULL)
        {
            struct SetNode *next = current->next;
            free(current);
            current = next;
        }
    }

    else if (entry -> type == TYPE_HASH)
    {
        struct HashNode *current =(struct HashNode *)entry->value;

        while (current != NULL)
        {
            struct HashNode *next = current->next;
            free(current);
            current = next;
        }
    }

    free(entry);
}

void freeTable(struct Entry *table[])
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        struct Entry *current = table[i];

        while (current != NULL)
        {
            struct Entry *next = current->next;

            freeEntry(current);

            current = next;
        }

        table[i] = NULL;
    }
}

int deleteEntry(struct Entry *table[], char key[]){
    int x=getIndex(key);
    struct Entry *current = table[x];

    if(current == NULL){
        printf("Entry not found\n");
        return -1;
    }

   if (current->next == NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            table[x] = NULL;
            current->next = NULL;
            freeEntry(current);
            return 1;
        }
        printf("Entry not found\n");
        return -1;
    }
    while(current->next!=NULL){

        if(strcmp(current -> next -> key,key)==0){
            struct Entry *temp=current -> next;
            current->next = current ->next->next;
            temp -> next = NULL;
            freeEntry(temp);
            return 1;
        }
        current=current->next;
    }
    printf("Entry not found\n");
    return -1;
}

void getEntry(struct Entry *table[], char key[])
{
    int x=getIndex(key);
    struct Entry *current = table[x];

    while(current != NULL){

        if(strcmp(current -> key,key)==0){

            if (isExpired(current))
            {
                deleteEntry(table, key);
                printf("Key expired\n");
                return;
            }
            if (current -> type != TYPE_STRING)
            {
                printf("Wrong data type\n");
                return;
            }
            printf("key: %s,value: %s\n",current -> key,(char *)current -> value);
            return;
        }
        current = current -> next;
    }
    printf("Entry not found\n");
}

void EXPIRE(struct Entry *table[], char key[], int seconds)
{
    int x = getIndex(key);

    struct Entry *current = table[x];

    while (current != NULL)
    {
        if (strcmp(current -> key, key) == 0)
        {
            current -> expiry = time(NULL) + seconds;

            printf("Key will expire in %d seconds\n", seconds);
            return;
        }

        current = current->next;
    }

    printf("Entry not found\n");
}

int TTL(struct Entry *table[], char key[])
{
    int x = getIndex(key);
    struct Entry *current = table[x];

    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            if (isExpired(current))
            {
                deleteEntry(table, key);
                return -2;
            }
            if (current->expiry == 0)
            {
                return -1;
            }
            return (int)(current->expiry - time(NULL));
        }
        current = current->next;
    }
    return -2;
}
