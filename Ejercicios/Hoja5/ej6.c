#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct thread_info_t
{
    int num;
    pthread_t id;

} thread_info_t;

typedef struct _shared_i_t 
{
    pthread_rwlock_t mutex;
    int valor;

} _shared_i_t;

_shared_i_t numero = {PTHREAD_RWLOCK_INITIALIZER, 0};

#define NUM_LECTURAS 5
#define NUM_ESCRITURAS 5


void *lector(void*_info)
{
    thread_info_t * info = (thread_info_t*) _info; // casting al tipo

    for(int i = 0; i < NUM_LECTURAS; ++i)
    {
        pthread_rwlock_rdlock(&numero.mutex); // mutex de lectura
        
        numero.valor++;

        printf("L[%i] %i\n", info->num, numero.valor); // L , thread, valor del recurso compartido
        usleep(100000); // microsegundos

        // desbloquear recurso
        pthread_rwlock_unlock(&numero.mutex); // mutex de lectura
    }

    return NULL;
}

void *escritor(void*_info)
{
    thread_info_t * info = (thread_info_t*) _info; // casting al tipo

    for(int i = 0; i < NUM_ESCRITURAS; ++i)
    {
        pthread_rwlock_wrlock(&numero.mutex); // mutex de lectura
        
        numero.valor++;

        printf("E[%i] %i\n", info->num, numero.valor); // L , thread, valor del recurso compartido
        usleep(100000); // microsegundos

        // desbloquear recurso
        pthread_rwlock_unlock(&numero.mutex); // mutex de lectura
    }

    return NULL;
}

void main(int argc, char *argv[])
{
    int le = atoi(argv[1]);
    int es = atoi(argv[2]);

    thread_info_t * pool = (thread_info_t *) malloc((le+es)*sizeof(thread_info_t));

    for(int i = 0; i < es; ++i)
    {
        pool[i].num = i;
        pthread_create(&(pool[i].id),
        NULL, 
        escritor,
        (void*) &pool[i]);
    }

    for(int i = 0; i < le; ++i) 
    {
        pool[i].num = i - es;
        pthread_create(&(pool[i].id),
        NULL, 
        lector,
        (void*) &pool[i]);
    }

    for(int i = 0; i< (es+le); ++i)
    {
        pthread_join(pool[i].id, NULL);
        printf("Thread %i termino\n", pool[i].num);
    }

    free(pool);

    return 0;
}