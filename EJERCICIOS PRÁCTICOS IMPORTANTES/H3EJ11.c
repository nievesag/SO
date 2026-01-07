#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

void escribe(int n_hijo)
{
    int fd = open("output.txt", O_WRONLY);

    char buffer[5];

    for(int i = 0; i < 5; i++)
    {
        buffer[i] = '0' + n_hijo;
    }

    int offset = 5 * n_hijo;

    // COLOCA EL OFFSET
    lseek(fd, offset, SEEK_SET);

    int w = write(fd, buffer, 5);

    if(w == -1)
    {
        perror("write()");
        exit(1);
    }

    close(fd);
}

int main(int argc, char * argv[])
{
    int fd = open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0664);

    if(fd == -1)
    {
        perror("open()");
        exit(1);
    }

    int wr = write(fd, "00000", 6);
    close(fd);

    if(wr == -1)
    {
        perror("write()");
        exit(1);
    }

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

    printf("Escritura completada en output.txt \n");

    return 0;
}