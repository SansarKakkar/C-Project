#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TABLE_SIZE 10
struct ListNode {
    char value[100];
    struct ListNode *next;
};
struct SetNode {
    char value[100];
    struct SetNode *next;
};
struct HashNode{
    char field[50];
    char value[100];
    struct HashNode *next;
};
enum DataType {
    TYPE_STRING,
    TYPE_LIST,
    TYPE_SET,
    TYPE_HASH
};
struct Entry {
    char key[50];
    enum DataType type;
    void *value;
    struct Entry *next;
};
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
    return hash %TABLE_SIZE;
}
// **********************************************SET***********************************************
int insertEntry(struct Entry *table[], char key[], char value[])
{
    int x = getIndex(key);
    struct Entry *current = table[x];
    while(current!=NULL){
        if (strcmp(current->key, key) == 0)
        {
            if (current->type != TYPE_STRING)
            {
                printf("Wrong data type\n");
                return -1;
            }
            char *temp = realloc(current->value, strlen(value) + 1);
            if (temp == NULL)
            {
                printf("Memory allocation failed\n");
                return -1;
            }
            current->value = temp;
            strcpy((char *)current->value, value);
            return 1;
        }
        current=current->next;
    }
    struct Entry *newEntry = malloc(sizeof(struct Entry));

    if (newEntry == NULL) {
        printf("Memory allocation failed\n");
        return -1;
    }

    strcpy(newEntry->key, key);
    newEntry->value=malloc(strlen(value)+1);
    if (newEntry->value == NULL)
    {
        printf("Memory allocation failed\n");
        free(newEntry);
        return -1;
    }
    strcpy((char *)newEntry->value, value);
    newEntry->type=TYPE_STRING;
    newEntry->next = table[x];
    table[x] = newEntry;
    return 1;
}
// **********************************************GET***********************************************
void getEntry(struct Entry *table[], char key[])
{
    int x=getIndex(key);
    struct Entry *current = table[x];
    while(current!=NULL){
        if(strcmp(current->key,key)==0){
            if (current->type != TYPE_STRING)
            {
                printf("Wrong data type\n");
                return;
            }
            printf("key: %s,value: %s\n",current->key,(char *)current->value);
            return;
        }
        current=current->next;
    }
    printf("Entry not found\n");
}
// **********************************************DEL***********************************************
int deleteEntry(struct Entry *table[], char key[]){
    int x=getIndex(key);
    struct Entry *current = table[x];
    if(current==NULL){
        printf("Entry not found\n");
        return -1;
    }
    if(current->next==NULL){
         if(strcmp(current->key,key)==0){
            if (current->type != TYPE_STRING)
            {
                printf("Wrong data type\n");
                return -1;
            }
            table[x]=current->next;
            current->next=NULL;
            free(current->value);
            free(current);
            return 1;
         }
         printf("Entry not found\n");
         return -1;
    }
    while(current->next!=NULL){
        if(strcmp(current->next->key,key)==0){
            struct Entry *temp=current->next;
            current->next=current->next->next;
            temp->next=NULL;
            free(temp->value);
            free(temp);
            return 1;
        }
        current=current->next;
    }
    printf("Entry not found\n");
    return -1;
}
// ******************************************LPUSH***************************************************
int LPUSH(struct Entry *table[], char key[], char value[])
{
    int x = getIndex(key);
    struct Entry *current = table[x];

    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            if (current->type != TYPE_LIST)
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

    strcpy(newNode->value, value);
    newNode->next = NULL;
    newEntry->type = TYPE_LIST;
    newEntry->value = newNode;
    newEntry->next = table[x];
    table[x] = newEntry;
    return 1;
}
// **********************************************RPUSH***********************************************
int RPUSH(struct Entry *table[], char key[], char value[])
{
    int x = getIndex(key);
    struct Entry *current = table[x];

    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            if (current->type != TYPE_LIST)
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
            struct ListNode *currentNode=(struct ListNode *)current->value;
            while(currentNode->next!=NULL){
                currentNode=currentNode->next;
            }
            newNode->next=NULL;
            currentNode->next = newNode;
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

    strcpy(newNode->value, value);
    newNode->next = NULL;
    newEntry->type = TYPE_LIST;
    newEntry->value = newNode;
    newEntry->next = table[x];
    table[x] = newEntry;
    return 1;
}
// **********************************************LPOP***********************************************
int LPOP(struct Entry *table[], char key[])
{
    int x = getIndex(key);
    struct Entry *current = table[x];
    struct Entry *prev = NULL;
    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            if (current->type != TYPE_LIST)
            {
                printf("Wrong data type\n");
                return -1;
            }
            struct ListNode * currentNode=(struct ListNode *)current->value;
            if(currentNode->next==NULL){
                if(prev==NULL){
                    table[x]=current->next;
                    current->next=NULL;
                    free(current);
                    return 1;
                }
                prev->next=current->next;
                current->next=NULL;
                free(current);
                return 1;
            }
            current->value=currentNode->next;
            currentNode->next=NULL;
            free(currentNode);
            return 1;
        }
        prev=current;
        current = current->next;
    }
    printf("Entry not found");
    return -1;
}
// **********************************************RPOP***********************************************
int RPOP(struct Entry *table[], char key[])
{
    int x = getIndex(key);
    struct Entry *current = table[x];
    struct Entry *prev = NULL;
    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            if (current->type != TYPE_LIST)
            {
                printf("Wrong data type\n");
                return -1;
            }
            struct ListNode * currentNode=(struct ListNode *)current->value;
            if(currentNode->next==NULL){
                if(prev==NULL){
                    table[x]=current->next;
                    current->next=NULL;
                    free(current);
                    return 1;
                }
                prev->next=current->next;
                current->next=NULL;
                free(current);
                return 1;
            }
            while(currentNode->next->next!=NULL){
                currentNode=currentNode->next;
            }
            struct ListNode * temp=currentNode->next;
            currentNode->next=NULL;
            free(temp);
            return 1;
        }
        prev=current;
        current = current->next;
    }
    printf("Entry not found");
    return -1;
}
// **********************************************LRANGE***********************************************
void LRANGE(struct Entry *table[], char key[])
{
    int x = getIndex(key);
    struct Entry *current = table[x];

    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            if (current->type != TYPE_LIST)
            {
                printf("Wrong data type\n");
                return;
            }
            struct ListNode *currentNode = (struct ListNode *)current->value;

            while (currentNode != NULL)
            {
                printf("%s->", currentNode->value);
                currentNode = currentNode->next;
            }
            printf("null\n");

            return;
        }

        current = current->next;
    }

    printf("Entry not found\n");
}
// **********************************************SADD***********************************************
int SADD(struct Entry *table[], char key[], char value[]){
    int x = getIndex(key);
    struct Entry *current = table[x];
    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            if (current->type != TYPE_SET)
            {
                printf("Wrong data type\n");
                return -1;
            }
            struct SetNode *temp =(struct SetNode *)current->value;
            while(temp!=NULL){
                if(strcmp(temp->value,value)==0){
                    printf("Duplicate Value\n");
                    return -1;
                }
                temp=temp->next;
            }
            struct SetNode *newNode = malloc(sizeof(struct SetNode));

            if (newNode == NULL)
            {
                printf("Memory allocation failed\n");
                return -1;
            }
            strcpy(newNode->value, value);

            struct SetNode *currentNode =(struct SetNode *)current->value;
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
    struct SetNode *newNode = malloc(sizeof(struct SetNode));

    if (newNode == NULL)
    {
        printf("Memory allocation failed\n");
        free(newEntry);
        return -1;
    }

    strcpy(newNode->value, value);
    newNode->next = NULL;
    newEntry->type = TYPE_SET;
    newEntry->value = newNode;
    newEntry->next = table[x];
    table[x] = newEntry;
    return 1;
}
// **********************************************SREM***********************************************
int SREM(struct Entry *table[], char key[],char value[])
{
    int x = getIndex(key);
    struct Entry *current = table[x];
    struct Entry *prev = NULL;
    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            if (current->type != TYPE_SET)
            {
                printf("Wrong data type\n");
                return -1;
            }
            struct SetNode * currentNode=(struct SetNode *)current->value;
            struct SetNode * prevNode=NULL;
            while(currentNode!=NULL){
                if(strcmp(currentNode->value,value)==0){
                    break;
                }
                prevNode=currentNode;
                currentNode=currentNode->next;
            }
            if(currentNode==NULL){
                return -1;
            }
            if(currentNode->next==NULL){
                if(prevNode==NULL){
                    if(prev==NULL){
                        table[x]=current->next;
                        current->next=NULL;
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
            if(prevNode==NULL){
                current->value=currentNode->next;
                currentNode->next=NULL;
                free(currentNode);
                return 1;
            }
            prevNode->next=currentNode->next;
            currentNode->next=NULL;
            free(currentNode);
            return 1;
        }
        prev=current;
        current = current->next;
    }
    printf("Entry not found");
    return -1;
}
// **********************************************SISMEMBER***********************************************
void SISMEMBER(struct Entry *table[], char key[],char value[])
{
    int x = getIndex(key);
    struct Entry *current = table[x];
    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            if (current->type != TYPE_SET)
            {
                printf("Wrong data type\n");
                return;
            }
            struct SetNode * currentNode=(struct SetNode *)current->value;
            while(currentNode!=NULL){
                if(strcmp(currentNode->value,value)==0){
                    printf("value Exists");
                    return;
                }
                currentNode=currentNode->next;
            }
            printf("Value Does Not Exists");
            return;
        }
        current = current->next;
    }
    printf("Entry not found");
    return;
}
// **********************************************SMEMBERS***********************************************
void SMEMBERS(struct Entry *table[], char key[])
{
    int x = getIndex(key);
    struct Entry *current = table[x];
    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            if (current->type != TYPE_SET)
            {
                printf("Wrong data type\n");
                return;
            }
            struct SetNode * currentNode=(struct SetNode *)current->value;
            while(currentNode!=NULL){
                printf("%s->",currentNode->value);
                currentNode=currentNode->next;
            }
            printf("null\n");
            return;
        }
        current = current->next;
    }
    printf("Entry not found");
    return;
}
// ********************************************HSET*******************************************
int HSET(struct Entry *table[], char key[],char field[],char value[]){
    int x=getIndex(key);
    struct Entry *current=table[x];
    while(current!=NULL){
        if(strcmp(current->key,key)==0){
            if (current->type != TYPE_HASH)
            {
                printf("Wrong data type\n");
                return -1;
            }
            struct HashNode *currentNode=(struct HashNode *)current->value;
            struct HashNode *temp=currentNode;
            while(temp!=NULL){
                if(strcmp(temp->field,field)==0){
                    strcpy(temp->value,value);
                    return 1;
                }
                temp=temp->next;
            }
            struct HashNode * newNode=malloc(sizeof(struct HashNode));
            if (newNode == NULL)
            {
                printf("Memory allocation failed\n");
                return -1;
            }
            strcpy(newNode->field,field);
            strcpy(newNode->value,value);
            newNode->next=currentNode;
            current->value=newNode;
            return 1;
        }
        current=current->next;
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
    strcpy(newNode->field,field);
    strcpy(newNode->value,value);
    newNode->next = NULL;
    newEntry->next=table[x];
    strcpy(newEntry->key,key);
    newEntry->type=TYPE_HASH;
    newEntry->value=newNode;
    table[x]=newEntry;
    return 1;

}
// ********************************************HGET********************************************
void HGET(struct Entry *table[], char key[],char field[]){
    int x=getIndex(key);
    struct Entry *current=table[x];
    while(current!=NULL){
        if(strcmp(current->key,key)==0){
            if (current->type != TYPE_HASH)
            {
                printf("Wrong data type\n");
                return;
            }
            struct HashNode *currentNode=(struct HashNode *)current->value;
            while(currentNode!=NULL){
                if(strcmp(currentNode->field,field)==0){
                    printf("value: %s\n",currentNode->value);
                    return;
                }
                currentNode=currentNode->next;
            }
            printf("feild not found\n");
            return;
        }
        current=current->next;
    }
    printf("entry not found");
    return;
}
// ********************************************HDEL********************************************
int HDEL(struct Entry *table[], char key[], char field[]){
    int x = getIndex(key);
    struct Entry *current = table[x];
    struct Entry *prev = NULL;
    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            if (current->type != TYPE_HASH)
            {
                printf("Wrong data type\n");
                return -1;
            }
            struct HashNode *currentNode =(struct HashNode *)current->value;
            struct HashNode *prevNode = NULL;
            while (currentNode != NULL)
            {
                if (strcmp(currentNode->field, field) == 0)
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
            if (currentNode->next == NULL && prevNode == NULL)
            {
                if (prev == NULL)
                {
                    table[x] = current->next;
                }
                else
                {
                    prev->next = current->next;
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
// ********************************************HGETALL********************************************
void HGETALL(struct Entry *table[], char key[]){
    int x = getIndex(key);
    struct Entry *current = table[x];
    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            if (current->type != TYPE_HASH)
            {
                printf("Wrong data type\n");
                return;
            }
            struct HashNode *currentNode =(struct HashNode *)current->value;
            while (currentNode != NULL)
            {
                printf("%s : %s\n",currentNode->field,currentNode->value);
                currentNode = currentNode->next;
            }
            return;
        }
        current = current->next;
    }
    printf("Entry not found\n");
}
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
int main()
{
    char input[100];
    char originalCommand[100];
    struct Entry *table[TABLE_SIZE] = {NULL};
    LoadDatabase(table);
    while (1)
    {
        printf("> ");
        fgets(input, sizeof(input), stdin);
        strcpy(originalCommand, input);
        originalCommand[strcspn(originalCommand, "\n")] = '\0';
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "exit\n") == 0)
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
// **********************************************LPUSH***********************************************
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
// *************************************************RPUSH************************************************
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
// *********************************************LPOP***************************************************
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
// *****************************************RPOP*****************************************************
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
// ********************************************LRANGE********************************************
        else if (strcmp(arr[0], "LRANGE") == 0)
        {
            if (count != 2)
            {
                printf("Usage: LRANGE key\n");
                continue;
            }

            LRANGE(table, arr[1]);
        }
// ********************************************SADD********************************************
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
// ********************************************SREM********************************************
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
// ********************************************SISMEMBER********************************************
        else if (strcmp(arr[0], "SISMEMBER") == 0)
        {
            if (count != 3)
            {
                printf("Usage: SISMEMBER key value\n");
                continue;
            }
            SISMEMBER(table, arr[1], arr[2]);
        }
// ********************************************SMEMBERS********************************************
        else if (strcmp(arr[0], "SMEMBERS") == 0 )
        {
            if (count != 2)
            {
                printf("Usage: SMEMBERS key\n");
                continue;
            }

            SMEMBERS(table, arr[1]);
        }       
// ********************************************HSET********************************************
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
// ********************************************HGET********************************************
        else if (strcmp(arr[0], "HGET") == 0 )
        {
            if (count != 3)
            {
                printf("Usage: HGET key\n");
                continue;
            }
            HGET(table, arr[1], arr[2]);
        }
// ********************************************HDEL********************************************
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
// ********************************************HGETALL********************************************
        else if (strcmp(arr[0], "HGETALL") == 0 )
        {
            if (count != 2)
            {
                printf("Usage: HGETALL key\n");
                continue;
            }
            HGETALL(table, arr[1]);
        }
// ********************************************DEFAULT********************************************
        else
        {
            printf("Unknown command\n");
        }
    }

    return 0;
}