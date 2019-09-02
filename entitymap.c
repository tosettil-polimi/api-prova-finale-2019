#include "entity.c"

struct Entities *e;
struct Report *report;

struct EntityNode {
    struct Entity *kv; /* key and val */
    struct EntityNode *next;
};

struct Entities {
    int size;
    int indexes[SIZE_INIT_GENERAL]; // list of used hashes (indexes of list)
    int indexesSize;
    struct EntityNode *list[SIZE_INIT_GENERAL];
};

static inline struct Entities *createEntities() {
    struct Entities *e = (struct Entities*) malloc(sizeof(struct Entities));
    
    e->size = SIZE_INIT_GENERAL;
    e->indexesSize = 0;

    return e;
}


static inline int addent(char *name) {
    int pos = hashCode(e->size, name);
    
    struct EntityNode *node = e->list[pos];
    struct EntityNode *temp = node;
    struct EntityNode *lastNode = NULL;

    while(temp) { // serve a gestire le collisioni di hash
        if(strcmp(temp->kv->name, name) == 0) // se l'entità è già presente, non fa nulla
            return 0;

        if (temp->next == NULL)
            lastNode = temp;
        
        temp = temp->next;
    }

    if (lastNode == NULL) { // no collisioni, inserisco nella lista principale della hash
        lastNode = (struct EntityNode*) malloc(sizeof(struct EntityNode)); // dovrebbe essere uno nuovo, ma risparmio qualcosa di mem

        lastNode->kv = createEntity(name);
        lastNode->next = NULL;

        e->list[pos] = lastNode;
        
        binaryAdd(e->indexes, e->indexesSize, pos);
        e->indexesSize++;
    } else { // hash collision, inserisco in coda al nodo con stesso hash
        lastNode->next = (struct EntityNode*) malloc(sizeof(struct EntityNode));

        lastNode->next->kv = createEntity(name);
        lastNode->next->next = NULL;
    }
    
    return 1;
}

static inline void deleteDependencies(char *name) {
    struct EntityNode *node;
    int i;
    
    //if (strcmp(name, "Elizabeth_Cutler") == 0) printf("e->indexesSize: %d\n", e->indexesSize);

    for (i = 0; i < e->indexesSize; i++) {
        node = e->list[e->indexes[i]];
        //if (strcmp(name, "Elizabeth_Cutler") == 0) printf("e->indexes[%d]: %ld\n", i, e->indexes[i]);
        while (node) {
            struct Entity *ent = node->kv;
            deleteRelEntByName(ent, name);
            node = node->next;
        }
    }
}

static inline int delent(char *name) {
    int pos = hashCode(e->size, name);

    struct EntityNode *node = e->list[pos];
    struct EntityNode *prec = node;

    while (node) {
        if (strcmp(node->kv->name, name) == 0) {
            deleteDependencies(name);

            if (node == e->list[pos]) { // first element
                e->list[pos] = node->next;
            } else {
                prec->next = node->next;
            }

            freeEntity(node->kv);
            free(node);

            if (!e->list[pos]) { // node->next == NULL
                binaryDelete(e->indexes, e->indexesSize, pos);
                e->indexesSize--;
            }

            return 1;
        }

        prec = node;
        node = node->next;
    }

    return 0;
}

static inline struct Entity *getEntityByName(char *name) {
    int pos = hashCode(e->size, name);

    struct EntityNode *ent = e->list[pos];

    while (ent) {
        if (strcmp(ent->kv->name, name) == 0)
            return ent->kv;
        
        ent = ent->next;
    }

    return NULL;
}

static inline int addrel(char *ent1, char *ent2, char *rel) {
    struct Entity *ent = getEntityByName(ent1);

    if(e->list[hashCode(e->size, ent2)] == NULL) return -2;
    
    int retval = insertRelationEntity(ent, ent2, rel);
    
    if (retval == -2)
        return -4;
    
    return retval;
}

static inline int delrel(char *ent1, char *ent2, char *rel) {
    int pos = hashCode(e->size, ent1);
    struct EntityNode *node = e->list[pos];

    while(node) {
        if (strcmp(node->kv->name, ent1) == 0) {
            return deleteRelation(node->kv, ent2, rel);
        }

        node = node->next;
    }

    return -1;
}

static inline void doReport(FILE *fp) {
    if (report->relationsNum == 0) {
        fputs("none\n", fp);
        return;
    }

    for (short i = 0; i < report->relationsNum; i++) {
        printReportObject(report->objects[i], fp);
        if (i + 1 != report->relationsNum) fputc(' ', fp);
    }

    fputc('\n', fp);
}

