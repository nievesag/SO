
# include <sys/types.h>
# include <sys/wait.h>

// ./9 ls -l -a // queremos tipo lista
// execvp(argv[1],argv+1) // exec + v (vector) + p (path)
// o
// execvp(argv[1],&argv[1]) // argv+1 es igual que &argv[1] => la direccion de la primera posicion

int main(int argc, char *argv[])
{
    pid_t pid;
    pid = fork();

    switch (pid) 
    {
        // error
        case -1:
            perror("fork");
            exit(1);
        break;

        // hijo
        case 0: 
            printf("Hijo: %i (padre: %i)\n", getpid(), getppid());
            rc = execvp(argv[1],&argv[1]);

            printf("NO SALE NADA");

            if(rc == -1)
            {
                perror("execvp()");
            }

            return 1;
        break;

        // padre
        default: 
            printf("Padre: %i (hijo: %i)\n", getpid(), pid);
        break;
    }


    int s; // status
    rc = waitpid(0, &s, 0); // 0 o pid es lo mismo

    if (WIFEXITED(s))
    {
        printf("Hijo terminó con código %i\n", WAITSTATUS(s));
    }
    else if (WIFSIGNALE(s))
    {
        printf("Hijo terminó con señal %i\n", WTERMSIG(s));
    }

    return 0
}