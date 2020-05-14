# Prova Finale di Algoritmi e Principi dell'Informatica - a.a. 2018-2019

Scopo del progetto è quello di implementare un social network "entità/relazione", ogni entità può avere e ricevere relazioni da altre entità.

## Build

### Compilazione
Per compilare il codice sorgente installare gcc sul proprio PC:
    - Windows: Scaricare ed installare il programma [__MinGW__](http://www.mingw.org/)
    - Ubuntu: eseguire da terminale ```sudo apt update && sudo apt install build-essential```
    - MacOS: installa XCode (buona fortuna) da questo [__link__](https://apps.apple.com/it/app/xcode/id497799835)
Una volta configurato gcc, eseguite da terminale il seguente comando:
```
sh build.sh
```

### Esecuzione
Per poter lanciare il programma basta eseguire da terminale il seguente comando:
```
./release
```

## Comandi
I comandi presenti nel programma sono i seguenti:
- `addent "pippo"`: aggiunge un entità (o utente);
- `addrel "amico" "pippo" "pluto"`: collega l'entità "pluto" con l'entità "pippo" con la relazione dal nome "amico", la relazione non è biunivoca;
- `delent "pippo"`: elimina l'entità "pippo" e tutte le relazioni ad esso collegate;
- `delrel "amico"`: elimina la relazione "amico";
- `report`: stampa a video, per ogni relazione, l'entità che riceve più relazioni e il numero delle stesse;

## Sviluppo
### HashMap & Linked List
Il progetto è stato sviluppato con due strutture dati principali ovvero HashMap & Linked List.
#### StringList
La struttura StringList viene utilizzata ovunque serva una lista di stringhe all'interno delle HashMap; viene usata come lista dei nomi delle relazioni o lista dei nomi delle entità:
```c
struct StringList {
    int size;
    char **list;
};
```
Relativamente alla StringList, abbiamo diverse funzioni che ci permettono di utilizzarla quali la add, la remove, la search e la order.
Per ulteriori dettagli visita [__stringlist.c__](https://github.com/tosettil-polimi/api-prova-finale-2019/blob/master/stringlist.c)
#### Relationships
HashMap delle relazioni di un entità, al sui interno troviamo (per ogni relazione "key"), tutte le altre entità collegate:

```c
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

```
#### Entities
Di seguito la HashMap delle entità:
```c
struct Entity {
    char *name; // nome dell'entità
    struct Relationships *relationships; // hashmap delle relazioni che l'entità riceve
};

struct Entities *e;

struct EntityNode {    
    struct Entity *kv; /* key: name, val: relationships */
    struct EntityNode *next;
};

struct Entities {  // hashmap delle entità, dimensione statica, senza re-hash, per ottimizzare la velocità
    int size;   // grandezza della hashmap (max SIZE_INIT_GENERAL)
    int indexes[SIZE_INIT_GENERAL]; // list of used hashes (indexes of list)
    int indexesSize;
    struct EntityNode *list[SIZE_INIT_GENERAL];
    struct StringList *relations; 
};
```
## Credits
[__Lorenzo Tosetti__](https://github.com/tosettil-polimi)
