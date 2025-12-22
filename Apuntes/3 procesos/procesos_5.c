
# include <sys/types.h>
# include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    pid = fork();

    switch (pid) {
        case -1:
            perror("fork");
            exit(1);
        case 0: // hijo
            printf("Hijo: %i (padre: %i)\n", getpid(), getppid());
            sleep(600);
            break;
        default: // padre
            printf("Padre: %i (hijo: %i)\n", getpid(), pid);
            break;
    }
    return 0
}