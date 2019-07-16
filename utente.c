struct Utente {
    char *nome;
    struct Utente *relazioni;
    int size;
};

void freeUtente(struct Utente *utente) {
    free(utente->nome);
    free(utente->relazioni);
    free(utente);
}

void addRel(struct Utente utente, struct Utente relazione) {
    utente.relazioni = (struct Utente*) realloc(utente.relazioni, sizeof(struct Utente) * (utente.size + 1));
    utente.relazioni[utente.size] = relazione;
    utente.size++;
}