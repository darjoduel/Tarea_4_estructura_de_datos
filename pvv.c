#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"

#define help 61964
#define start 743628
#define read 71842
#define all 5560
#define exit 60838
//#define

int CharToNum(char *str){// convierte cadena a numero entero positivo
    int num = atoi(str);
    if(num < 0){
        num*= -1;
    }
    return num;
}

int CharToInt(char *str){// convierte cadena a numero entero
    int result=0;
    
    while(*str){
        result = result * 10 + (*str - '0');
        str++;
    }
    return result;
}

int main(int argc, char *argv[]){

    int command = CharToInt(argv[1]);

    switch (command){
        case help:
            printf("1.-'./pvv start <numero>', para crear el grafo con el tamano ingresado\n");
            printf("2.-'./pvv read <archivo.txt>', para leer los enlaces desde el archivo texto ingresado\n");
            printf("3.-'./pvv all', para imprimir la representacion en uso del grafo actual\n");
            printf("4.-'./pvv exit', para eliminar las estructuras\n");
            break;
        case start:
            int size = CharToNum(argv[2]);
            if( size < 4 || pvv_create(size) == 1){
                printf("Error. No se pudo crear el grafo\n");
                printf("Intente ingresando un numero mayor a 4\n");
            }
            else{
                printf("Grafo creado con %d nodos\n", size);
            }
            break;
        case read:
            if(!FileExists(argv[2])){
                printf("El archivo %s no existe\n", argv[2]);
            }
            break;
        case all:
            break;
        case exit:
            break;
        default:
            printf("Comando no reconocido. Ingrese './pvv help' para conocer los comandos\n");
            break;
    }
    //printf("%d", command);

    return 0;
}