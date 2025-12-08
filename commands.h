#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"

// declaración para evitar inclusión circular
int FileExists(const char *filename);

int pvv_create(int size){
    FILE *data = fopen("graphdata", "w");
    if(!data){
        return 1;
    }

    fprintf(data, "tamano:%d\n", size);
    fclose(data);

    return 0;
}

int pvv_read(const char* filename) {
    if (!FileExists(filename)){
        return 1;
    }

    FILE *data = fopen("graphdata", "r");
    if (!data){
        printf("Error al abrir graphdata\n");
        return 1;
    }

    int size;

    if (fscanf(data, "tamano:%d\n", &size) != 1){
        printf("Error al leer tamano\n");
        fclose(data);
        return 1;
    }

    fclose(data);

    data = fopen("graphdata", "w");
    if (!data){
        printf("Error al reabrir graphdata\n");
        return 1;
    }

    fprintf(data, "tamano:%d\n", size);
    fprintf(data, "fuente:%s\n", filename);

    fclose(data);

    return 0;
}
