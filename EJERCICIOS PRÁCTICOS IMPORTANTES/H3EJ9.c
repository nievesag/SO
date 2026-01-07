#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/wait.h>


int main (int argc, char * argv[])
{
    pid_t pid = fork();

    char* file = argv[1];

    switch (pid)
    {
    case -1: // error
        perror("fork()");
        exit(1);
        break;

    case 0: // hijo
        int rc = execvp(file, &argv[1]);

        if (rc == -1)
        {
            perror("execvp()");
        }
        break;
    
    default: // padre
        int status;
        pid_t w = waitpid(pid,&status,0);

        if (w == -1) 
        {
            perror("waitpid()");
            exit(1);
        }

        if (WIFEXITED(status)) 
        {
            printf("El proceso hijo terminó con el código de salida %i\n", WEXITSTATUS(status));
        }

        else if (WIFSIGNALED(status)) 
        {
            printf("El proceso hijo terminó por señal %i \n", WTERMSIG(status));
        }

        break;
    }
    
    return 0;
}