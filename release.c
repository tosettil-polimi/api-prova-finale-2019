#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#define SIZE_INIT 50   // size of hashmap relationship
#define SIZE_INIT_GENERAL 400 // size of entities hashmap
#define MAX_STR 33      // max size for strings
#define MAX_RELATION 15 //max relations present

struct StringList {
    int size;
    char **list;
};

static inline int binaryStringListAdd(struct StringList *list, char *str) {
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


static inline int binaryStringListSearch(struct StringList *list, char *str) {
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

static inline int binaryStringListDelete(struct StringList *list, char *str) {
    int index = binaryStringListSearch(list, str);

    if (index < 0) 
        return -1;
    
    for (short i = index; i < list->size - 1; i++)
        strcpy(list->list[i], list->list[i+1]);

    free(list->list[(list->size) - 1]);
    list->size--;

    list->list = (char**) realloc(list->list, list->size * sizeof(char*));

    return index;
}

static inline struct StringList *createStringList() {
    struct StringList *list;

    list = (struct StringList*) malloc(sizeof(struct StringList));
    list->size = 0;
    list->list = NULL;
    
    return list;
}

static inline void freeStringList(struct StringList *list) {
    int i = 0;

    for (; i < list->size; i++) {
        free(list->list[i]);
    }

    free(list);
}
static inline int binaryAdd(int *v, int size, int val) {
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

static inline int unsortedSearch(int *v, int size, int val) {
    // Start searching from both ends 
    int front = 0, back = size - 1; 
  
    // Keep searching while two indexes 
    // do not cross. 
    while (front <= back) {
        if (v[front] == val) return front;
        if (v[back] == val) return back;
        front++; 
        back--; 
    }

    return -1; 
}

static inline int binarySearch(int *v, int size, int val) {
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

static inline int binaryDelete(int *v, int size, int val) {
    int i = binarySearch(v, size, val);

    if (i < 0) 
        return 0;
    
    for (; i < size - 1; i++)
        v[i] = v[i+1];

    return 1;
}

struct Report {
    struct ReportObject *objects[MAX_RELATION];
    int relationsNum;
};

struct ReportObject {
    int maxRelNum;
    char relName[MAX_STR];
    struct StringList *names;
    int *numComparse;
};

static inline struct Report *createReport() {
    struct Report *rep = (struct Report*) calloc(1, sizeof(struct Report));
    rep->relationsNum = 0;

    return rep;
}

static inline int addReportObject(struct Report *rep, struct ReportObject *obj) {
    int added = 0, cmp, i, j;

    for (i = 0; i < rep->relationsNum; i++) {
        cmp = strcmp(obj->relName, rep->objects[i]->relName);

        if (cmp < 0) {            
            rep->relationsNum++;

            struct ReportObject *prec, *temp;
            temp = obj;

            added = 1;

            for (j = i; j < rep->relationsNum; j++) {
                prec = rep->objects[j];
                rep->objects[j] = temp;
                temp = prec;
            }

            break;
        } else if (cmp == 0) { // already present
            return -2;
        }
    }

    if (!added) {
        rep->objects[rep->relationsNum] = obj;
        rep->relationsNum++;
    }

    return i;
}

static inline int findReportObject(struct Report *rep, char *relName) {
    int l = 0, r = rep->relationsNum - 1;

    while (l <= r) {
        int m = l + (r - l) / 2;

        int res = strcmp(relName, rep->objects[m]->relName);
        
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


static inline void addComparsa(struct ReportObject *obj, char *name) {
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

static inline struct ReportObject *createReportObject(char *relName) {
    struct ReportObject *rep = (struct ReportObject*) malloc(sizeof(struct ReportObject));

    strcpy(rep->relName, relName);

    rep->maxRelNum = 0;
    rep->names = createStringList();
    
    return rep;
}

// aggiunge all'oggetto report la relazione che l'entita 'name' riceve
static inline int addReportComparsa(struct Report *rep, char *relName, char *name) {
    int index = findReportObject(rep, relName);
    struct ReportObject *obj;

    if (index < 0) {
        obj = createReportObject(relName);
        addReportObject(rep, obj);
        addComparsa(obj, name);
        
        return 1;
    }

    obj = rep->objects[index];

    addComparsa(obj, name);

    return 2;
}

static inline void freeReportObject(struct ReportObject *rep) {
    freeStringList(rep->names);
    free(rep->numComparse);
    free(rep);
}

static inline int deleteReportObject(struct Report *rep, char *relName) {
    int index = findReportObject(rep, relName);

    if (index < 0) return 0;

    freeReportObject(rep->objects[index]);
    
    for (short i = index; i < rep->relationsNum - 1; i++) 
        rep->objects[i] = rep->objects[i + 1];
    
    rep->relationsNum--;

    return 1;
}

static inline int removeReportComparsa(struct Report *rep, char *relName, char *name) {
    int index = findReportObject(rep, relName);
    struct ReportObject *obj;

    if (index < 0) return -1;       // no relation with name "relName"
    
    obj = rep->objects[index];
    index = binaryStringListSearch(obj->names, name);

    if (index < 0) return 0;       // no relation for entity "name"

    if (obj->numComparse[index] == 1) { // only one relation, time to delete the name from the report object
        for (short i = index; i < obj->names->size - 1; i++) {
            obj->numComparse[i] = obj->numComparse[i + 1];
            strcpy(obj->names->list[i], obj->names->list[i + 1]); // TODO: check if is possible to do assignment
        }
        
        obj->numComparse = (int*) realloc(obj->numComparse, sizeof(int) * (obj->names->size - 1));
        free(obj->names->list[(obj->names->size) - 1]);
        obj->names->size--;

        if (obj->names->size == 0) {
            deleteReportObject(rep, relName);
        }

        return 1;
    } 
    
    obj->numComparse[index]--;

    if ((obj->numComparse[index] + 1) == obj->maxRelNum) {
        index = unsortedSearch(obj->numComparse, obj->names->size, obj->maxRelNum);
        if (index < 0) obj->maxRelNum--;
    }

    return 2;
}

// maybe not to use
static inline void delentReport(struct Report *rep, char *name) {
    for (short i = 0; i < rep->relationsNum; i++) {
        struct ReportObject *obj = rep->objects[i];

        int index = binaryStringListSearch(obj->names, name);

        if(index >= 0) {
            int numComparse = obj->numComparse[index];

            for (short j = index; j < obj->names->size - 1; j++) {
                strcpy(obj->names->list[j], obj->names->list[j + 1]);
                obj->numComparse[j] = obj->numComparse[j + 1];
            }

            obj->names->size--;

            if (obj->names->size == 0) {
                deleteReportObject(rep, obj->relName);
                continue;
            }

            if (numComparse == obj->maxRelNum) {                
                index = binarySearch(obj->numComparse, obj->names->size, obj->maxRelNum);
                if (index < 0) obj->maxRelNum--;
            }
        }
    }
}

static inline void freeReport(struct Report *rep) {
    for (short i = 0; i < rep->relationsNum; i++) {
        freeReportObject(rep->objects[i]);
    }

    free(rep);
}

static inline void printReportObject(struct ReportObject *rep, FILE *fp) {
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

    fprintf(fp, "%d", rep->maxRelNum);
    fputc(';', fp);
}

struct Report *report;


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
        if(strcmp(temp->key, key) == 0) { // se l'entità è già presente, inserisce la relazione all'interno di quell'entità (per esempio io sono lollo in relazione con luca secondo rel_1,
                                        // se viene chiamato il comando "addrel" "tose" "luca" "rel_2", ora la struttura sarà: lollo % luca -> rel_1, rel_2)
            return (binaryStringListAdd(temp->val, relation) >= 0);
        }

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

static inline void freeStringListReport(struct StringList *list, char *name) {
    for (short i; i < list->size; i++) {
        removeReportComparsa(report, list->list[i], name);
        free(list->list[i]);
    }

    free(list);
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

            freeStringListReport(node->val, node->key);
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

                for (j = 0; j < temp2->val->size; j++) { // free of every string in the list
                    removeReportComparsa(report, temp2->val->list[j], temp2->key);
                    free(temp2->val->list[j]);    
                }
                
                free(temp2->key);
                free(temp2->val->list); // free of the list
                free(temp2->val);
                free(temp2);

                temp2 = next;
            }

            for (j = 0; j < temp->val->size; j++) { // free of every string in the list
                removeReportComparsa(report, temp->val->list[j], temp->key);
                free(temp->val->list[j]);                    
            }
            
            free(temp->key);
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
}

static inline void readline(char *str, FILE *fp) {
    char c;
    int i = 0;

    do {
        c = fgetc(fp);
        str[i] = c;
        i++;
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

        if (retval > 0) {
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
        /*
        if (retval) {
            delentReport(report, temp);
        }
        */
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

    return 0;

}

void main() {
    int i, j, stop = 0;
    char input[130];

    e = createEntities();
    report = createReport();

    do {
        readline(input, stdin);
        stop = parseInput(input, stdout);
    } while (stop != 10);
}
