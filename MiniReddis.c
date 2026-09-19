#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TABLE_SIZE 10
struct ListNode {
    char value[100];
    struct ListNode *next;
};
struct Entry {
    char key[50];
    struct ListNode *value;
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
void insertEntry(struct Entry *table[], char key[], char value[])
{
    int x = getIndex(key);
    struct Entry *current = table[x];
    while(current!=NULL){
        if(strcmp(current->key,key)==0){
            strcpy(current->value,value);
            return;
        }
        current=current->next;
    }
    struct Entry *newEntry = malloc(sizeof(struct Entry));

    if (newEntry == NULL) {
        printf("Memory allocation failed\n");
        return;
    }

    strcpy(newEntry->key, key);
    strcpy(newEntry->value, value);

    newEntry->next = table[x];

    table[x] = newEntry;
}
void getEntry(struct Entry *table[], char key[])
{
    int x=getIndex(key);
    struct Entry *current = table[x];
    while(current!=NULL){
        if(strcmp(current->key,key)==0){
            printf("key: %s,value: %s\n",current->key,current->value);
            return;
        }
        current=current->next;
    }
    printf("Entry not found\n");
}
void deleteEntry(struct Entry *table[], char key[]){
    int x=getIndex(key);
    struct Entry *current = table[x];
    if(current==NULL){
        printf("Entry not found\n");
        return;
    }
    if(current->next==NULL){
         if(strcmp(current->key,key)==0){
            table[x]=current->next;
            current->next=NULL;
            free(current);
            return;
         }
         printf("Entry not found\n");
         return;
    }
    while(current->next!=NULL){
        if(strcmp(current->next->key,key)==0){
            struct Entry *temp=current->next;
            current->next=current->next->next;
            temp->next=NULL;
            free(temp);
            return;
        }
        current=current->next;
    }
    printf("Entry not found\n");
}
void LPUSH(struct Entry *table[], char key[], char value[])
{
    int x = getIndex(key);
    struct Entry *current = table[x];

    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            struct ListNode *newNode = malloc(sizeof(struct ListNode));

            if (newNode == NULL)
            {
                printf("Memory allocation failed\n");
                return;
            }

            strcpy(newNode->value, value);
            newNode->next = current->value;
            current->value = newNode;

            return;
        }

        current = current->next;
    }
    struct Entry *newEntry = malloc(sizeof(struct Entry));

    if (newEntry == NULL)
    {
        printf("Memory allocation failed\n");
        return;
    }

    strcpy(newEntry->key, key);
    struct ListNode *newNode = malloc(sizeof(struct ListNode));

    if (newNode == NULL)
    {
        printf("Memory allocation failed\n");
        free(newEntry);
        return;
    }

    strcpy(newNode->value, value);
    newNode->next = NULL;
    newEntry->value = newNode;
    newEntry->next = table[x];
    table[x] = newEntry;
}
void RPUSH(struct Entry *table[], char key[], char value[])
{
    int x = getIndex(key);
    struct Entry *current = table[x];

    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            struct ListNode *newNode = malloc(sizeof(struct ListNode));

            if (newNode == NULL)
            {
                printf("Memory allocation failed\n");
                return;
            }

            strcpy(newNode->value, value);
            struct ListNode *currentNode=current->value;
            while(currentNode->next!=NULL){
                currentNode=currentNode->next;
            }
            newNode->next=NULL;
            currentNode->next = newNode;
            return;
        }

        current = current->next;
    }
    struct Entry *newEntry = malloc(sizeof(struct Entry));

    if (newEntry == NULL)
    {
        printf("Memory allocation failed\n");
        return;
    }

    strcpy(newEntry->key, key);
    struct ListNode *newNode = malloc(sizeof(struct ListNode));

    if (newNode == NULL)
    {
        printf("Memory allocation failed\n");
        free(newEntry);
        return;
    }

    strcpy(newNode->value, value);
    newNode->next = NULL;
    newEntry->value = newNode;
    newEntry->next = table[x];
    table[x] = newEntry;
}
int main(){
    char input[100];
    struct Entry *table[TABLE_SIZE] = {NULL};
    while(1){
        printf("> ");
        fgets(input,sizeof(input),stdin);
        if(strcmp(input,"exit\n")==0){
            return 0;
        }
        char *arr[10];
        int count = 0;
        char *token=strtok(input," \t\n");
        while(token!=NULL){
            arr[count]=token;
            count++;
            token=strtok(NULL," \t\n");
        }
        if(count == 0)
            continue;

        if(strcmp(arr[0], "SET") == 0)
        {
            if(count != 3)
            {
                printf("Usage: SET key value\n");
                continue;
            }
            insertEntry(table, arr[1], arr[2]);
        }
        else if(strcmp(arr[0], "GET") == 0)
        {
            if(count != 2)
            {
                printf("Usage: GET key\n");
                continue;
            }
            getEntry(table, arr[1]);
        }
        else if(strcmp(arr[0], "DEL") == 0)
        {
            if(count != 2)
            {
                printf("Usage: DEL key\n");
                continue;
            }

            deleteEntry(table, arr[1]);
        }
        else
        {
            printf("Unknown command\n");
        }
    }
    return 0;

}
// void SET(struct KeyValue Database[],char key[],char value[],int *idx){
//     int found=-1;
//     for(int i=0;i<*idx;i++){
//         if(strcmp(Database[i].key, key) == 0){
//             found=i;
//             strcpy(Database[i].value,value);
//             break;
//         }
//     }
//     if(found==-1){
//         strcpy(Database[*idx].key,key);
//         strcpy(Database[*idx].value,value);
//         (*idx)++;
//     }
//     return;
// }
// void GET(struct KeyValue Database[],char key[],int *idx){
//     int found=-1;
//     for(int i=0;i<*idx;i++){
//         if(strcmp(Database[i].key, key) == 0){
//             found=i;
//             printf("Key: %s, Value: %s, idx: %d\n",Database[i].key,Database[i].value,i);
//             break;
//         }
//     }
//     if(found==-1){
//         printf("Key not found");
//     }
//     return;
// }
// void DEL(struct KeyValue Database[],char key[],int *idx){
//     int found=-1;
//     for(int i=0;i<*idx;i++){
//         if(strcmp(Database[i].key, key) == 0){
//             found=i;
//             for(int j=i;j<*idx-1;j++){
//                 Database[j]=Database[j+1];
//             }
//             (*idx)--;
//             break;
//         }
//     }
//     if(found==-1){
//         printf("Key not found");
//     }
//     return;
// }
// *****************************************************************
// printf("Key: %s\n", table[index]->key);
// printf("Value: %s\n", table[index]->value);
// *****************************************************************
    // // struct KeyValue Database[100];
    // int idx=0;
    // SET(Database, "name", "Sansar", &idx);
    // SET(Database,"age","34",&idx);
    // SET(Database,"shivansh","good boy",&idx);
    // SET(Database,"age","45",&idx);
    // GET(Database,"shivansh",&idx);
    // DEL(Database,"shivansh",&idx);
    // GET(Database,"shivansh",&idx);
    // insertEntry(table, "rajat", "367");
    // insertEntry(table, "hello", "69");
    // int index = getIndex("rajat");
    // getEntry(table,"hello");
    // deleteEntry(table,"hello");
    // getEntry(table,"hello");
