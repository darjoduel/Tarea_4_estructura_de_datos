#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdbool.h>
#include "structures.h"

int FileExists(const char *filename){//verifica si un archivo existe
    struct stat buffer;
    return (stat (filename, &buffer) == 0);
}