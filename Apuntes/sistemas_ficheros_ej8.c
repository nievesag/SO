> dd if=/dev/urandom of=./rand bs=512 bc=4

// --- Ejercicio 4 esbozado, Implementar una versión simplificada del comando dd. El programa recibirá 5 argumentos posicionales, en: 
// ● input_file: El archivo de entrada del que se leerá. Si el archivo es ‘-’ se leerá de la entrada estándar. 
// ● output_file: El archivo de salida en el que se escribirá. Si el archivo es ‘-’ se escribirá en la salida estándar. 
// ● block_size: El número de bytes que se leerán o escribirán en cada llamada al sistema. (entero, int)  
// ● block_count: El número de bloques que se copiarán (entero, int) 
// ● seek: Número de bloques que se saltarán de la salida antes de escribir. 

// Notas sobre la implementación: 
// ● Tratar los errores en las llamadas al sistema adecuadamente. Asumir que el programa siempre se ejecuta con 5 argumentos. 
// ● La función atoi(3) puede usarse para convertir una cadena de caracteres a un entero. 
// ● El fichero de salida se creará (con permisos rw- rw- r--) si no existe, en caso contrario se truncará el contenido. 
// ● El programa usará un buffer estático de 8192 bytes. Si el tamaño de bloque indicado es mayor se usará el tamaño máximo del buffer 
//   como tamaño de bloque. 
// ● Tratar adecuadamente el valor que devuelven las llamadas read(2) y write(2). Además considerar que la llamada read()/write() 
//   pueden no leer un bloque completo en la llamada.

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int in_fd, out_fd; // ficheros sobre los que vamos a trabajar

    in_fd = open(argv[1], O_RDONLY); // abrir el primer fichero que pases (parámetro posicional 1o argv[1])

    if(in_fd == -1)
    {
        perror("open() in");
        return 1;
    }

    out_fd = open(argv[2], O_WRONLY, O_CREAT, O_TRUNC, 0444); // write only | crearlo si no existe | truncar | permisos (concatenar S_IWUSR ect o poner el ocatl correspondiente 0444 como ejemplo)

    if(out_fd == -1)
    {
        perror("open() out");
        return 1;
    }

    // LECTURA
    int bs = atoi(argv[3]); // tamaño de bloque, atoi para pasar a int
    int count = atoi(argv[4]); 

    char buffer[8192]; // 8192 bytes -> array de CHAR!!

    int total_bytes = 0; // llevas la cuenta de los bytes totales que has leido

    // !! esto es solo para un bloque, habria que repetir esto para tantos bloques como sea necesario
    while(bytes < bs) // mientras el numero de bytes que has leído sea menor que el block size
    {
        int rc = read(in_fd, buffer + total_bytes, bs - total_bytes); // descriptor sobre el que queremos leer, buffer en el que queremos guardar lo que hemos leído (dir), tamaño de los bytes que queremos leer
        // si 0 ha llegado al final y se puede pasar a escribir
        // si es < de lo que querías leer no lo has leído entero

        // si bytes == 0 paras, si bytes <= bs seguir leyendo por donde te hayas quedado
        if(rc == 0) 
        {
            break;
        }

        total_bytes += rc; // vas llevando la cuenta de lo que vas leyendo
    }

    // una vez leido todo: ESCRITURA...
    
    // llamada al sistema -> cerrar los ficheros
    close(in_fd);
    close(out_fd);
}