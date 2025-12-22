// --- Ejercicio 4 esbozado, Escribir un programa (mistat) que emule el comportamiento de stat(1). 
// El programa aceptará un único argumento que será la ruta del fichero del que se quiere obtener la información. 
// Si el fichero no existe se informará del error. La información del fichero será: 
// ● El número major y minor asociado al dispositivo, ver major(3)/minor(3). 
// ● El número de inodo del fichero. 
// ● El tipo de fichero (únicamente considerar directorio, enlace simbólico o fichero ordinario). 
// ● La hora en la que se accedió el fichero por última vez. ¿Qué diferencia hay entre st_mtime y st_ctime?

// recomienda leer man stat para saber bien cómo funciona (man 2 stat) (man 3type stat) (man 7 inode)
// if(man st mode && S_IFREG) // si devuelve 0 si es si no no... ver ejemplos de mask en man 7 inode

#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    struct stat statbuf; // no puede ser un puntero porque tiene que estar en la pila
    int rc = lstat(argv[1], &statbuf); // USAR ESTA LLAMADA, en un examen habría que saber qué llamada de stat usar

    // gestión de error
    if(rc == -1)
    {
        perror("stat()")
        return 1; // != de 0 es llamada no exitosa, sale con el código de error asociado
    }

    // es directorio
    if(S_ISDIR(statbuf.st_mode))
    {
        printf("%/\n", argv[1]);
    }
    // es regular
    else if(S_ISREG(statbuf.st_mode))
    {
        // & logica bit a bit -> CONSULTAR APUNTES PARA EXPLICACIÓN BIT A BIT
        // comprueba si tienes permisos de ejecucion
        if (statbuf.st_mode & (S_IXUSR | S_IXGRP | S_XOTH)) // mascaras binarias...
        {
            printf("%*\n", argv[1]); // no es ejecutable
        }
        else
        {
            printf("%-\n", argv[1]); // es ejecutable
        }
    }

    return 0;
}

