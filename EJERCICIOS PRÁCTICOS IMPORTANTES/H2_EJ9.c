// Escribir un programa que tenga un comportamiento similar a ls
// que mostrará el contenido de un directorio cuya ruta se proporciona como argumento. 
// -​ Comprobará que el argumento es un directorio y que tiene acceso con la llamada al sistema adecuada
// - Recorrerá las entradas del directorio y escribirá su nombre de fichero. Además:
//      - Si es un fichero regular y tiene permiso de ejecución para usuario, grupo u otros, escribirá el carácter * después del nombre.
//      - Si es un directorio escribirá el carácter / después del nombre.
//      - Si es un enlace simbólico, escribirá -> y el nombre del fichero enlazado obtenido con readlink(2)

// Nota: la variable d_name de las estructuras dirent solo contienen el nombre para obtener los atributos del archivo es necesario especificar la ruta completa concatenando el nombre del directorio, en el primer argumento.
// Para concatenar ambas cadenas, directorio y nombre del archivo, definir un buffer de tamaño PATH_MAX (#include <linux/limits.h>) y la llamada snprintf(3).

#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <linux/limits.h>
#include <unistd.h>


#define PATH_MAX 8192

int main (int argc, char *argv[])
{
    struct stat statbuf;

    int rc = lstat(argv[1], &statbuf);

    // gestion error
    if(rc == -1)
    {
        perror("stat()");
        return 1;
    }

    //comprobará que el argumento es un directorio 
    if(!S_ISDIR(statbuf.st_mode))
    {
        perror("no es directorio");
        return 1;
    }
    // y que tiene acceso con la llamada al sistema adecuada
    if(access(argv[1], R_OK | X_OK) == -1)
    {
        perror("no tienes acceso");
        return 1;
    }

    DIR* d = opendir(argv[1]); // abre el directorio
    struct dirent* dent;

    // readdir() function returns a pointer to a dirent structure representing the next directory entry in the directory stream pointed to by d.

    // Recorre las entradas del directorio
    while((dent = readdir(d)) != NULL) // es NULL si se acaba la busqueda o si ocurre un error
    {
//- Si es un fichero regular y tiene permiso de ejecución para usuario, grupo u otros, escribirá el carácter * después del nombre.
//      - Si es un directorio escribirá el carácter / después del nombre.
//      - Si es un enlace simbólico, escribirá -> y el nombre del fichero enlazado obtenido con readlink(2)

        // Escribe su nombre
        printf(dent->d_name);
        
        int rc = lstat(argv[1], &statbuf);

        // Es un archivo regular
        if(dent->d_type == DT_REG)
        {
            printf("*\n");
        }  

        else if(dent->d_type == DT_DIR)
        {
            printf("/\n");
        }

        else if(dent->d_type == DT_LNK)
        {
            printf("/\n");

            char origen[PATH_MAX];
			snprintf(origen, PATH_MAX, "%s/%s", argv[1], dent->d_name);

			char destino[PATH_MAX];
			if (readlink(origen, destino, PATH_MAX) == -1)
            {
				perror("readlink");
				continue;
			}
			printf("%s -> %s\n", origen, destino);
        }

        char path[PATH_MAX]; // buffer

        // para concatenar cadenas, agrega la ruta del directorio (argv[1]) al nombre del fichero (dent->d_name)
        // para poder hacer stat
        snprintf(path, PATH_MAX, "%s/%s", argv[1], dent->d_name);
    }

    // Además:
    //      - Si es un fichero regular y tiene permiso de ejecución para usuario, grupo u otros, escribirá el carácter * después del nombre.
    //      - Si es un directorio escribirá el carácter / después del nombre.
    //      - Si es un enlace simbólico, escribirá -> y el nombre del fichero enlazado obtenido con readlink(2)

    closedir(d);
    return 0;
}