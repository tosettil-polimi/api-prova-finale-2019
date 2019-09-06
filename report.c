#include "stringlist.c"
#include "binaryintarray.c"

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

static inline void printReportDebug(struct Report *rep) {
    for (int i = 0; i < rep->relationsNum; i++) {
        printf("Relation: %s, max comparse: %d\n", rep->objects[i]->relName, rep->objects[i]->maxRelNum);
        for (int j = 0; j < rep->objects[i]->names->size; j++) {
            printf("\tNome: %s, comparse: %d\n", rep->objects[i]->names->list[j], rep->objects[i]->numComparse[j]);
        }
    }
}

struct Report *report;