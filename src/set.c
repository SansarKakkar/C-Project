#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "store.h"

int SADD(struct Entry *table[], char key[], char value[]){
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
            if (current -> type != TYPE_SET)
            {
                printf("Wrong data type\n");
                return -1;
            }
            struct SetNode *temp =(struct SetNode *)current->value;
            while(temp != NULL){
                if(strcmp(temp -> value, value)==0){
                    printf("Duplicate Value\n");
                    return -1;
                }
                temp = temp->next;
            }
            struct SetNode *newNode = malloc(sizeof(struct SetNode));

            if (newNode == NULL)
            {
                printf("Memory allocation failed\n");
                return -1;
            }
            strcpy(newNode -> value, value);

            struct SetNode *currentNode = (struct SetNode *)current->value;
            newNode->next = currentNode;
            current->value = newNode;

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
    struct SetNode *newNode = malloc(sizeof(struct SetNode));

    if (newNode == NULL)
    {
        printf("Memory allocation failed\n");
        free(newEntry);
        return -1;
    }

    strcpy(newNode->value, value);
    newNode -> next = NULL;
    newEntry -> type = TYPE_SET;
    newEntry -> value = newNode;
    newEntry -> next = table[x];
    newEntry->expiry = 0;
    table[x] = newEntry;
    return 1;
}

int SREM(struct Entry *table[], char key[],char value[])
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
            if (current -> type != TYPE_SET)
            {
                printf("Wrong data type\n");
                return -1;
            }
            struct SetNode * currentNode = (struct SetNode *)current->value;
            struct SetNode * prevNode = NULL;
            while(currentNode != NULL){

                if(strcmp(currentNode -> value, value) ==0){
                    break;
                }
                prevNode = currentNode;
                currentNode = currentNode -> next;
            }

            if(currentNode == NULL){
                return -1;
            }

            if(currentNode -> next == NULL){

                if(prevNode == NULL){

                    if(prev == NULL){
                        table[x] = current -> next;
                        current -> next = NULL;
                        free(currentNode);
                        free(current);
                        return 1;
                    }
                    prev->next=current->next;
                    current->next=NULL;
                    free(currentNode);
                    free(current);
                    return 1;
                }
            }

            if(prevNode == NULL){
                current -> value = currentNode -> next;
                currentNode -> next = NULL;
                free(currentNode);
                return 1;
            }
            prevNode -> next = currentNode -> next;
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

void SISMEMBER(struct Entry *table[], char key[],char value[])
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
            if (current -> type != TYPE_SET)
            {
                printf("Wrong data type\n");
                return;
            }
            struct SetNode * currentNode = (struct SetNode *)current -> value;

            while(currentNode != NULL){

                if(strcmp(currentNode -> value,value) ==0){
                    printf("value Exists");
                    return;
                }
                currentNode = currentNode -> next;
            }
            printf("Value Does Not Exists");
            return;
        }
        current = current -> next;
    }
    printf("Entry not found\n");
    return;
}

void SMEMBERS(struct Entry *table[], char key[])
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
            if (current -> type != TYPE_SET)
            {
                printf("Wrong data type\n");
                return;
            }
            struct SetNode * currentNode=(struct SetNode *)current->value;

            while(currentNode != NULL){
                printf("%s->",currentNode -> value);
                currentNode=currentNode -> next;
            }
            printf("null\n");
            return;
        }
        current = current -> next;
    }
    printf("Entry not found\n");
    return;
}
