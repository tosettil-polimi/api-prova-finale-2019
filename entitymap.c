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
            lastNode = lastNode;
        
        temp = temp->next;
    }

    if (lastNode == NULL) { // no collisioni, inserisco nella lista principale della hash
        lastNode = (struct EntityNode*) malloc(sizeof(struct EntityNode)); // dovrebbe essere uno nuovo, ma risparmio qualcosa di mem

        lastNode->kv = createEntity(name);
        lastNode->next = NULL;

        e->list[pos] = lastNode;
        
        e->indexes[e->indexesSize] = pos;
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

    for (i = 0; i < e->indexesSize; i++) {
        node = e->list[e->indexes[i]];

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
    binaryStringListAdd(relationsPresent, rel);
    struct Entity *ent = getEntityByName(ent1);
    return insertRelationEntity(ent, ent2, rel);
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

void readline(char *str) {
    char c;
    int i = 0;
    
    do {
        c = fgetc(stdin);
        str[i] = c;
        i++;
    } while (c != '\n');

    str[i] = 0;
}

int parseInput(char *s) {

    if (s[0] != '"') return 0;

    int index = 1;
    char temp[MAX_STR];
    
    while (s[index] != '"' && s[index] != 0) {
        temp[index - 1] = s[index];
        index++;
    }

    temp[index - 1] = 0;

    if (strcmp(temp, "addent") == 0) {

        return 1;
    }

    if (strcmp(temp, "addrel") == 0) {

        return 2;
    }

    if (strcmp(temp, "delent") == 0) {

        return 3;
    }

    if (strcmp(temp, "delrel") == 0) {

        return 4;
    }

    if (strcmp(s, "\"report\"") == 0) {

        return 5;
    }

    if (strcmp(s, "\"end\"") == 0) {
        return 6;
    }

    return 0;

}

void main() {
    int i, j, stop = 0;
    char input[130];

    do {
        readline(input);
        parseInput(input);
    } while (!stop);

    e = createEntities();
    relationsPresent = createStringList();

    addent("tose");
    addent("gio");
    addent("dio");
    addent("ciaone");
    
    addrel("tose", "gio", "amico");
    addrel("tose", "dio", "amico");
    addrel("tose", "dio", "bestemmia");
    addrel("gio", "dio", "bestemmia");
    addrel("dio", "ciaone", "dice");
    addrel("gio", "ciaone", "dice");
    addrel("ciaone", "tose", "dice");
    

    for (i = 0; i < e->indexesSize; i++) {
        struct Entity *ent = e->list[e->indexes[i]]->kv;
        struct RelationshipsNode *rel = relationshipsToArray(ent->relationships);

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

    freeEntities();

    /*
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
    */
}