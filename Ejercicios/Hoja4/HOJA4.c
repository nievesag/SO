# Ejercicio 1:

include <stdio.h> 
include <unistd.h> 

int num = 22;
int mul; 

 const char *msg = "El resultado es:\n"; 

 int main(void) 
 { 
    static int factor = 2; 

    mul = num * factor; 

    printf("%s%i\n", msg, mul); 

    sleep(600); 


    return 0; 
}

/*
            CÓDIGO C (A QUÉ CORRESPONDE EN EL PROGRAMA) | OFFSET | DIR VIRTUAL | FLAGS
.text ->    el main, el bucle principal | 0x001_000 |  0x001_000 | R E
.rodata ->  todas las constantes (const char msg) | 0x002_000 | 0x002_000 | R
.bss ->     variables sin inicializar (mul) | 2db0 | 3db0 | R W
.data ->    variables inicializadas (num, msg?, factor) | 2db0 | 3db0 | R W

(compilar primero etc)
> ./program  & echo $! // ejecuta y da el pid del programa
> cat /proc/<pid>/maps

            DIRECCIONES VIRTUALES | OFFSET DEL FICHERO | FLAGS | TIPO (MAP/ANÓNIMO) | RUTA
.text ->    (lo primero que sale, las direcciones en ese segmento) | 00001000 | r-xp | map (podemos ver la ruta) | /home/ubuntu/program
.rodata ->  | 00002000 | r--p | map | /home/ubuntu/program
.bss ->     | 00003000 | rw-p | map | /home/ubuntu/program
.data ->    | 00003000 | rw-p | map | /home/ubuntu/program
[heap] ->   | 00000000 | rw-p | anonimo | [heap]
[stack] ->  | 00000000 | rw-p | anonimo | [stack]
*/


/*
    > strings -tx /home/ubuntu/program 
(con grep para filtrar : > strings -tx /home/ubuntu/program grep "El resultado es")
*/

/*
Entry point -> 0x1080
debe estar en el offset 1000
buscar en maps ese offset y se tiene su direccion virtual (como lo hicimos en la tabla antes)

segmento de fichero: el offset en el que empieza + lo que ocupa (filesize) (?)
*/

/*
diapositiva 15 tema 4
*/

// [vdso] -> bibilioteca cargada en el kernel, lo que mas usa la guarda ahi para aligerar procesos
// [vvar] -> los datos que utiliza el [vdso]

#define CONSTANT 10 // rodata
int num1 = CONSTANT;  // data
int num2;  // bss
int main(int argc, char *argv[]) 
{ 
    int *i = malloc((sizeof(int)); // puntero, anonimo [heap]
    num2 = argc; 
    for (*i=0; *i<CONSTANT; *i=*i+1)
    {
        fprintf(stdout, "%s: %d, argc: %d\n", string, num1--, num2); 
    }

    return 0;
}

/*
// SOLO OCUPA ESPACIO EN EL EJECUTABLE SI ES UNA VARIABLE GLOBAL INICIALIZADA,
ocupa espacio en disco

Simbolo | Espacio en ejecutable (si/no) | segmento
CONSTANT| no | es una macro no existe en ningun sitio
i       | no | [stack]
num1    | si | .data
num2    | no | .bss
*/

// # Ejercicio 3
#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <sys/mman.h>
#include <string.h> 
 
// crea una region de memoria (mmap(2)) con:
// tamaño 1024 bytes
// acceso privado
// modo de lectura y escritura
int main(int argc, char *argv[]) 
{ 
    // crea direccion de memoria
    void *ptr = mmap(NULL, // 
                    1024, // 1k, 1024*size of char (como es size of char es 1 es solo 1024)
                    PROT_READ | PROT_WRITE,// permisos
                    MAP_PRIVATE | MAP_ANON,  // proteccion
                    -1, // fd
                    0); // offset
    
    // mirar el RETURN VALUE en el manual para control de errores
    if(ptr == MAP_FAILED)
    {
        perror("mmap()");
        return 1; // salida de error
    }

    // inicializarla con '\0'
    memset(ptr, 0, 1024);

    // mostrar pid del proceso y direccion del segmento
    printf("PID: %i, VMA: %p\n", getpid(), ptr);

    sleep(600); // para poder hacer el ejercicio mientras 
    return 0; 
} 

// ejecutar el codigo
// > gcc -o 3 3.c
// > ./3
//    hacer ctrl + z
// > bg
// > cat /proc/15983/maps

/*
// Usando el PID consultar el fichero maps e identificar el segmento creado en el espacio de memoria del proceso:
// buscar el puntero qeue imprimimos en map (o un rango que la contenga), tiene que tener los permisos que la hemos dado y ser privada y anonima por lo que no podemos ver la ruta
Direccion inicial | Dirección final | Offset | Flags
inicio rango | final del rango | 0000000 | rw-p
*/