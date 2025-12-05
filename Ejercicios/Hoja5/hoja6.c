// para crear ficheros v-> vim

// EJERCICIO 1
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct thread_info_t
{
    int num;
    pthread_t id;

} thread_info_t;

void *haz_thread(void*_info)
{
    thread_info_t * info = (thread_info_t*) _info; // casting al tipo

    printf("Thread %i sleeping...\n", info->num);
    sleep(info->num);

    return NULL;
}

int main(int argc, char *argv[])
{
    int nt = atoi(argv[1]);

    // reserva memoria para crear las pools
    thread_info_t * pool = (thread_info_t *) malloc(nt*sizeof(thread_info_t));

    for(int i = 0; i < nt ; i++)
    {
        pool[i].num = i;
        pthread_create(&pool[i].id, NULL, haz_thread, (void*) &pool[i]);
    }

    for(int i = 0; i < nt, i++)
    {
        pthread_join(pool[i].id, NULL); // sincroniza los threads
        printf("Thread %i termino\n", pool[i].num);
    }

    free(pool);

    return 0;
}

// EJERCICIO 2
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct thread_info_t
{
    int num;
    pthread_t id;

} thread_info_t;

void *haz_thread(void*_info)
{
    thread_info_t * info = (thread_info_t*) _info; // casting al tipo

    printf("Thread %i sleeping...\n", info->num);
    sleep(600); // para poder hacer el ejercicio mientras 

    return NULL;
}

int main(int argc, char *argv[])
{
    int nt = atoi(argv[1]);

    // reserva memoria para crear las pools
    thread_info_t * pool = (thread_info_t *) malloc(nt*sizeof(thread_info_t));

    for(int i = 0; i < nt ; i++)
    {
        pool[i].num = i;
        pthread_create(&pool[i].id, NULL, haz_thread, (void*) &pool[i]);
    }

    for(int i = 0; i < nt, i++)
    {
        pthread_join(pool[i].id, NULL); // sincroniza los threads
        printf("Thread %i termino\n", pool[i].num);
    }

    free(pool);

    return 0;
}

// ejecutar el codigo
// > gcc -o 2 2.c
// > ./2 4
//    hacer ctrl + z
// > bg
// ps -L -o pid ; -L -o tid ; -L -o tgid ; -L -o pgid ; -L -o cmd

// EJERCICIO 3
// Escribir un programa que realice la suma paralela de los N primeros números naturales
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct thread_info_t
{
    int num;
    pthread_t id;
    int tb; // tamaño de bloque

} thread_info_t;

typedef struct suma_p_t
{
    pthread_mutex_t mutex; // para proteger la variable compartida, para que no se sobrescriba
    int suma; // variable compartida suma

} suma_p_t;

suma_p_t suma = {PTHREAD_MUTEX_INITIALIZER, 0}; // suma global, aqui se inicializa el mutex

void *haz_thread(void*_info)
{
    thread_info_t * info = (thread_info_t*) _info; // casting al tipo

    int suma_parcial = 0;

    int ini = info->tb * info->num;
    int fin = (info->tb * info->num + 1); // del thread en el que estamos hasta antes del siguiente

    printf("[%i] sumando %i - %i\n", info->num, ini, fin);

    for(int i = ini; i < fin; i++)
    {
        suma_parcial += i;
    }

    suma.suma += suma_parcial; // aumentas la global suma

    sleep(info->num); // para que no sumen a la vez !! LOCK FREE woau
 
}

int main(int argc, char *argv[])
{
    int nt = atoi(argv[1]); // numero de threads
    int tb = atoi(argv[2]); // tamaño de bloque determina cuantos numeros sumara cada thread

    // reserva memoria para crear las pools
    thread_info_t * pool = (thread_info_t *) malloc(nt*sizeof(thread_info_t));

    for(int i = 0; i < nt ; i++)
    {
        pool[i].num = i;
        pool[i].tb = tb;

        pthread_create(&pool[i].id, NULL, haz_thread, (void*) &pool[i]);
    }

    for(int i = 0; i < nt; i++)
    {
        pthread_join(pool[i].id, NULL); // sincroniza los threads
        printf("Thread %i termino\n", pool[i].num);
    }

    int n = (nt * tb) - 1;
    printf("Suma total: %i / %i\n", suma.suma, (n*(n+1)/2));

    free(pool);

    return 0;
}

// EJERCICIO 4
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 10
#define P_ELEMENTS 5

typedef struct thread_info_t
{
    int num;
    pthread_t id;

} thread_info_t;

