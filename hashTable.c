#include <stdio.h>
#include <stdlib.h>

struct GeneralNode {
    char *key;
    int val;
    struct GeneralNode *next;
};

struct GeneralHashTable {
    int size;
    struct GeneralNode **list;
};

struct GeneralHashTable *createTable(int size){
    struct GeneralHashTable *t = (struct GeneralHashTable*) malloc(sizeof(struct GeneralHashTable));
    t->size = size;
    t->list = (struct GeneralNode**) malloc(sizeof(struct GeneralNode*) * size);
    
    int i;

    for(i=0;i<size;i++)
        t->list[i] = NULL;
    
    return t;
}

unsigned int rol(const unsigned int value, int shift) {
    if ((shift &= sizeof(value) * 8 - 1) == 0)
      return value;
    return (value << shift) | (value >> (sizeof(value)*8 - shift));
}

unsigned int hashCodeString(char *str)
{
    unsigned int hash = 0x55555555;

    while(*str) {
        hash ^= *str++;
        hash = rol(hash, 5);
    }

    return hash;
}

int hashCode(struct GeneralHashTable *t, char *key){
    int hash = hashCodeString(key);
    
    return hash % t->size;
}

void insert(struct GeneralHashTable *t, char *key, int val){
    int pos = hashCode(t,key);
    struct GeneralNode *list = t->list[pos];
    struct GeneralNode *temp = list;

    while(temp) {
        if(temp->key == key){
            temp->val = val;
            return;
        }

        temp = temp->next;
    }

    struct GeneralNode *newNode = (struct GeneralNode*) malloc(sizeof(struct GeneralNode));

    newNode->key = key;
    newNode->val = val;
    newNode->next = list;

    t->list[pos] = newNode;
}

int find(struct GeneralHashTable *t, char *key){
    int pos = hashCode(t, key);
    struct GeneralNode *list = t->list[pos];
    struct GeneralNode *temp = list;
    
    while(temp){
        if(temp->key == key){
            return temp->val;
        }
        temp = temp->next;
    }

    return -1;
}

/*
int main(){
    struct GeneralHashTable *t = createTable(5);
    insert(t,2,3);
    insert(t,5,4);
    printf("%d", find(t,5));
    return 0;
}
*/