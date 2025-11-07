#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <mman.h> 
#include <string.h> 
#include <sys/wait.h> 
#include <string.h> 

void write_char(void * _ptr, int proc)
{
    char * ptr = (char *) _ptr; // casting a char para poder escribirla

    for(int i = 0; i < 5; ++i)  // 5 procesos
    {
        // sabemos que cada proceso se empieza a escribir en indice del proceso (proc) por el tamaño (5)
        ptr[(proc*5)+1] = '0'+ proc; // escribe en el array
    }
}

int main(int argc, char *argv[]) 
{ 
    // Preparacion del segmento de memoria
    //  1. Abrir el fichero (patra poder mapearlo)
    //  2. tamaño = 30 bytes (el padre + 5 hijos, 6*5=30)
    //  3. Hacer el mmap de ese fichero, mapa compartido (porque voy a crear procesesos y 
    //   todos van a tener que escribir en esa region de memoria) y de rw (lectura y escrituta)
    
    // 0660 -> lectura y escrtitura para usuario y grupo y ninguno para otro
    int fd = open("salida.txt", O_RDWR | O_TRUNC | O_CREAT, 0660); 
    // falta COMPROBAR SI SE HA ABIERTO BIEN

    // ahora tenemos un fichero abierto de tamaño 0 (hacer trunc asegura que va a ser 0)

    ftruncate(fd,30);
    // comprobar si se ha truncado bien

    void *ptr = mmap(NULL, // 
                    30, // 1k, 1024*size of char (como es size of char es 1 es solo 1024)
                    PROTO_READ | PROTO_WRITE,// permisos, proteccion
                    MAP_SHARE, // no puede ser anonimo porque vamos a proyectar un fichero
                    fd, // descriptor del fichero
                    0); // offset desde el principio del fichero
    
    // mirar el RETURN VALUE en el manual para control de errores
    if(ptr == MAP_FAILED)
    {
        perror("mmap()");
        return 1; // salida de error
    }

    write_char(ptr,0); // padre ecribe

    // crea hijos
    for(int i = 1; i < 6; ++i)
    {
        // fork se hace despues porque antes queremos encribir en la misma region de memoria no que todos los hijos tengan su propia estructura file
        pid_t pid = fork(); // los hijos si quieren modificar la region de memoria compartida

        if(pid != 0) // si soy el padre creo el siguiente hijo
        {
            continue;
        }

        write_char(ptr,i); // hijo escribe

        exit(0); // el hijo ya no hace nada mas, sale
    }

    // espero a que terminen los 5 hijos
    for(int i = 0; i < 5; ++i)
    {
        int status;
        pid_t pid = wait(&status);; // pid del proceso que ha terminado, sacar en que estado ha terminado
        printf("HIJO: %i\n", status);
    }

    // Al final hay que sincronizar
    msync(ptr,30,MS_SYNC);

    // unmapear
    munmap(ptr,30);

    return 0; 
}

// ejecutar el codigo
// > gcc -0 4 4.c
// > ./4
// > ls
// > cat salida.txt