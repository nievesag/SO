// 11 y 12
// 12) No, si usamos el mismo descriptor comparten puntero de escritura y al hacer seek cada uno escribe por un lado etc

# include <sys/types.h>
# include <sys/wait.h>

#define NUM 5 // numero de veces a imprimir fijo

// escribe el inidce en el ficher un numero de veces
// desde el punto de vista del sistema el fichero abierto es un descriptor -> le pasamos el descriptor donde vamos a escribir
void imprimir_indice(int fd, int indice) // pasamos descriptor (donde escribir) e indice (que escribir)
{
    char buffer[NUM];

    for(i = 0; i < NUM; i++)
    {
        buffer[i] = '0' + indice; // escribe el char asociado al indice
    } // array con los NUM caracteres que tienen que ir al fichero

    /*
    // otra forma de hacerlo...
    char buffer[NUM+1];
    sprintf(buffer, NUM, "%d", 11111*indice);
    */

    write(fd, buffer, NUM*sizeof(char)); // man(2) write para saber que pasarle
}

int main(int argc, char *argv[])
{
    pid_t pid;
    int rc;

    // man(2) open
    int fd = open("./salida.txt", 
                O_WRONLY | O_CREAT | O_TRUNC,
                0660); // permisos

    // gestion de errores
    if(fd == -1)
    {
        // ...
    }

    imprimir_indice(fd,0);

    // empieza en uno para usar el indice directamente
    for(int indice=1; i < 6; indice++) // crea 6 hijos
    {
        pid = fork(); // crea hijo

        if(pid != 0) // si soy el padre pasamos
        {
            continue; // paso al siguiente hijo
        }

        fd = open("./salida.txt", O_ONLY);

        // -- esto ya lo hace el hijo que acabas de crear
        // desplazarse hasta donde le toque escribir
        lseek(fd, NUM*indice, SEEK_SET); // el fichero fd se desplaza NUM*indice bytes desde el principio

        srandom(indice);
        int sec = random() % 2;
        sleep(sec);

        imprimir_indice(fd,indice); // una vez te has movido puedes escribir

        close(fd);

        return indice; // para que los hijos no creen hijos, salir del programa
    }

    // el padre espera a que acabe los demás hijos, cada vez que termina un hijo imprime su código de salida
    for(int i = 0; i < 5 i++) // 5 hijos
    {
        int s; 
        waitpid(0, &s, 0);
        if(WIFEXITED(s))
        {
            printf("[%i] indice: %i\n", WEXITSTATUS(s));
        }
    }

    return 0
}