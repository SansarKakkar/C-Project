#include <stdio.h>
#include <string.h>
#include "persist.h"
#include "store.h"
#include "list.h"
#include "set.h"
#include "hash.h"

void SaveCommands(char *command){
    FILE *file = fopen("Database.txt", "a");
    if (file == NULL)
    {
        printf("Could not open file\n");
        return ;
    }
    fprintf(file,"%s\n" ,command);
    fclose(file);
}

void LoadDatabase(struct Entry *table[])
{
    FILE *file = fopen("Database.txt", "r");
    if (file == NULL)
    {
        return;
    }
    char command[200];
    while (fgets(command, sizeof(command), file) != NULL)
    {
        command[strcspn(command, "\n")] = '\0';
        char *arr[10];
        int count = 0;
        char *token = strtok(command, " \t\n");

        while (token != NULL && count < 10)
        {
            arr[count] = token;
            count++;
            token = strtok(NULL, " \t\n");
        }
        if (count == 0)
        {
            continue;
        }
        if (strcmp(arr[0], "SET") == 0)
        {
            insertEntry(table, arr[1], arr[2]);
        }
        else if (strcmp(arr[0], "DEL") == 0)
        {
            deleteEntry(table, arr[1]);
        }
        else if (strcmp(arr[0], "LPUSH") == 0)
        {
            LPUSH(table, arr[1], arr[2]);
        }
        else if (strcmp(arr[0], "RPUSH") == 0)
        {
            RPUSH(table, arr[1], arr[2]);
        }
        else if (strcmp(arr[0], "LPOP") == 0)
        {
            LPOP(table, arr[1]);
        }
        else if (strcmp(arr[0], "RPOP") == 0)
        {
            RPOP(table, arr[1]);
        }
        else if (strcmp(arr[0], "SADD") == 0)
        {
            SADD(table, arr[1], arr[2]);
        }
        else if (strcmp(arr[0], "SREM") == 0)
        {
            SREM(table, arr[1], arr[2]);
        }
        else if (strcmp(arr[0], "HSET") == 0)
        {
            HSET(table, arr[1], arr[2], arr[3]);
        }
        else if (strcmp(arr[0], "HDEL") == 0)
        {
            HDEL(table, arr[1], arr[2]);
        }
    }
    fclose(file);
}