static inline void freeEntities() {
    int i;

    for (i = 0; i < e->indexesSize; i++) {
        struct EntityNode *node = e->list[e->indexes[i]];
        struct EntityNode *temp;

        while (node) {
            temp = node->next;

            freeEntity(node->kv);
            free(node);

            node = temp;
        }
    }
}

static inline void printEntities() {
    int i, j;    

    for (i = 0; i < e->indexesSize; i++) {
        struct Entity *ent = e->list[e->indexes[i]]->kv;
        struct RelationshipsNode *rel;
        
        rel = NULL;
        
        if (ent->relationships != NULL) rel = relationshipsToArray(ent->relationships);

        printf("Entity: %s\n", ent->name);

        while (rel) {
            printf ("   Name: %s, size: %d, hash: %d\n", rel->key, rel->val->size, hashCode(SIZE_INIT, rel->key));

            for (j = 0; j < rel->val->size; j++) {
                printf("      Rel: %s", rel->val->list[j]);
            }

            printf("\n");

            rel = rel->next;
        }

        printf("\n");
    }
}

static inline void readline(char *str, FILE *fp) {
    char c;
    int i = 0;

    do {
        c = fgetc(fp);
        str[i] = c;
        i++;
        fputc(c, stdout);
    } while (c != '\n');

    str[i - 1] = 0;
}

static inline int parseInput(char *s, FILE *out) {
    int index = 0, i = 0;
    char temp[MAX_STR];
    
    while (s[index] != ' ' && s[index] != 0) {  
        temp[index] = s[index];
        index++;
    }

    temp[index] = 0;

    index += 1;
    int nQuot = 0, retval;

    if (strcmp(temp, "addent") == 0) {
        do {
            if (s[index] != '"') {
                temp[i] = s[index];
                i++;
            } else {
                nQuot++;
            }

            index++;
        } while (s[index] != 0 && nQuot < 2);
        
        temp[i] = 0;

        return addent(temp);
    }

    if (strcmp(temp, "addrel") == 0) {
        char ent1[MAX_STR], ent2[MAX_STR], rel[MAX_STR];

        do {
            if (s[index] != '"') {
                ent1[i] = s[index];
                i++;
            } else {
                nQuot++;
            }

            index++;
        } while (nQuot < 2 && s[index] != 0);

        if(s[index] == 0) return -3;

        ent1[i] = 0, i = 0, nQuot = 0;
        index++;

        do {
            if (s[index] != '"') {
                ent2[i] = s[index];
                i++;
            } else {
                nQuot++;
            }

            index++;
        } while(nQuot < 2);

        if(s[index] == 0) return -3;

        ent2[i] = 0, i = 0;

        do {
            index++;

            if (s[index] != '"') {
                rel[i] = s[index];
                i++;
            }
        } while(s[index] != 0);

        retval = addrel(ent1, ent2, rel);

        if (retval >= 0) {
            addReportComparsa(report, rel, ent2);
        }
    }

    if (strcmp(temp, "delent") == 0) {
        do {
            if (s[index] != '"') {
                temp[i] = s[index];
                i++;
            }

            index++;
        } while (s[index] != 0);
        
        temp[i] = 0;
        
        retval = delent(temp);

        if (retval) {
            delentReport(report, temp);
        }
    }

    if (strcmp(temp, "delrel") == 0) {
        char ent1[MAX_STR], ent2[MAX_STR], rel[MAX_STR];

        do {
            if (s[index] != '"') {
                ent1[i] = s[index];
                i++;
            } else {
                nQuot++;
            }

            index++;
        } while (nQuot < 2 && s[index] != 0);

        if(s[index] == 0) return -3;

        ent1[i] = 0, i = 0, nQuot = 0;
        index++;

        do {
            if (s[index] != '"') {
                ent2[i] = s[index];
                i++;
            } else {
                nQuot++;
            }

            index++;
        } while(nQuot < 2);

        if(s[index] == 0) return -3;

        ent2[i] = 0, i = 0;

        do {
            index++;

            if (s[index] != '"') {
                rel[i] = s[index];
                i++;
            }
        } while(s[index] != 0);

        retval = delrel(ent1, ent2, rel);

        if (retval) {
            removeReportComparsa(report, rel, ent2);
        }
    }

    if (strcmp(s, "report") == 0) {
        doReport(out);
        return 5;
    }

    if (strcmp(s, "end") == 0) {
        return 10;
    }

    if (strcmp(s, "print") == 0) {
        printEntities();
        return 6;
    }

    return 0;

}

void main() {
    int i, j, stop = 0;
    char input[130];

    FILE *fp = fopen("output.txt", "w");

    e = createEntities();
    report = createReport();

    do {
        readline(input, stdin);
        stop = parseInput(input, fp);
    } while (stop != 10);
}
