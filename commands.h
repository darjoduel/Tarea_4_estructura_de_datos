#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "functions.h"

int pvv_create(int size){
    FILE *data = fopen("graphdata", "w");
    if(!data){
        return 1;
    }

    fprintf(data, "tamano:%d\n", size);
    fclose(data);

    return 0;
}

