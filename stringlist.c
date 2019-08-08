#include "libandconst.h"

struct StringList {
    int size;
    char **list;
};

int binaryStringListAdd(struct StringList *list, char *str) {
    int added = 0, cmp, i, j;

    for (i = 0; i < list->size; i++) {
        cmp = strcmp(str, list->list[i]);

        if (cmp < 0) {

            if (list->list == NULL) // if list is empty
                list->list = (char**) malloc(sizeof(char*));
            else 
                list->list = (char**) realloc(list->list, list->size * sizeof(sizeof(char*)) + sizeof(char*));
            
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
            return 0;
        }
    }

    if (!added) {
        list->list = (char**) realloc(list->list, list->size * sizeof(sizeof(char*)) + sizeof(char*));
        list->list[list->size] = (char*) malloc(sizeof(char) * MAX_STR);
        strcpy(list->list[list->size], str);
        list->size++;
    }

    return 1;
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