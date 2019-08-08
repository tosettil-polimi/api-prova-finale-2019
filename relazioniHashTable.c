#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utente.c"

int hashCode(int, char*);

struct Node {
    char *key;
    char *val;
    struct Node *next;
};

struct RelazioniHashTable {
    int size;
    struct Node **list;
};


struct RelazioniHashTable *createTable(){
    struct RelazioniHashTable *table;
    table = (struct RelazioniHashTable*) malloc(sizeof(struct RelazioniHashTable));
    table->size = 100;
    table->list = (struct Node**) malloc(sizeof(struct Node*) * 100);
    
    int i;

    for(i=0;i<5;i++)
        table->list[i] = NULL;
    
    return table;
}

int insertGeneral(struct RelazioniHashTable *table, char *key, char *val){
    pos = hashCode(table->size, key);
    list = table->list[pos];
    temp = list;

    while(temp) {

        if(strcmp(temp->key, key) == 0) {
            free(temp->val);
            temp->val = NULL;
            return 0;
        }

        temp = temp->next;
    }

    struct GeneralNode *newNode = (struct GeneralNode*) malloc(sizeof(struct GeneralNode));

    newNode->key = key;
    newNode->val = val;
    newNode->next = list;

    table->list[pos] = newNode;
    return 1;
}