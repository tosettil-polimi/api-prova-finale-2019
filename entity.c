#include "report.c"

struct RelationshipsNode {
    char *key;
    struct StringList *val;
    struct RelationshipsNode *next;
};

struct Relationships {
    int size;
    int indexes[SIZE_INIT];
    int indexesSize;
    struct RelationshipsNode *list[SIZE_INIT];
};

struct Entity {
    char *name;
    struct Relationships *relationships;
};


static inline int hashCode(int size, const char *key){
    unsigned int hash = 0x55555555;
    int five = 5;
    
    while(*key) {
        hash ^= *key++;
        if (!((five &= sizeof(hash) * 8 - 1) == 0))
            hash = (hash << five) | (hash >> (sizeof(hash)*8 - five));
    }
    
    return hash % size;
}

static inline char *entityToString(struct Entity *ent) {
    return NULL;
}

static inline int insertRelationEntity(struct Entity *ent, char *key, char *relation) {
    if(ent == NULL) return -1;
    
    int pos = hashCode(ent->relationships->size, key);
    
    struct RelationshipsNode *node = ent->relationships->list[pos];
    struct RelationshipsNode *temp = node;
    struct RelationshipsNode *lastNode = NULL;

    while(temp) { // serve a gestire le collisioni di hash
        if(strcmp(temp->key, key) == 0) // se l'entità è già presente, inserisce la relazione all'interno di quell'entità (per esempio io sono lollo in relazione con luca secondo rel_1,
                                        // se viene chiamato il comando "addrel" "tose" "luca" "rel_2", ora la struttura sarà: lollo % luca -> rel_1, rel_2)
            return binaryStringListAdd(temp->val, relation) >= 0;

        if (temp->next == NULL) {
            lastNode = temp;
        }
        
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

        binaryAdd(ent->relationships->indexes, ent->relationships->indexesSize, pos);
        ent->relationships->indexesSize++;
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


static inline struct StringList *getRelationsByKey(struct Entity* ent, char *key) {
    struct RelationshipsNode *temp = ent->relationships->list[hashCode(ent->relationships->size, key)];

    while(temp) {
        if (strcmp(temp->key, key) == 0)
            return temp->val; // found
        
        temp = temp->next;
    }

    return NULL; // not found
}

static inline int deleteRelation(struct Entity *ent, char *name, char *rel) {
    int pos = hashCode(ent->relationships->size, name);
    struct RelationshipsNode *node = ent->relationships->list[pos];

    while (node) {
        if (strcmp(node->key, name) == 0) {
            binaryStringListDelete(node->val, rel);
            node = ent->relationships->list[pos];
            
            if (node->val->size == 0) { // if is the last relation between the two entities
                freeStringList(node->val);
                free(node->key);
                
                ent->relationships->list[pos] = node->next;

                free(node);
                
                if (ent->relationships->list[pos] == NULL) {
                    binaryDelete(ent->relationships->indexes, ent->relationships->indexesSize, pos);
                    ent->relationships->indexesSize--;
                }
            }
            
            return 1;
        }

        node = node->next;
    }

    return 0;
}

// cancella tutte le relazioni che ent ha con key (si usa con delent)
static inline int deleteRelEntByName(struct Entity* ent, char *key) {
    int pos = hashCode(ent->relationships->size, key);

    struct RelationshipsNode *node = ent->relationships->list[pos];
    struct RelationshipsNode *prec = node;

    while (node) {
        if (strcmp(node->key, key) == 0) {
            if (node == ent->relationships->list[pos])
                ent->relationships->list[pos] = node->next;
            else
                prec->next = node->next;

            freeStringList(node->val);
            free(node->key);
            free(node);

            if (!ent->relationships->list[pos]) {
                binaryDelete(ent->relationships->indexes, ent->relationships->indexesSize, pos);
                ent->relationships->indexesSize--;
            }

            return 1;
        }

        prec = node;
        node = node->next;
    }

    return 0;
}

static inline void addRelation(struct Entity *ent, char *key, char *entity) {
    struct StringList *list = getRelationsByKey(ent, key);

    if (list != NULL) {

    }
}

static inline struct Relationships *createRelationships() {
    struct Relationships *hashMap;

    hashMap = (struct Relationships*) calloc(1, sizeof(struct Relationships));

    hashMap->size = SIZE_INIT;
    hashMap->indexesSize = 0;
    
    /*
    int i;

    for (i = 0; i < SIZE_INIT; i++) {
        hashMap->list[i] = NULL;
    }
    */

    return hashMap;
}

static inline struct Entity *createEntity(char *name) {
    struct Entity *ent = (struct Entity*) malloc(sizeof(struct Entity));

    ent->name = (char*) malloc(sizeof(char) * MAX_STR);
    strcpy(ent->name, name);
    ent->relationships = createRelationships();

    return ent;
}

static inline void freeEntity(struct Entity *ent) {
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

    free(ent->name);
    free(ent->relationships);
    
    free(ent);
}

static inline struct RelationshipsNode *relationshipsToArray(struct Relationships *rel) {
    if (rel->indexesSize == 0) return NULL;

    struct RelationshipsNode *first = (struct RelationshipsNode*) malloc(sizeof(struct RelationshipsNode));
    struct RelationshipsNode *node = first;
    struct RelationshipsNode *prec;

    int i;

    for (i = 0; i < rel->indexesSize; i++) {        
        struct RelationshipsNode *temp = rel->list[rel->indexes[i]];
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

    if (prec != NULL) {
        if (prec->next != NULL) {
            free(prec->next);
            prec->next = NULL;
        }
    }
    
    return first;
}

static inline struct StringList *getValEntity(struct Entity *ent, const char *key) {
    int pos = hashCode(ent->relationships->size, key);
    struct RelationshipsNode *node = ent->relationships->list[pos];

    while (node) {
        if (strcmp(node->key, key) == 0)
            return node->val;
        
        node = node->next;
    }

    return NULL;
}
