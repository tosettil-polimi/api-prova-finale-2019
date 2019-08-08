#include "stringlist.c"

struct RelationshipsNode {
    char *key;
    struct StringList *val;
    struct RelationshipsNode *next;
};

struct Relationships {
    int size;
    int elemNum;
    struct RelationshipsNode **list;
};

struct Entity {
    char *name;
    struct Relationships *relationships;
};


int hashCode(int size, const char *key){
    unsigned int hash = 0x55555555;
    int five = 5;
    
    while(*key) {
        hash ^= *key++;
        if (!((five &= sizeof(hash) * 8 - 1) == 0))
            hash = (hash << five) | (hash >> (sizeof(hash)*8 - five));
    }
    
    return hash % size;
}

char *entityToString(struct Entity *ent) {
    return NULL;
}

int insertRelationEntity(struct Entity *ent, char *key, char *relation) {
    int pos = hashCode(ent->relationships->size, key);
    
    struct RelationshipsNode *node = ent->relationships->list[pos];
    struct RelationshipsNode *temp = node;
    struct RelationshipsNode *lastNode = NULL;

    while(temp) { // serve a gestire le collisioni di hash
        if(strcmp(temp->key, key) == 0) // se l'entità è già presente, inserisce la relazione all'interno di quell'entità (per esempio io sono lollo in relazione con luca secondo rel_1,
                                        // se viene chiamato il comando "addrel" "luca" "tose" "rel_2", ora la struttura sarà: lollo % luca -> rel_1, rel_2)
            return binaryStringListAdd(temp->val, relation);

        if (temp->next == NULL)
            lastNode = lastNode;
        
        temp = temp->next;
    }

    if (lastNode == NULL) { // no collisioni, inserisco nella lista principale della hash
        lastNode = (struct RelationshipsNode*) malloc(sizeof(struct RelationshipsNode)); // dovrebbe essere uno nuovo, ma risparmio qualcosa di mem

        lastNode->key = (char*) malloc(sizeof(char) * MAX_STR);
        strcpy(lastNode->key, key);

        lastNode->val = createStringList();
        binaryStringListAdd(lastNode->val, relation);
        
        lastNode->next = NULL;

        ent->relationships->list[pos] = lastNode;
    } else { // hash collision, inserisco in coda al nodo con stesso hash
        lastNode->next = (struct RelationshipsNode*) malloc(sizeof(struct RelationshipsNode));

        lastNode->next->key = (char*) malloc(sizeof(char) * MAX_STR);;
        strcpy(lastNode->next->key, key);

        lastNode->next->val = createStringList();
        binaryStringListAdd(lastNode->next->val, relation);

        lastNode->next->next = NULL;
    }
    
    return 1;
}


struct StringList *getRelationsByKey(struct Entity* ent, char *key) {
    struct RelationshipsNode *temp = ent->relationships->list[hashCode(ent->relationships->size, key)];

    while(temp) {
        if (strcmp(temp->key, key) == 0)
            return temp->val; // found
        
        temp = temp->next;
    }

    return NULL; // not found
}

void addRelation(struct Entity *ent, char *key, char *entity) {
    struct StringList *list = getRelationsByKey(ent, key);

    if (list != NULL) {

    }
}

struct Relationships *createRelationships() {
    struct Relationships *hashMap;

    hashMap = (struct Relationships*) malloc(sizeof(struct Relationships));
    hashMap->list = (struct RelationshipsNode**) malloc(sizeof(struct RelationshipsNode*) * SIZE_INIT);
    hashMap->size = SIZE_INIT;
    hashMap->elemNum = 0;

    return hashMap;
}

struct Entity *createEntity(char *name) {
    struct Entity *ent = (struct Entity*) malloc(sizeof(struct Entity));

    ent->name = (char*) malloc(sizeof(char) * MAX_STR);
    strcpy(ent->name, name);
    ent->relationships = createRelationships();

    return ent;
}

void freeEntity(struct Entity *ent) {
    struct RelationshipsNode *temp, *temp2, *next;
    int i, j;
    
    for(i = 0; i < ent->relationships->size; i++) {
        temp = ent->relationships->list[i];

        if (temp != NULL) {            
            temp2 = temp->next;
            
            while (temp2) { // free of concatenated other relationships (same hash)
                next = temp2->next;

                free(temp2->key);
                for (j = 0; j < temp2->val->size; j++) // free of every string in the list
                    free(temp2->val->list[j]);
                free(temp2->val->list); // free of the list
                free(temp2->val);
                free(temp2);

                temp2 = next;
            }

            free(temp->key);
            for (j = 0; j < temp->val->size; j++) // free of every string in the list
                free(temp->val->list[j]);
            free(temp->val->list); // free of the list
            free(temp->val); // free of StringList val

            free(temp);
        }
    }

    free(ent->relationships->list);

    free(ent->name);
    free(ent->relationships);
    
    free(ent);
}

struct RelationshipsNode *relationshipsToArray(struct Relationships *rel) {
    struct RelationshipsNode *first = (struct RelationshipsNode*) malloc(sizeof(struct RelationshipsNode));
    struct RelationshipsNode *node = first;
    struct RelationshipsNode *prec;

    int i;

    for (i = 0; i < rel->size; i++) {
        
        if (rel->list[i] != NULL) {
            struct RelationshipsNode *temp = rel->list[i];
            do {
                node->next = (struct RelationshipsNode*) malloc(sizeof(struct RelationshipsNode));

                node->key = (char*) malloc(sizeof(char) * MAX_STR);
                strcpy(node->key, temp->key);

                node->val = temp->val;

                prec = node;
                node = node->next;

                temp = temp->next;
            } while (temp);
        }
    }

    free(prec->next);
    prec->next = NULL;
    
    return first;
}

struct StringList *getValEntity(struct Entity *ent, const char *key) {
    int pos = hashCode(ent->relationships->size, key);
    struct RelationshipsNode *node = ent->relationships->list[pos];

    while (node) {
        if (strcmp(node->key, key) == 0)
            return node->val;
        
        node = node->next;
    }

    return NULL;
}

void main() {
    struct Entity *ent;
    int i;
    
    ent = createEntity("tose");

    insertRelationEntity(ent, "gio", "amico");
    insertRelationEntity(ent, "gio", "compagno");
    insertRelationEntity(ent, "gio", "culo");

    insertRelationEntity(ent, "dio", "culo");
    insertRelationEntity(ent, "dio", "cane");

    insertRelationEntity(ent, "amns", "culo");
    insertRelationEntity(ent, "amnb", "culo");
    insertRelationEntity(ent, "amne", "cane");
    
    struct RelationshipsNode *rel = relationshipsToArray(ent->relationships);

    while (rel) {
        printf ("Name: %s, hash: %d\n", rel->key, hashCode(SIZE_INIT, rel->key));

        for (i = 0; i < rel->val->size; i++) {
            printf("\tRel: %s", rel->val->list[i]);
        }
        printf("\n");

        rel = rel->next;
    }

    freeEntity(ent);
}