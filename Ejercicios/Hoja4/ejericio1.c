#include <>

int num = 22; // . variable global definida
int mul; // . variable global no definida

const char *msg = ""; // .rodata variable de memoria definida

// ... falta el ejercicio en si

// > readelf -l ejercicio1 // program headers, section to segment mapping
 // ejemplo:
 // 04 .rotada -> es el header numero 4
 // R (read) offset 0x1000
 // > ./ejercicio1 &
 // > ls
 // maps tiene la lista de areas de memoria virtual que tiene este programa
 // map_files permite consultar el contenido
 // > cat maps
 // primera columna:direcciones virtuales
 // segunda columna:flags (R W X P)
 // tercera columna:offset
 // cuarta columna: major y minor te dice donde esta el dispositivo para poder acceder al super bloque etc
 // quinta columna: inodo
 // sexta columna:  path, cuando no hay path es que solo estan en memoria no es disco (no tiene major ni minor(ningún dispositivo/fichero les respalda, no se pueden sincronizar) ni inodo)

 // > sudo strings (direccion de rodata por ej)


 // [vvar] [vdso] para que sirven estas regiones de memoria???