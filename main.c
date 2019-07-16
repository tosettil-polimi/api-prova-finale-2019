#include "hashtable.c"
#include "utente.c"

// Primo test di struttura dati

void main() {
    struct GeneralHashTable *hashTable = createTableGeneral(100);

    struct Utente prova;
    struct Utente *utenti;
    
    prova.nome = malloc(sizeof(char) * 10);
    utenti = malloc(sizeof(struct Utente) * 100);

    prova.relazioni = utenti;

    printf("size: %lu\n", sizeof(*(prova.relazioni)));

    freeUtente(utenti);
    free(prova.nome);
}