typedef struct _buffer_t {

    pthread_mutex_t mutex;

    // elements < BUFFER_SIZE
    pthread_cond_t produce; // los consumidores cuando dejen espacio en el buffer le diran a los productores: "Produce!"

    // elements > 0
    pthread_cond_t consume; // los productores cuando dejen espacio en el buffer le diran a los consumidores: "Consume!"


    int data[BUFFER_SIZE];

    int in;
    int out;

    int elements;

} _buffer_t;

_buffer_t buffer = {PTHREAD_MUTEX_INITIALIZER,  // mutex
                    PTHREAD_COND_INITIALIZER,   // produce
                    PTHREAD_COND_INITIALIZER,   // consume
                    0,                          // in
                    0,                          // out
                    0};                         // elements

void * productor(void*_info)
{
    thread_info_t * info = (thread_info_t*) _info; // casting al tipo

    for(int i = 0; i < P_ELEMENTS; ++i)
    { 
        pthread_mutex_lock(&buffer.mutex); // asegurarse que elements solo lo estoy modificando yo -> entramos en una region critica

        // buffer.elements < BUFFER_SIZE -> predicado "puedo producir"
        while(buffer.elements == BUFFER_SIZE) // la condicion se escribe justo al reves que el predicado
        {
            // esperar mientras no haya espacio en el buffer
            pthread_cond_wait(&buffer.produce, &buffer.mutex);
        }

        int item = 100 * info->num + i;

        // modificamos el recurso compartido ->
        // buffer circular
        buffer.data[buffer.in] = item;
        buffer.in = (buffer.in + 1) % BUFFER_SIZE;

        buffer.elements++;

        printf("P[%i] E:%i I:%i in:%i out:%i\n",
        info->num, buffer.elements, item, buffer.in, buffer.out);

        pthread_cond_signal(&buffer.consume); // despierta al consumidor
        pthread_mutex_unlock(&buffer.mutex);

        sleep(1);
    }

    return NULL;
}

void * consumidor(void * _info)
{
    thread_info_t * info = (thread_info_t*) _info; // casting al tipo

    while(1)
    { 
        pthread_mutex_lock(&buffer.mutex); // asegurarse que elements solo lo estoy modificando yo -> entramos en una region critica

        // buffer.elements > 0 -> predicado
        while(buffer.elements == 0) // la condicion se escribe justo al reves que el predicado
        {
            // esperar mientras no haya espacio en el buffer
            pthread_cond_wait(&buffer.consume, &buffer.mutex);
        }

        // si tuviesemos otra condicion total:
        // while(buffer.elements == 0 && total > 0)
        // comprobar si hemos salido por total o por buffer.elements
        // si quedasen les despertariamos con broadcast()


        // modificamos el recurso compartido ->
        // buffer circular
        int item = buffer.data[buffer.out];

        /*   
        // POISON PILL
        if (item == FIN) 
        {
            break;
        }
        */ 

        buffer.out = (buffer.out + 1) % BUFFER_SIZE; // avanzamos en uno el puntero de lectura

        buffer.elements--;

        printf("C[%i] E:%i I:%i in:%i out:%i\n",
        info->num, buffer.elements, item, buffer.in, buffer.out);

        pthread_cond_signal(&buffer.produce); // hemos dejado un hueco en el buffer -> se puede producir
        pthread_mutex_unlock(&buffer.mutex);

        sleep(2);
    }

    return NULL;
}

void main(int argc, char *argv[])
{
    int p = atoi(argv[1]);
    int c = atoi(argv[2]);

    thread_info_t * pool = (thread_info_t *) malloc((p+c)*sizeof(thread_info_t));

    for(int i = 0; i < p; ++i)
    {
        pool[i].num = i;
        pthread_create(&(pool[i].id),
        NULL, 
        productor,
        (void*) &pool[i]);
    }

    for(int i = 0; i < (p+c); ++i) 
    {
        pool[i].num = i - p;
        pthread_create(&(pool[i].id),
        NULL, 
        consumidor,
        (void*) &pool[i]);
    }

    for(int i = 0; i < (p+c); ++i)
    {
        pthread_join(pool[i].id, NULL);
        printf("Thread %i termino\n", pool[i].num);
    }

    free(pool);

    return 0;
}

// EJERCICIO 5 CON PILDORA EVENENADA
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 10
#define P_ELEMENTS 5

typedef struct thread_info_t
{
    int num;
    pthread_t id;

} thread_info_t;

typedef struct _buffer_t {

    pthread_mutex_t mutex;

    // elements < BUFFER_SIZE
    pthread_cond_t produce; // los consumidores cuando dejen espacio en el buffer le diran a los productores: "Produce!"

    // elements > 0
    pthread_cond_t consume; // los productores cuando dejen espacio en el buffer le diran a los consumidores: "Consume!"


    int data[BUFFER_SIZE];

    int in;
    int out;

    int elements;

} _buffer_t;

