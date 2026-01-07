#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int global;

void main(int argc, char * argv[]) 
{
    int local = 3;
    pid_t pid;

    global = 10;

    pid = fork();

    if (pid == 0) 
    {
        global = global + 5;
        local = local + 5;

        printf("HIJO\n");
    }
    else 
    {
        wait(NULL);

        global += 10;
        local += 10;

        printf("PADRE\n");
    }

    printf("global:%d local:%d\n", global, local);
}

/*
IMPRIME:
HIJO
global:15 local:8
PADRE
global:20 local:13
*/