#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#define SIZE_INIT 1000   // size of hashmap relationship
#define SIZE_INIT_GENERAL 10000 // size of entities hashmap
#define MAX_STR 33      // max size for strings


struct StringList {
    int size;
    char **list;
};

int binaryStringListAdd(struct StringList *list, char *str) {
    int added = 0, cmp, i, j;

    for (i = 0; i < list->size; i++) {
        cmp = strcmp(str, list->list[i]);

        if (cmp < 0) {

            list->list = (char**) realloc(list->list, (list->size + 1 ) * sizeof(char*));
            
            list->list[list->size] = (char*) malloc(sizeof(char) * MAX_STR);
            list->size++;
            

            char prec[MAX_STR], temp[MAX_STR];
            strcpy(temp, str);

            added = 1;

            for (j = i; j < list->size; j++) {
                strcpy(prec, list->list[j]);
                strcpy(list->list[j], temp);
                strcpy(temp, prec);
            }

            break;
        } else if (cmp == 0) { // already present
            return -2;
        }
    }

    if (!added) {
        if (list->list == NULL) list->list = (char**) malloc(sizeof(char*));
        else list->list = (char**) realloc(list->list, (list->size + 1 ) * sizeof(char*));

        list->list[list->size] = (char*) malloc(sizeof(char) * MAX_STR);
        strcpy(list->list[list->size], str);
        list->size++;
    }

    return i;
}


int binaryStringListSearch(struct StringList *list, char *str) {
    int l = 0, r = list->size - 1;

    while (l <= r) {
        int m = l + (r - l) / 2;

        int res = strcmp(str, list->list[m]);
        
        // Check if x is present at mid
        if (res == 0)
            return m;

        // If x greater, ignore left half
        if (res > 0)
            l = m + 1;

            // If x is smaller, ignore right half
        else
            r = m - 1;
    }
    
    return -1;
}

int binaryStringListDelete(struct StringList *list, char *str) {
    int i = binaryStringListSearch(list, str);

    if (i < 0) 
        return 0;

    char prec[MAX_STR], temp[MAX_STR];
    
    for (; i < list->size - 1; i++)
        strcpy(list->list[i], list->list[i+1]);

    free(list->list[(list->size) - 1]);
    list->size--;

    list->list = (char**) realloc(list->list, list->size * sizeof(char*));

    return 1;
}

struct StringList *createStringList() {
    struct StringList *list;

    list = (struct StringList*) malloc(sizeof(struct StringList));
    list->size = 0;
    list->list = NULL;
    
    return list;
}

void freeStringList(struct StringList *list) {
    int i = 0;

    for (; i < list->size; i++) {
        free(list->list[i]);
    }

    free(list);
}

struct StringList *relationsPresent;

int binaryAdd(int *v, int size, int val) {
    int i, j;

    for (i = 0; i < size; i++) {
        if (val < v[i] || val == v[i]) {
            int prec, temp;
            temp = val;

            for (j = i; j < size + 1; j++) { // shift
                prec = v[j];
                v[j] = temp;
                temp = prec;
            }

            return i;
        } 
    }

    v[size] = val;

    return size;
}

int binarySearch(int *v, int size, int val) {
    int l = 0, r = size - 1;

    while (l <= r) {
        int m = l + (r - l) / 2;

        // Check if x is present at mid
        if (val == v[m])
            return m;

        // If x greater, ignore left half
        if (val > v[m])
            l = m + 1;

            // If x is smaller, ignore right half
        else
            r = m - 1;
    }
    
    return -1;
}

int binaryDelete(int *v, int size, int val) {
    int i = binarySearch(v, size, val);

    if (i < 0) 
        return 0;
    
    for (; i < size - 1; i++)
        v[i] = v[i+1];

    return 1;
}

struct ReportObject {
    int maxRelNum;
    char relName[MAX_STR];
    struct StringList *names;
    int *numComparse;
};

void createReportObject(struct ReportObject *obj, char *relName) {
    strcpy(obj->relName, relName);

    obj->maxRelNum = 0;
    obj->names = createStringList();
}

void addComparsa(struct ReportObject *obj, char *name) {
    int index = binaryStringListAdd(obj->names, name);
    int i;
    
    if (index >= 0) {
        if (obj->names->size == 1)
            obj->numComparse = (int*) malloc(sizeof(int));
        else
            obj->numComparse = (int*) realloc(obj->numComparse, sizeof(int) * obj->names->size);
        
        int prec = obj->numComparse[index], precPrec;
        obj->numComparse[index] = 1;

        for (i = index; i < obj->names->size - 1; i++) {
            precPrec = obj->numComparse[i + 1];
            obj->numComparse[i + 1] = prec;
            prec = precPrec;
        }        
    } else {
        index = binaryStringListSearch(obj->names, name);
        obj->numComparse[index]++;
    }

    if (obj->maxRelNum < obj->numComparse[index]) {
        obj->maxRelNum = obj->numComparse[index];
    }
}

void freeReportObject(struct ReportObject *rep) {
    freeStringList(rep->names);
    free(rep->numComparse);
}

char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