_buffer_t buffer = {PTHREAD_MUTEX_INITIALIZER,  // mutex
                    PTHREAD_COND_INITIALIZER,   // produce
                    PTHREAD_COND_INITIALIZER,   // consume
                    0,                          // in
                    0,                          // out
                    0};                         // elements

void * productor(void*_info)
{
    thread_info_t * info = (thread_info_t*) _info; // casting al tipo

    for(int i = 0; i < P_ELEMENTS; ++i)
    { 
        pthread_mutex_lock(&buffer.mutex); // asegurarse que elements solo lo estoy modificando yo -> entramos en una region critica

        // buffer.elements < BUFFER_SIZE -> predicado "puedo producir"
        while(buffer.elements == BUFFER_SIZE) // la condicion se escribe justo al reves que el predicado
        {
            // esperar mientras no haya espacio en el buffer
            pthread_cond_wait(&buffer.produce, &buffer.mutex);
        }

        int item = 100 * info->num + i;

        // modificamos el recurso compartido ->
        // buffer circular
        buffer.data[buffer.in] = item;
        buffer.in = (buffer.in + 1) % BUFFER_SIZE;

        buffer.elements++;

        printf("P[%i] E:%i I:%i in:%i out:%i\n",
        info->num, buffer.elements, item, buffer.in, buffer.out);

        pthread_cond_signal(&buffer.consume); // despierta al consumidor
        pthread_mutex_unlock(&buffer.mutex);

        sleep(1);
    }

    return NULL;
}

void * consumidor(void * _info)
{
    thread_info_t * info = (thread_info_t*) _info; // casting al tipo

    while(1)
    { 
        pthread_mutex_lock(&buffer.mutex); // asegurarse que elements solo lo estoy modificando yo -> entramos en una region critica

        // buffer.elements > 0 -> predicado
        while(buffer.elements == 0) // la condicion se escribe justo al reves que el predicado
        {
            // esperar mientras no haya espacio en el buffer
            pthread_cond_wait(&buffer.consume, &buffer.mutex);
        }

        // modificamos el recurso compartido ->
        // buffer circular
        int item = buffer.data[buffer.out];

        buffer.out = (buffer.out + 1) % BUFFER_SIZE; // avanzamos en uno el puntero de lectura

        buffer.elements--;

        printf("C[%i] E:%i I:%i in:%i out:%i\n",
        info->num, buffer.elements, item, buffer.in, buffer.out);

        pthread_cond_signal(&buffer.produce); // hemos dejado un hueco en el buffer -> se puede producir
        pthread_mutex_unlock(&buffer.mutex);

        // si era pildora salgo del while, pero la gestion del buffer anterior se sigue haciendo igual
        if(item == -1) 
        {
            break;
        }

        sleep(2);
    }

    return NULL;
}

void main(int argc, char *argv[])
{
    int p = atoi(argv[1]);
    int c = atoi(argv[2]);

    thread_info_t * pool = (thread_info_t *) malloc((p+c)*sizeof(thread_info_t));

    // CREAR LAS PILDORAS FUERA DE LA LOGICA DE PRODUCTORES Y CONSUMIDORES, mejor en el thread principal

    for(int i = 0; i < p; ++i)
    {
        pool[i].num = i;
        pthread_create(&(pool[i].id),
        NULL, 
        productor,
        (void*) &pool[i]);
    }

    for(int i = 0; i < (p+c); ++i) 
    {
        pool[i].num = i - p;
        pthread_create(&(pool[i].id),
        NULL, 
        consumidor,
        (void*) &pool[i]);
    }

    for(int i = 0; i < (c); ++i) // ESCRIBIR LAS PILDORAS
    {
        while(buffer.elements == BUFFER_SIZE)
        {
            pthread_cond_wait(&buffer.produce, &buffer.mutex);
        }

        buffer.data[buffer.in]= -1;
        buffer.in = (buffer.in + 1) % BUFFER_SIZE;

        pthread_cond_signal(&buffer.consume);
        pthread_mutex_unlock(&buffer.mutex);
    }

    for(int i = p; i < (p+c); ++i) // sincronizar los consumidores
    {
        pthread_join(pool[i].id, NULL);
        printf("Thread %i termino\n", pool[i].num);
    }

    free(pool);

    return 0;
}

// EJERCICIO 8
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define BUFFER_SIZE 10
#define P_ELEMENTS 5

typedef struct thread_info_t
{
    int num;
    pthread_t id;

} thread_info_t;

typedef struct _buffer_t {

    pthread_mutex_t mutex;

    // elements < BUFFER_SIZE
    pthread_cond_t produce; // los consumidores cuando dejen espacio en el buffer le diran a los productores: "Produce!"

    // elements > 0
    pthread_cond_t consume; // los productores cuando dejen espacio en el buffer le diran a los consumidores: "Consume!"


    char data[BUFFER_SIZE][PATH_MAX];

    int in;
    int out;

    int elements;

} _buffer_t;

