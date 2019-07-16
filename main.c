#include "hashtable.c"

// Primo test di struttura dati
typedef struct {
    char *nome;
    struct Utente *relazioni;
} Utente;

void freeUtente(Utente *utente) {
    free(utente->nome);
    free(utente->relazioni);
}

void main() {
    Utente prova;
    struct Utente *utenti;
    
    prova.nome = malloc(sizeof(char) * 10);
    utenti = malloc(sizeof(Utente) * 100);

    prova.relazioni = utenti;

    printf("size: %d\n", sizeof(prova.relazioni));

    freeUtente(utenti);
}