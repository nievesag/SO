#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

// implementa una version simplificada del comando dd

// recibirá 5 argumentos posicionales:
// input_file: archivo de entrada que se leerá, si es - se leerá la entrada estándar
// output_file: archivo de salida en el que se escribirá, si es - se escribirá en la salida estándar
// block_size: el número de bytes que se leerán o escribirán en cada llamada al sistema (entero, int)
// block_count: el número de bloques que se copiarán (entero, int)
// seek: número de bloques que se saltarán de la salida antes de escribir

// Notas:
// tratar los errores en las llamadas al sistema adecuadamente
// la función atoi(3) puede usarse para convertir cadenas de caracteres a ints
// el fichero se creará con los permisos rw- rw- r-- si no existe, en caso contrario se truncará el contenido
// el programa usará un buffer estático de 8192 bytes, si el tamaño del bloque indicado es mayor se usará el tamaño máximo del buffer como tamaño de bloque
// tratar adecuadamente el valor que devuelvan las llamadas read(2) y write(2), además considerar que la llamada read/write pueden no leer un bloque completo en la llamada 

int main(int argc, char *argv[])
{
    int in_fd, out_fd; // ficheros con los que vamos a trabajar

    in_fd = open(argv[1], O_RDONLY); // abre el archivo de entrada que se leerá en modo de solo lectura
    
    // gestión error
    if(in_fd == -1)
    {
        perror("open(in)");
        return 1;
    }

    out_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0664); // acceso solo lectura, si el fichero no existe se crea con los permisos en 0664 si existe se trunca

    // gestión error
    if(out_fd == -1)
    {
        perror("open(out)");
        return 1;
    }

    // LECTURA
    int blockSize = atoi(argv[3]); // el número de bytes que se leerán o escribirán en cada llamada
    int blockCount = atoi(argv[4]); // el número de bloques que se copiarán
    int seek = atoi(argv[5]); // número de bloques que se saltarán de la salida antes de escribir
    // On  files  that  support seeking, the read operation commences at the file offset, and the file offset is incremented by the number of bytes read.
    // If the file offset is at or past the end of file, no bytes are read, and read() returns zero.

    // si tiene seek
	if(seek > 0)
    {
        // salta seek bloques
	    lseek(out_fd, (off_t)seek * blockSize, SEEK_SET);
	}

    // buffer estático de 8192 bytes, si el tamaño del bloque indicado es mayor se usará el tamaño máximo del buffer como tamaño de bloque

    int BUFFER_SIZE = 8192;
    char buffer[BUFFER_SIZE];
    
    if(blockSize > BUFFER_SIZE) { blockSize = BUFFER_SIZE; }

    int total_bytes = 0; // para llevar la cuenta de los bytes que has leido

    while(total_bytes < blockSize) // mientras queden bytes que leer...
    {
        // read(descriptor del que queremos leer, buffer en el que guardar lo que hemos leído (saltándonos total_bytes), número de bytes a leer)

        int rc = read(in_fd, buffer + total_bytes, blockSize - total_bytes); // devuelve el numero de bytes leidos
        // falta gestionar seek

        if(rc == 0) { break; }

        total_bytes += rc;
    }

    // ESCRITURA
    int bytes_escritos = 0;
    while(bytes_escritos < total_bytes)
    {
        int wr = write(out_fd, buffer + bytes_escritos, total_bytes-bytes_escritos);

        // si no se ha escrito nada (ya no hay nada que escribir), sales
        if(wr == 0) { break; }

        printf("Copiados %i bloques de %i bytes\n", total_bytes, wr);
    }
    
    close(in_fd);
    close(out_fd);
    return 0;
}