typedef struct suma_t {
    pthread_mutex_t mutex;
    size_t  total;
}suma_t;

suma_t sumar = {PTHREAD_MUTEX_INITIALIZER, 0};

_buffer_t buffer = {PTHREAD_MUTEX_INITIALIZER,  // mutex
                    PTHREAD_COND_INITIALIZER,   // produce
                    PTHREAD_COND_INITIALIZER,   // consume
                    0,                          // in
                    0,                          // out
                    0};                         // elements

void escribe_buffer(char tipo, int id, const char* item) // le pasamos el item que queremos escribir
{
    // como el buffer es compartido -> usar los mecanismos de sincronizacion

     pthread_mutex_lock(&buffer.mutex); // asegurarse que elements solo lo estoy modificando yo -> entramos en una region critica

    // buffer.elements < BUFFER_SIZE -> predicado "puedo producir"
    while(buffer.elements == BUFFER_SIZE) // la condicion se escribe justo al reves que el predicado
    {
        // esperar mientras no haya espacio en el buffer
        pthread_cond_wait(&buffer.produce, &buffer.mutex);
    }

    // modificamos el recurso compartido ->
    // buffer circular
    strcpy(buffer.data[buffer.in], item); // copia en el buffer in el item
    buffer.in = (buffer.in + 1) % BUFFER_SIZE;

    buffer.elements++;

    printf("%c[%i] E:%i I:%i in:%i out:%i\n",
    tipo, id, buffer.elements, item, buffer.in, buffer.out);

    pthread_cond_signal(&buffer.consume); // despierta al consumidor
    pthread_mutex_unlock(&buffer.mutex);
}

void * productor(void*_path)
{
    char * path = (char*) _path; // casting al tipo

    char item[PATH_MAX];

    DIR * dir = opendir(path);

    struct dirent * dent;

    while((dent = readdir(dir)) != NULL) // la lee y la compara en la misma linea
    {
        sprintf(item, "%s/%s", path, dent->d_name); // crea el path ej: "/home/user/.bashrc"

        escribe_buffer('P', 0, item); // identificador es 0 porque solo vamos a tener un thread productor segun enunciado
    }

    return NULL;
}

void * consumidor(void * _info)
{
    thread_info_t * info = (thread_info_t*) _info; // casting al tipo
    char item[PATH_MAX];
    struct stat statbuf;

    while(1)
    { 
        pthread_mutex_lock(&buffer.mutex); // asegurarse que elements solo lo estoy modificando yo -> entramos en una region critica

        // buffer.elements > 0 -> predicado
        while(buffer.elements == 0) // la condicion se escribe justo al reves que el predicado
        {
            // esperar mientras no haya espacio en el buffer
            pthread_cond_wait(&buffer.consume, &buffer.mutex);
        }

        // si hay elementos que leer leo desde el buffer el que me toca
        strcpy(item, buffer.data[buffer.out]);
        buffer.out = (buffer.out + 1) % BUFFER_SIZE; // avanzamos en uno el puntero de lectura

        buffer.elements--;

        pthread_cond_signal(&buffer.produce); // hemos dejado un hueco en el buffer -> se puede producir
        pthread_mutex_unlock(&buffer.mutex);

        if(item[0] == '\0')
        {
            break;
        }

        stat(item, &statbuf);

        if(S_ISREG(statbuf.st_mode))
        {
            printf("C[%i]: %s - %i\n", info->num, item, statbuf.st_size);
            pthread_mutex_lock(&suma.mutex);
            sumar.total += statbuf.st_size;
            pthread_mutex_unlock(&suma.mutex);
    
        }

        sleep(2);
    }

    return NULL;
}

void main(int argc, char *argv[])
{
    //int p = atoi(argv[1]);
    int c = atoi(argv[2]);

    thread_info_t * pool = (thread_info_t *) malloc((c)*sizeof(thread_info_t));

    pthread_t p;
    pthread_create(&p, NULL, p, (void*) argv[1]);

    for(int i = 0; i < c; ++i)
    {
        pool[i].num = i+1;
        pthread_create(&(pool[i].id),
        NULL, 
        consumidor,
        (void*) &pool[i]);
    }

    pthread_join(productor,NULL);

    for(int i = 0; i < (p+c); ++i) 
    {
        pool[i].num = i - p;
        pthread_create(&(pool[i].id),
        NULL, 
        consumidor,
        (void*) &pool[i]);
    }

    for(int i = 0; i < (p+c); ++i)
    {
        pthread_join(pool[i].id, NULL);
        printf("Thread %i termino\n", pool[i].num);
    }

    free(pool);

    return 0;
}