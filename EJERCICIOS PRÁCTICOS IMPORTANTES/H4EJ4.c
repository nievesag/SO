#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <string.h>

void escribe(int n_hijo)
{
    int fd = open("output.txt", O_RDWR);

    int offset = 5 * n_hijo; // calculo del offset

    char * map = mmap(NULL, 
        (5*(5+1)),
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        0);

    if(map == MAP_FAILED)
    {
        perror("mmap() escribe");
        exit(1);
    }

    // el padre escribe 5 0s
    memset(map + offset, '0' + n_hijo, 5);

    close(fd);

    int sync = msync(map, 5*(5+1), MS_SYNC);

    if(sync == -1)
    {
        perror("msync()");
        exit(1);
    }

    munmap(map, 5*(5+1));
}

int main(int argc, char * argv[])
{
    int fd = open("output.txt", 
        O_CREAT | O_RDWR | O_TRUNC, 
        0664);

    if(fd == -1)
    {
        perror("open()");
        exit(1);
    }

    int trun = ftruncate(fd, (5*(5+1)));

    if(trun == -1)
    {
        perror("ftruncate()");
        exit(1);
    }

    char * map = mmap(NULL, 
        (5*(5+1)),
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        0);

    if(map == MAP_FAILED)
    {
        perror("mmap()");
        exit(1);
    }

    // el padre escribe 5 0s
    memset(map, '0', 5);

    close(fd);

    // el padre crea 5 hijos
    for(int i = 0; i < 5; i++)
    {
        pid_t pid = fork();

        switch (pid)
        {
        case -1: // error
            perror("fork()");
            exit(1);
            break;
        
        case 0: // hijos
            escribe(i+1);
            exit(i+1); // !!! PARA QUE LOS HIJOS NO CREEN MÁS HIJOS MUY IMPORTATE
            break;

        default: // padre
                // NO HACE NADA
            break;
        }
    }

    // espera la finalizacion de todos los hijos
    for(int i = 0; i < 5; i++)
    {
        int status;
        pid_t w = wait(&status);

        if(w == -1)
        {
            perror("wait()");
            exit(1);
        }

        if (WIFEXITED(status)) 
        {
            printf("El proceso hijo %i con PID %i terminó correctamente \n", WEXITSTATUS(status), w);
        }
    }

    int sync = msync(map, 5*(5+1), MS_SYNC);

    if(sync == -1)
    {
        perror("msync()");
        exit(1);
    }

    munmap(map, 5*(5+1));

    printf("Escritura completada en output.txt \n");

    return 0;
}