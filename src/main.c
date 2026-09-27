#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "store.h"
#include "list.h"
#include "set.h"
#include "hash.h"
#include "persist.h"
#include "transaction.h"

int main()
{
    char input[100];
    char originalCommand[100];
    struct Entry *table[TABLE_SIZE] = {NULL};
    struct Transaction transaction = {0};
    LoadDatabase(table);
    while (1)
    {
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }
        if (strchr(input, '\n') == NULL)
        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF){}
            printf("Command too long\n");
            continue;
        }
        strcpy(originalCommand, input);
        originalCommand[strcspn(originalCommand, "\n")] = '\0';
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "exit") == 0)
        {
            return 0;
        }

        char *arr[10];
        int count = 0;

        char *token = strtok(input, " \t\n");
        while (token != NULL)
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
            if (count != 3)
            {
                printf("Usage: SET key value\n");
                continue;
            }
            int found=insertEntry(table, arr[1], arr[2]);
            if(found==1){
                SaveCommands(originalCommand);
            }
        }

        else if (strcmp(arr[0], "GET") == 0)
        {
            if (count != 2)
            {
                printf("Usage: GET key\n");
                continue;
            }
            getEntry(table, arr[1]);
        }

        else if (strcmp(arr[0], "DEL") == 0)
        {
            if (count != 2)
            {
                printf("Usage: DEL key\n");
                continue;
            }
            int found=deleteEntry(table, arr[1]);
            if(found==1){
                SaveCommands(originalCommand);
            }
        }
        else if (strcmp(arr[0], "LPUSH") == 0)
        {
            if (count != 3)
            {
                printf("Usage: LPUSH key value\n");
                continue;
            }
            int found=LPUSH(table, arr[1], arr[2]);
            if(found==1){
                SaveCommands(originalCommand);
            }
        }
        else if (strcmp(arr[0], "RPUSH") == 0)
        {
            if (count != 3)
            {
                printf("Usage: RPUSH key value\n");
                continue;
            }
            int found=RPUSH(table, arr[1], arr[2]);
            if(found==1){
                SaveCommands(originalCommand);
            }
        }
        else if (strcmp(arr[0], "LPOP") == 0)
        {
            if (count != 2)
            {
                printf("Usage: LPOP key\n");
                continue;
            }
            int found=LPOP(table, arr[1]);
            if(found==1){
                SaveCommands(originalCommand);
            }
        }
        else if (strcmp(arr[0], "RPOP") == 0)
        {
            if (count != 2)
            {
                printf("Usage: RPOP key\n");
                continue;
            }
            int found=RPOP(table, arr[1]);
            if(found==1){
                SaveCommands(originalCommand);
            }
        }
        else if (strcmp(arr[0], "LRANGE") == 0)
        {
            if (count != 2)
            {
                printf("Usage: LRANGE key\n");
                continue;
            }

            LRANGE(table, arr[1]);
        }
        else if (strcmp(arr[0], "SADD") == 0)
        {
            if (count != 3)
            {
                printf("Usage: SADD key value\n");
                continue;
            }
            int found=SADD(table, arr[1], arr[2]);
            if(found==1){
                SaveCommands(originalCommand);
            }
        }
        else if (strcmp(arr[0], "SREM") == 0)
        {
            if (count != 3)
            {
                printf("Usage: SREM key value\n");
                continue;
            }
            int found=SREM(table, arr[1], arr[2]);
            if(found==1){
                SaveCommands(originalCommand);
            }
        }
        else if (strcmp(arr[0], "SISMEMBER") == 0)
        {
            if (count != 3)
            {
                printf("Usage: SISMEMBER key value\n");
                continue;
            }
            SISMEMBER(table, arr[1], arr[2]);
        }
        else if (strcmp(arr[0], "SMEMBERS") == 0 )
        {
            if (count != 2)
            {
                printf("Usage: SMEMBERS key\n");
                continue;
            }

            SMEMBERS(table, arr[1]);
        }
        else if (strcmp(arr[0], "HSET") == 0)
        {
            if (count != 4)
            {
                printf("Usage: HSET key\n");
                continue;
            }
            int found=HSET(table, arr[1], arr[2], arr[3]);
            if(found==1){
                SaveCommands(originalCommand);
            }
        }
        else if (strcmp(arr[0], "HGET") == 0 )
        {
            if (count != 3)
            {
                printf("Usage: HGET key\n");
                continue;
            }
            HGET(table, arr[1], arr[2]);
        }
        else if (strcmp(arr[0], "HDEL") == 0 )
        {
            if (count != 3)
            {
                printf("Usage: HDEL key\n");
                continue;
            }
            int found=HDEL(table, arr[1], arr[2]);
            if(found==1){
                SaveCommands(originalCommand);
            }
        }
        else if (strcmp(arr[0], "HGETALL") == 0 )
        {
            if (count != 2)
            {
                printf("Usage: HGETALL key\n");
                continue;
            }
            HGETALL(table, arr[1]);
        }
        else if (strcmp(arr[0], "BEGIN") == 0 && count == 1)
        {
            BEGIN(table, &transaction);
        }
        else if (strcmp(arr[0], "COMMIT") == 0 && count == 1)
        {
            COMMIT(&transaction);
        }
        else if (strcmp(arr[0], "ROLLBACK") == 0 && count == 1)
        {
            ROLLBACK(table, &transaction);
        }
        else if (strcmp(arr[0], "EXPIRE") == 0 && count == 3)
        {
            EXPIRE(table, arr[1], atoi(arr[2]));
        }
        else if (strcmp(arr[0], "TTL") == 0 && count == 2)
        {
            printf("%d\n", TTL(table, arr[1]));
        }
        else
        {
            printf("Unknown command\n");
        }
    }

    return 0;
}
