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


    char data[BUFFER_SIZE];

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

        // buffer.elements < BUFFER_SIZE -> predicado
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

        pthread_cond_signal(&buffer.consume);
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