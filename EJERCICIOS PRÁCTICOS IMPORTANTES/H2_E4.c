#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <time.h>


// programa que emula el comportamiento de stat(1) 
// acepta un unico argumento que es la ruta del fichero del que se quiere obtener info, si no existe error

int main(int argc, char *argv[])
{
    struct stat statbuf; // no puede ser puntero => tiene que estar en la pila
    int rc = lstat(argv[1], &statbuf); // usar la llamada lstat con el primer argumento y statbuf

    // gestion de error
    if(rc == -1)
    {
        perror("stat()");
        return 1;
    }

    printf("Tipo: ");
    // TIPO DE FICHERO -> directorio / enlace simbólico / fichero regular
    if(S_ISDIR(statbuf.st_mode)) // directorio
    {
        printf("directorio\n");
    }
    else if(S_ISREG(statbuf.st_mode)) // fichero regular
    {
        printf("fichero regular\n");

        // mascaras binarias para comprobar si tiene permisos de ejecucion
        // logica bit a bit
        if(statbuf.st_mode & (S_IXGRP | S_IXUSR | S_IXOTH))
        {
            printf("ES EJECUTABLE\n");
        }
        else
        {
            printf("NO ES EJECUTABLE\n");
        }
    }
    else if(S_ISLNK(statbuf.st_mode))
    {
        printf("enlace simbolico\n");
    }

    printf("inodo: %ju\n", (uintmax_t) statbuf.st_ino);
    printf("Número de dispositivo: [%i,%i]\n", major(statbuf.st_dev), minor(statbuf.st_dev));
    printf("Último cambio de estado: %s\n", ctime(&statbuf.st_ctime)); // ctime -> change time
    printf("Último acceso: %s\n", ctime(&statbuf.st_atime)); // atime -> access time

    return 0;
}