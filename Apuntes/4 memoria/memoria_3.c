#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <mman.h> 
#include <string.h> 
 
int main(int argc, char *argv[]) 
{ 
    void *ptr = mmap(NULL, // 
                    1024, // 1k, 1024*size of char (como es size of char es 1 es solo 1024)
                    PROTO_READ | PROTO_WRITE,// permisos, proteccion
                    MAP_PRIVATE | MAP_ANON,
                    -1,
                    0);
    
    // mirar el RETURN VALUE en el manual para control de errores
    if(ptr == MAP_FAILED)
    {
        perror("mmap()");
        return 1; // salida de error
    }

    memset(ptr, 0,1024);

    printf("PID: %i, VMA: %p/n", getpid(), ptr);

    sleep(6000);
    return 0; 
} 

// ejecutar el codigo
// > gcc -0 3 3.c
// > ./3
// bg
// cat /proc/15983/maps
