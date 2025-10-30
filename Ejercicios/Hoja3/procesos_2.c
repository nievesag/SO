> gcc -o mips mips.c # compilar el programa desde la shell
> ./mips
> cntrl + o => guardar

# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    // muestra los identificadores del proceso (PID, PPID, PGID y SID), 
    // el identificador del usuario y grupo, y su directorio de trabajo actual

    printf("PID: %i\n PPID: %i\n PGID: %i\n SID: %i\n USER: %i\n GROUP: %i\n DIR: %i\n", 
        getpid(), getppid(), getpgid(), getsid(), getuid(), getpgrp(), getcwd());
    
    return 0
}