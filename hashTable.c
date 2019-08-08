#include "relazioniHashTable.c"

int pos;
struct GeneralNode *list;
struct GeneralNode *temp;
struct GeneralHashTable *t;


// TODO
void freeUtente(struct Utente*);
void freeGeneralNode(struct GeneralNode*);

struct GeneralNode {
    char *key;
    struct Utente* val;
    struct GeneralNode *next;
};

struct GeneralHashTable {
    int size;
    struct GeneralNode **list;
};

struct GeneralHashTable *createTableGeneral(){
    t = (struct GeneralHashTable*) malloc(sizeof(struct GeneralHashTable));
    t->size = 500;
    t->list = (struct GeneralNode**) malloc(sizeof(struct GeneralNode*) * 500);
    
    int i;

    for(i=0;i<10;i++)
        t->list[i] = NULL;
    
    return t;
}

int hashCode(int size, char *key){
    unsigned int hash = 0x55555555;
    int five = 5;
    
    while(*key) {
        hash ^= *key++;
        if (!((five &= sizeof(hash) * 8 - 1) == 0))
            hash = (hash << five) | (hash >> (sizeof(hash)*8 - five));
    }
    
    return hash % size;
}

int insertGeneral(char *key, struct Utente *val){
    pos = hashCode(t->size, key);
    list = t->list[pos];
    temp = list;

    while(temp) {

        if(strcmp(temp->key, key) == 0) {
            freeUtente(temp->val);
            temp->val = NULL;
            return 0;
        }

        temp = temp->next;
    }

    struct GeneralNode *newNode = (struct GeneralNode*) malloc(sizeof(struct GeneralNode));

    newNode->key = key;
    newNode->val = val;
    newNode->next = list;

    t->list[pos] = newNode;
    
    return 1;
}

struct Utente* findGeneral(struct GeneralHashTable *t, char *key){
    pos = hashCode(t->size, key);
    list = t->list[pos];
    temp = list;
    
    while(temp){
        if(strcmp(temp->key, key) == 0) {
            return temp->val;
        }
        temp = temp->next;
    }

    return -1;
}

int removeGeneral(struct GeneralHashTable *t, char *key) {
    pos = hashCode(t->size, key);
    list = t->list[pos];
    temp = list;

    while (temp) {
        if (strcmp(temp->key, key) == 0) {
            freeGeneralNode(temp);
            return pos;
        }

        temp = temp->next;
    }

    return -1;
}