#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    int seg_padre = atoi(argv[1]);
    int seg_hijo = atoi(argv[2]);

    // hijo crea su propia sesión
    pid_t pid = fork();

    switch(pid)
    {
        case -1: // gestion errores
            perror("fork");
        break;

        case 0: // hijo
            printf("[Hijo] PID=%i PPID=%i PGID=%i SID=%i Durmiendo %is\n", getpid(), getppid(), 
            getsid(getpid()), seg_hijo);
            sleep(seg_hijo);
            printf("[Hijo] terminado \n");
        break;

        default: // padre
            printf("[Padre] PID=%i PPID=%i PGID=%i SID=%i Durmiendo %is\n", getpid(), getppid(), 
            getsid(getpid()), seg_padre);
            sleep(seg_padre);
            printf("[Padre] terminado \n");
        break;
    }

    return 0;
}