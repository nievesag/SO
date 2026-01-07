#include <stdio.h>
#include <unistd.h>

int num = 22; // .data
int mul; // .bss

const char *msg = "El resultado es:\n"; // .rodata

// .text
int main(void)
{
    static int factor = 2; // .data
    mul = num * factor;

    printf("%s%i\n", msg, mul);

    sleep(600);
    return 0;
}

/*
Segmento | Código C correspondiente                    | Offset             | Dir. Virtual       | Flags
.text    | todo lo que esté en el main que no va a otro| 0x0000000000001000 | 0x0000000000001000 | R E
.rodata  | const char *msg , "%s%i"                    | 0x0000000000002000 | 0x0000000000002000 | R
.bss     | mul                                         | 0x0000000000002db0 | 0x0000000000003db0 | R W
.data    | factor, num, *msg                           | 0x0000000000002db0 | 0x0000000000003db0 | R W
*/

/*
Segmento | Rango de direcciones virtuales | Offset del fichero | Flags | Tipo map/anónimo | Ruta fichero
.text    |    5780ee57f000-5780ee580000   | 00001000           | r-xp  |     map     |  /home/cynthia/H4EJ1
.rodata  | 5780ee580000-5780ee581000      | 00002000           | r--p  |     map     |  /home/cynthia/H4EJ1
         | 5780ee581000-5780ee582000      |
.bss     | 5780ee582000-5780ee583000      | 00003000           | rw-p  |     map     |  /home/cynthia/H4EJ1
.data    | 5780ee582000-5780ee583000      | 00003000           | rw-p  |     map     |  /home/cynthia/H4EJ1
[heap]   | 57812a5af000-57812a5d0000      | 00000000           | rw-p  |  anónimo    | [heap]
[stack]  | 7ffd06e1c000-7ffd06e3d000      | 00000000           | rw-p  |  anónimo    | [stack]
*/

/*
Cuestiones:
1) En qué segmento(s) de memoria virtual está la cadena "El resultado es:\n". Los contenido del segmento de memoria virtual se puede acceder en el directorio del proceso 
/proc/<pid proceso>/map_files/<rango del segmento>. Comprobar que la respuesta es correcta con el comando strings(1).
// En el 5780ee580000-5780ee581000
// con strings -tx ./H4EJ1
// segmento 2004

2) Segmentos de fichero y de memoria
FICHERO: virual dir + file size = 0x0000000000000000 + 0x0000000000000678
MEMORIA: 5780ee57e000-5780ee57f000

3) Esquema de memoria, teoría

4)
vdso -> virtual elf dynamic shared object: donde está el código optimizado para las llamadas al kernel
vvar -> virtual variables: variables auxiliares que usa el vdso
*/