void printReportObject(struct ReportObject *rep, FILE *fp) {
    char c;
    int i = 0, j = 0;

    fputc('"', fp);
    do {
        c = rep->relName[i];
        if (c != 0) fputc(c, fp);
        i++;
    } while (c != 0);
    fputc('"', fp);
    
    fputc(' ', fp);

    for (i = 0; i < rep->names->size; i++) {
        if (rep->numComparse[i] == rep->maxRelNum) {
            fputc('"', fp);
            do {   
                c = rep->names->list[i][j];
                if (c != 0) fputc(c, fp);
                j++;
            } while(c != 0);
            fputc('"', fp);
            fputc(' ', fp);

            j = 0;
        }
    }

    char str[10];
    itoa(rep->maxRelNum, str);
    i = 1;

    c = str[0];

    while (c != 0) {
        fputc(c, fp);
        c = str[i];
        i++;
    }

    fputc(';', fp);
}


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
    if(ent == NULL) return -1;
    
    int pos = hashCode(ent->relationships->size, key);
    
    struct RelationshipsNode *node = ent->relationships->list[pos];
    struct RelationshipsNode *temp = node;
    struct RelationshipsNode *lastNode = NULL;

    while(temp) { // serve a gestire le collisioni di hash
        if(strcmp(temp->key, key) == 0) // se l'entità è già presente, inserisce la relazione all'interno di quell'entità (per esempio io sono lollo in relazione con luca secondo rel_1,
                                        // se viene chiamato il comando "addrel" "tose" "luca" "rel_2", ora la struttura sarà: lollo % luca -> rel_1, rel_2)
            return binaryStringListAdd(temp->val, relation);

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


struct StringList *getRelationsByKey(struct Entity* ent, char *key) {
    struct RelationshipsNode *temp = ent->relationships->list[hashCode(ent->relationships->size, key)];

    while(temp) {
        if (strcmp(temp->key, key) == 0)
            return temp->val; // found
        
        temp = temp->next;
    }

    return NULL; // not found
}

int deleteRelation(struct Entity *ent, char *name, char *rel) {
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
int deleteRelEntByName(struct Entity* ent, char *key) {
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

void addRelation(struct Entity *ent, char *key, char *entity) {
    struct StringList *list = getRelationsByKey(ent, key);

    if (list != NULL) {

    }
}

struct Relationships *createRelationships() {
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

    free(ent->name);
    free(ent->relationships);
    
    free(ent);
}

struct RelationshipsNode *relationshipsToArray(struct Relationships *rel) {
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
};

struct Entities *createEntities() {
    struct Entities *e = (struct Entities*) malloc(sizeof(struct Entities));
    
    e->size = SIZE_INIT_GENERAL;
    e->indexesSize = 0;

    return e;
}


int addent(char *name) {
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

void deleteDependencies(char *name) {
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

int delent(char *name) {
    int pos = hashCode(e->size, name);

    struct EntityNode *node = e->list[pos];
    struct EntityNode *prec = node;

    while (node) {
        if (strcmp(node->kv->name, name) == 0) {
            /*
            if (strcmp(name, "Elizabeth_Cutler") == 0) {
                printf("node: %ld\n", node);
            }*/
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

struct Entity *getEntityByName(char *name) {
    int pos = hashCode(e->size, name);

    struct EntityNode *ent = e->list[pos];

    while (ent) {
        if (strcmp(ent->kv->name, name) == 0)
            return ent->kv;
        
        ent = ent->next;
    }

    return NULL;
}

int addrel(char *ent1, char *ent2, char *rel) {
    struct Entity *ent = getEntityByName(ent1);

    if(e->list[hashCode(e->size, ent2)] == NULL) return -2;
    
    int retval = insertRelationEntity(ent, ent2, rel);
    
    if (retval == -2)
        return -4;

    if (retval > 0) 
        return binaryStringListAdd(relationsPresent, rel);
    
    return retval;
}

int delrel(char *ent1, char *ent2, char *rel) {
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

void report(FILE *fp) {
    int i, j, z;

    for (i = 0; i < relationsPresent->size; i++) {
        int relMaxNum = 0;
        char *rel = relationsPresent->list[i];
        
        struct ReportObject *rep = (struct ReportObject*) malloc(sizeof(struct ReportObject));
        createReportObject(rep, rel);

        for (j = 0; j < e->indexesSize; j++) {
            struct EntityNode *node = e->list[e->indexes[j]];

            while (node) {
                for (z = 0; z < node->kv->relationships->indexesSize; z++) {
                    struct RelationshipsNode *relNode = node->kv->relationships->list[node->kv->relationships->indexes[z]];
                    
                    while (relNode) {
                        int index = binaryStringListSearch(relNode->val, rel);
                        
                        if (index >= 0) {
                            addComparsa(rep, relNode->key);
                        }

                        relNode = relNode->next;
                    }
                }

                node = node->next;
            }
        }

        printReportObject(rep, fp);
        
        if (i + 1 != relationsPresent->size) fputc(' ', fp);

        freeReportObject(rep);
    }

    if (relationsPresent->size == 0) 
        fputs("none", fp);

    fputc('\n', fp);
}

void freeEntities() {
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

void printEntities() {
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

void readline(char *str, FILE *fp) {
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
    int nQuot = 0;

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

        return addrel(ent1, ent2, rel);
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
        
        return delent(temp);
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

        return delrel(ent1, ent2, rel);
    }

    if (strcmp(s, "report") == 0) {
        report(out);
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

void main(int argn, char **argv) {
    int i, j, stop = 0;
    char input[130];

    strcpy(input, "test/");
    strcat(input, argv[1]);

    FILE *in = fopen(input, "r");
    FILE *out = fopen("output.txt", "w");

    e = createEntities();
    relationsPresent = createStringList();

    do {
        readline(input, in);
        stop = parseInput(input, out);
    } while (stop != 10);

    freeEntities();
}