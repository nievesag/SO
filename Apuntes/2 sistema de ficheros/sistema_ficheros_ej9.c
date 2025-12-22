// --- Ejercicio 9 esbozado, Escribir un programa (mils)

#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    DIR * d = opendir(argv[1]);
    struct dirent * dent;

    while((dent = readdir(d)) != NULL)
    {
        // printf("---> %s\n", dent->d_name); // a esto le falta la ruta
        char path[8192];
        snprintf(path,8192,"%s/%s", argv[1], dent->d_name); // para concatenar cadenas, agrega la ruta (atributo de entrada) al nombre del fichero, para poder hacer stat
    }


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
        printf("%*\n", path); 
    }
    else 
    {
        printf("%-\n", path);
    }

    closedir(d);
    return 0;    
}