#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "store.h"

int LPUSH(struct Entry *table[], char key[], char value[])
{
    int x = getIndex(key);
    struct Entry *current = table[x];

    while (current != NULL)
    {
        if (strcmp(current -> key, key) == 0)
        {
            if (isExpired(current))
            {
                deleteEntry(table, key);
                printf("Key expired\n");
                return -1;
            }
            if (current -> type != TYPE_LIST)
            {
                printf("Wrong data type\n");
                return -1;
            }
            struct ListNode *newNode = malloc(sizeof(struct ListNode));
            if (newNode == NULL)
            {
                printf("Memory allocation failed\n");
                return -1;
            }

            strcpy(newNode->value, value);
            struct ListNode *currentNode =(struct ListNode *)current->value;
            newNode->next = currentNode;
            current->value = newNode;
            return 1;
        }
        current = current->next;
    }
    struct Entry *newEntry = malloc(sizeof(struct Entry));

    if (newEntry == NULL)
    {
        printf("Memory allocation failed\n");
        return -1;
    }

    strcpy(newEntry->key, key);
    struct ListNode *newNode = malloc(sizeof(struct ListNode));

    if (newNode == NULL)
    {
        printf("Memory allocation failed\n");
        free(newEntry);
        return -1;
    }

    strcpy(newNode -> value, value);
    newNode -> next = NULL;
    newEntry -> type = TYPE_LIST;
    newEntry -> value = newNode;
    newEntry -> next = table[x];
    newEntry->expiry = 0;
    table[x] = newEntry;
    return 1;
}

int RPUSH(struct Entry *table[], char key[], char value[])
{
    int x = getIndex(key);
    struct Entry *current = table[x];

    while (current != NULL)
    {
        if (strcmp(current -> key, key) == 0)
        {
             if (isExpired(current))
            {
                deleteEntry(table, key);
                printf("Key expired\n");
                return -1;
            }
            if (current -> type != TYPE_LIST)
            {
                printf("Wrong data type\n");
                return -1;
            }
            struct ListNode *newNode = malloc(sizeof(struct ListNode));

            if (newNode == NULL)
            {
                printf("Memory allocation failed\n");
                return -1;
            }

            strcpy(newNode -> value, value);
            struct ListNode *currentNode=(struct ListNode *)current -> value;
            while(currentNode -> next!= NULL){
                currentNode = currentNode->next;
            }
            newNode -> next= NULL;
            currentNode -> next = newNode;
            return 1;
        }

        current = current -> next;
    }
    struct Entry *newEntry = malloc(sizeof(struct Entry));

    if (newEntry == NULL)
    {
        printf("Memory allocation failed\n");
        return -1;
    }

    strcpy(newEntry -> key, key);
    struct ListNode *newNode = malloc(sizeof(struct ListNode));

    if (newNode == NULL)
    {
        printf("Memory allocation failed\n");
        free(newEntry);
        return -1;
    }

    strcpy(newNode -> value, value);
    newNode -> next = NULL;
    newEntry -> type = TYPE_LIST;
    newEntry -> value = newNode;
    newEntry -> next = table[x];
    newEntry->expiry = 0;
    table[x] = newEntry;
    return 1;
}

int LPOP(struct Entry *table[], char key[])
{
    int x = getIndex(key);
    struct Entry *current = table[x];
    struct Entry *prev = NULL;
    while (current != NULL)
    {
        if (strcmp(current -> key, key) == 0)
        {
             if (isExpired(current))
            {
                deleteEntry(table, key);
                printf("Key expired\n");
                return -1;
            }
            if (current -> type != TYPE_LIST)
            {
                printf("Wrong data type\n");
                return -1;
            }
            struct ListNode * currentNode = (struct ListNode *)current->value;
            if(currentNode -> next == NULL){

                if(prev == NULL){
                    table[x] = current->next;
                    current -> next = NULL;
                    free(current);
                    return 1;
                }
                prev -> next = current -> next;
                current -> next = NULL;
                free(current);
                return 1;
            }
            current -> value = currentNode -> next;
            currentNode -> next = NULL;
            free(currentNode);
            return 1;
        }
        prev = current;
        current = current->next;
    }
    printf("Entry not found\n");
    return -1;
}

int RPOP(struct Entry *table[], char key[])
{
    int x = getIndex(key);
    struct Entry *current = table[x];
    struct Entry *prev = NULL;
    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
             if (isExpired(current))
            {
                deleteEntry(table, key);
                printf("Key expired\n");
                return -1;
            }
            if (current->type != TYPE_LIST)
            {
                printf("Wrong data type\n");
                return -1;
            }
            struct ListNode * currentNode = (struct ListNode *)current->value;
            if(currentNode -> next == NULL){

                if(prev == NULL){
                    table[x] = current->next;
                    current -> next = NULL;
                    free(current);
                    return 1;
                }
                prev -> next = current -> next;
                current -> next = NULL;
                free(current);
                return 1;
            }

            while(currentNode -> next -> next != NULL){
                currentNode = currentNode -> next;
            }
            struct ListNode * temp = currentNode -> next;
            currentNode -> next = NULL;
            free(temp);
            return 1;
        }
        prev = current;
        current = current->next;
    }
    printf("Entry not found\n");
    return -1;
}

void LRANGE(struct Entry *table[], char key[])
{
    int x = getIndex(key);
    struct Entry *current = table[x];

    while (current != NULL)
    {
        if (strcmp(current -> key, key) == 0)
        {
             if (isExpired(current))
            {
                deleteEntry(table, key);
                printf("Key expired\n");
                return ;
            }
            if (current -> type != TYPE_LIST)
            {
                printf("Wrong data type\n");
                return;
            }
            struct ListNode *currentNode = (struct ListNode *)current->value;

            while (currentNode != NULL)
            {
                printf("%s->", currentNode -> value);
                currentNode = currentNode -> next;
            }
            printf("null\n");
            return;
        }

        current = current->next;
    }

    printf("Entry not found\n");
}
