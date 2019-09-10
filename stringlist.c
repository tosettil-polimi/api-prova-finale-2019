#include "libandconst.h"

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
            

            char *prec, *temp;
            temp = (char*) malloc(sizeof(char) * MAX_STR);
            strcpy(temp, str);

            added = 1;

            for (j = i; j < list->size; j++) {
                prec = list->list[j];
                list->list[j] = temp;
                temp = prec;
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

static inline int binaryStringListAddSame(struct StringList *list, char *str) {
    int added = 0, cmp, i, j;

    for (i = 0; i < list->size; i++) {
        cmp = strcmp(str, list->list[i]);

        if (cmp < 0) {

            list->list = (char**) realloc(list->list, (list->size + 1 ) * sizeof(char*));
            list->size++;
            
            char *prec, *temp;
            temp = str;

            added = 1;

            for (j = i; j < list->size; j++) {
                prec = list->list[j];
                list->list[j] = temp;
                temp = prec;
            }

            break;
        } else if (cmp == 0) { // already present
            return -2;
        }
    }

    if (!added) {
        if (list->list == NULL) list->list = (char**) malloc(sizeof(char*));
        else list->list = (char**) realloc(list->list, (list->size + 1 ) * sizeof(char*));

        list->list[list->size] = str;
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

    free(list->list[index]);
    
    for (short i = index; i < list->size - 1; i++)
        list->list[i] = list->list[i+1];

    list->size--;
    list->list = (char**) realloc(list->list, list->size * sizeof(char*));

    return index;
}

static inline int binaryStringListDeleteSame(struct StringList *list, char *str) {
    int index = binaryStringListSearch(list, str);

    if (index < 0) 
        return -1;
    
    for (short i = index; i < list->size - 1; i++)
        list->list[i] = list->list[i+1];

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

    free(list->list);
    free(list);
}

static inline void freeStringListSame(struct StringList *list) {
    free(list->list);
    free(list);
}