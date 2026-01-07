#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int a = 3;

void main(int argc, char * argv[]) 
{
    int b=2;
    for (int i = 0; i < 4; i++) 
    {
        pid_t p = fork();

        printf("p:%i\n", p);

        if (p == 0) 
        {
            printf("hijo %i entra\n", i);
            b++;
            execlp("/usr/bin/sleep", "/usr/bin/sleep", "2", NULL);

            printf("hijo %i duerme\n", i);

            a++;
        }
        else 
        {
            printf("padre %i entra\n", i);

            wait(NULL);
            a++;
            b--;
        }
    }

    printf("variables - a: %d b: %d\n", a, b);
}