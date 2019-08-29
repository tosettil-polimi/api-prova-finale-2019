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

void printReportObject(struct ReportObject *rep) {
    char c;
    int i = 0, j = 0;

    fputc('"', stdout);
    do {
        c = rep->relName[i];
        if (c != 0) fputc(c, stdout);
        i++;
    } while (c != 0);
    fputc('"', stdout);
    
    fputc(' ', stdout);

    for (i = 0; i < rep->names->size; i++) {
        if (rep->numComparse[i] == rep->maxRelNum) {
            fputc('"', stdout);
            do {   
                c = rep->names->list[i][j];
                if (c != 0) fputc(c, stdout);
                j++;
            } while(c != 0);
            fputc('"', stdout);
            fputc(' ', stdout);

            j = 0;
        }
    }

    char str[10];
    itoa(rep->maxRelNum, str);
    i = 1;

    c = str[0];

    while (c != 0) {
        fputc(c, stdout);
        c = str[i];
        i++;
    }

    fputc(';', stdout);
}