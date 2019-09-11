#include "entity.c"

struct Entities *e;

struct EntityNode {
    struct Entity *kv; /* key and val */
    struct EntityNode *next;
};

struct Entities {
    int size;
    int indexes[SIZE_INIT_GENERAL]; // list of used hashes (indexes of list)
    int indexesSize;
    struct EntityNode *list[SIZE_INIT_GENERAL];
    struct StringList *relations;
};

static inline struct Entities *createEntities() {
    struct Entities *e = (struct Entities*) calloc(1, sizeof(struct Entities));
    
    e->relations = createStringList();
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

    for (i = 0; i < e->indexesSize; i++) {
        node = e->list[e->indexes[i]];
        
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

    if(getEntityByName(ent2) == NULL) return -2;

    int index = binaryStringListAddSearch(e->relations, rel);
    
    int retval = insertRelationEntity(ent, ent2, e->relations->list[index]);
    
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

    return 0;
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

    freeStringList(e->relations);
    free(e);
}

// filterType: 0 => no filter
// filterType: 1 => entity that receive a relation
// filterType: 2 => entity that give a relation
// filterType: 3 => relation
static inline void printEntities(char *filter, int filterType) {
    int i, j, isFirst;

    for (i = 0; i < e->indexesSize; i++) {
        struct EntityNode *entNode = e->list[e->indexes[i]];

        while (entNode) {
            int entered = 0;
            struct Entity *ent = entNode->kv;

            struct RelationshipsNode *rel;
            
            rel = NULL;
            
            if (ent->relationships != NULL) rel = relationshipsToArray(ent->relationships);

            switch (filterType) {
                case 0:
                    printf("Entity: %s\n", ent->name);
                    entered = 1;

                    while (rel) {
                        printf ("   Name: %s, size: %d, hash: %d\n", rel->key, rel->val->size, hashCode(SIZE_INIT, rel->key));

                        for (j = 0; j < rel->val->size; j++) {
                            printf("      Rel: %s", rel->val->list[j]);
                        }

                        printf("\n");

                        rel = rel->next;
                    }
                    break;

                case 1:
                    isFirst = 1;

                    while (rel) {
                        if (strcmp(rel->key, filter) == 0) {
                            entered = 1;
                            if (isFirst) {
                                printf("Entity: %s\n", ent->name);
                                isFirst = 0;
                            }

                            printf ("   Name: %s, size: %d, hash: %d\n", rel->key, rel->val->size, hashCode(SIZE_INIT, rel->key));

                            for (j = 0; j < rel->val->size; j++) {
                                printf("      Rel: %s", rel->val->list[j]);
                            }

                            printf("\n");
                        }

                        rel = rel->next;
                    }
                    break;

                case 2:
                    if (strcmp(ent->name, filter) == 0) {           
                        entered = 1;         
                        printf("Entity: %s\n", ent->name);

                        while (rel) {
                            printf ("   Name: %s, size: %d, hash: %d\n", rel->key, rel->val->size, hashCode(SIZE_INIT, rel->key));

                            for (j = 0; j < rel->val->size; j++) {
                                printf("      Rel: %s", rel->val->list[j]);
                            }

                            printf("\n");

                            rel = rel->next;
                        }
                    }
                    break;

                case 3:
                    isFirst = 1;

                    while (rel) {
                        int isFirstFirst = 1;
                        
                        for (j = 0; j < rel->val->size; j++) {
                            if (strcpy(rel->val->list[j], filter) == 0) {
                                entered = 1;

                                if (isFirst) {
                                    printf("Entity: %s\n", ent->name);
                                    isFirst = 0;
                                }

                                if (isFirstFirst) {
                                    printf ("   Name: %s, size: %d, hash: %d\n", rel->key, rel->val->size, hashCode(SIZE_INIT, rel->key));
                                    isFirstFirst = 0;
                                }

                                printf("      Rel: %s", rel->val->list[j]);
                            }
                        }

                        if (!isFirstFirst) printf("\n");
                        
                        rel = rel->next;
                    }
                    break;
            }
            
            if (entered) printf("\n");

            entNode = entNode->next;
        }
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
    } while (c != '\n' && c != '\r' && c != 0);

    str[i - 1] = 0;
}

static inline int parseInput(char *s, FILE *out) {
    int index = 0, i = 0;
    char temp[130];
    
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

        if (retval > 0) {
            addReportComparsa(report, rel, getEntityByName(ent2)->name);
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

    if (strcmp(temp, "print") == 0) {
        char filter[MAX_STR], filterType;

        if (s[index - 1] == 0) {
            printEntities("", 0);
            return 6;
        }

        int space = 0;

        do {
            if (!space) {
                if (s[index] != ' ') {
                    filter[i] = s[index];
                } else {
                    space = 1;
                    filter[i] = 0;
                }
            } else if (s[index] != ' ') {
                switch (s[index]) {
                    case '1':
                        printEntities(filter, 1);
                        return 6;

                    case '2':
                        printEntities(filter, 2);
                        return 6;

                    case '3':
                        printEntities(filter, 3);
                        return 6;

                    default:
                        printEntities("", 0);
                        return 6;
                }
            }

            i++;
            index++;
        } while (s[index] != 0);

        return 6;
    }

    if (strcmp(s, "printreport") == 0) {
        printReportDebug(report);
        return 7;
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
    
    fclose(fp);
}
