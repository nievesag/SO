#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
 
int main(int argc, char *argv[]) 
{ 
    int factor    = atoi(argv[1]); // traduce a int

    int elementos = factor * 1024;  // k que queremos que reserve *1024 bytes

    printf("Inicializando región de memoria\n"); 

    int * ptr = (int *) malloc(elementos * sizeof(int)); 

    for (size_t i = 0; i < elementos; i++)
    { 
        ptr[i] = 1; 
    } 

    sleep(600); // para poder responder a las preguntas del enunciado
    return 0; 
} 

// > strace // traza las llamadas al sistema
// > cd/proc/numerodeproceso/maps
// > cat /proc/numerodeproceso/maps // muestra los mapas de memoria del proceso

// salen las direcciones de memoria, con sus flags, etc
