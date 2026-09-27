#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "store.h"

int HSET(struct Entry *table[], char key[],char field[],char value[]){
    int x=getIndex(key);
    struct Entry *current=table[x];

    while(current != NULL){

        if(strcmp(current -> key, key)==0){

             if (isExpired(current))
            {
                deleteEntry(table, key);
                printf("Key expired\n");
                return -1;
            }
            if (current -> type != TYPE_HASH)
            {
                printf("Wrong data type\n");
                return -1;
            }
            struct HashNode *currentNode=(struct HashNode *)current->value;
            struct HashNode *temp = currentNode;

            while(temp != NULL){

                if(strcmp(temp -> field, field)==0){
                    strcpy(temp -> value, value);
                    return 1;
                }
                temp = temp -> next;
            }
            struct HashNode * newNode = malloc(sizeof(struct HashNode));

            if (newNode == NULL)
            {
                printf("Memory allocation failed\n");
                return -1;
            }
            strcpy(newNode -> field, field);
            strcpy(newNode -> value, value);
            newNode -> next = currentNode;
            current -> value = newNode;
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
    struct HashNode *newNode = malloc(sizeof(struct HashNode));

    if (newNode == NULL)
    {
        printf("Memory allocation failed\n");
        free(newEntry);
        return -1;
    }
    strcpy(newNode -> field,field);
    strcpy(newNode -> value,value);
    newNode -> next = NULL;
    newEntry -> next = table[x];
    strcpy(newEntry -> key,key);
    newEntry -> type = TYPE_HASH;
    newEntry -> value = newNode;
    newEntry->expiry = 0;
    table[x] = newEntry;
    return 1;

}

void HGET(struct Entry *table[], char key[],char field[]){
    int x=getIndex(key);
    struct Entry *current = table[x];

    while(current!=NULL){

        if(strcmp(current -> key, key)==0){

             if (isExpired(current))
            {
                deleteEntry(table, key);
                printf("Key expired\n");
                return ;
            }
            if (current -> type != TYPE_HASH)
            {
                printf("Wrong data type\n");
                return;
            }
            struct HashNode *currentNode = (struct HashNode *)current->value;

            while(currentNode!=NULL){
                if(strcmp(currentNode -> field, field)==0){
                    printf("value: %s\n",currentNode -> value);
                    return;
                }
                currentNode = currentNode->next;
            }
            printf("feild not found\n");
            return;
        }
        current = current -> next;
    }
    printf("entry not found\n");
    return;
}

int HDEL(struct Entry *table[], char key[], char field[]){
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
            if (current -> type != TYPE_HASH)
            {
                printf("Wrong data type\n");
                return -1;
            }
            struct HashNode *currentNode = (struct HashNode *)current->value;
            struct HashNode *prevNode = NULL;
            while (currentNode != NULL)
            {
                if (strcmp(currentNode -> field, field) == 0)
                {
                    break;
                }
                prevNode = currentNode;
                currentNode = currentNode->next;
            }
            if (currentNode == NULL)
            {
                printf("Field not found\n");
                return -1;
            }
            if (currentNode -> next == NULL && prevNode == NULL)
            {
                if (prev == NULL)
                {
                    table[x] = current -> next;
                }
                else
                {
                    prev -> next = current -> next;
                }
                free(currentNode);
                free(current);
                return 1;
            }
            if (prevNode == NULL)
            {
                current->value = currentNode->next;
                free(currentNode);
                return 1;
            }
            prevNode->next = currentNode->next;
            free(currentNode);
            return 1;
        }
        prev = current;
        current = current->next;
    }
    printf("Entry not found\n");
    return -1;
}

void HGETALL(struct Entry *table[], char key[]){
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
            if (current -> type != TYPE_HASH)
            {
                printf("Wrong data type\n");
                return;
            }
            struct HashNode *currentNode = (struct HashNode *)current->value;

            while (currentNode != NULL)
            {
                printf("%s : %s\n", currentNode -> field, currentNode -> value);
                currentNode = currentNode->next;
            }
            return;
        }
        current = current->next;
    }
    printf("Entry not found\n");
}
