#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <string.h>

#define size 1024 // region de memoria tamaño 1024 bytes

int main(int argc, char * argv[])
{
    void * map = mmap(NULL,
    size, 
    PROT_READ | PROT_WRITE,
    MAP_PRIVATE | MAP_ANON,
    -1,
    0);

    if(map == MAP_FAILED)
    {
        perror("mmap()");
        exit(EXIT_FAILURE);
    }

    memset(map, '\0', size);

    printf("PID: %i Dirección del segmento: %p \n", getpid(), map);

    sleep(600);

    return 0;
}

// Dirección inicial | Dirección final | Offset   | Flags
// 796e82442000      |    796e82445000 | 00000000 | rw-p