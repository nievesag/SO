#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <pthread.h>

typedef struct _thread_info_t
{
    pthread_t thread_id;        /* ID returned by pthread_create() */
    int       thread_num;       /* Application-defined thread # */
    int tb;                     /* Tamaño de bloque # */

} thread_info_t;

typedef struct _suma_t
{
    pthread_mutex_t mutex;        /* Mutex */
    int suma;                     /* Variable compartida suma # */
} suma_t;

suma_t suma = {PTHREAD_MUTEX_INITIALIZER, 0};

void *crea_thread(void* _info)
{
    thread_info_t * info = (thread_info_t*) _info; // casting al tipo


    int ini = info->tb * info->thread_num;
    int fin = (info->thread_num+1) * info->tb;

    int suma_parcial = 0; // usar una suma parcial para usar menos tiempo el mutex

    for(int i = ini; i < fin; i++)
    {
        suma_parcial += i;
    }

    pthread_mutex_lock(&suma.mutex);
    
    suma.suma += suma_parcial;

    pthread_mutex_unlock(&suma.mutex);

    // sleep(info->thread_num); // para que no sumen a la vez si no quisiese usar mutex
}

int main(int argc, char * argv[])
{
    int NUM = atoi(argv[1]);
    int BLOCK_SIZE = atoi(argv[2]);

    thread_info_t * pool = (thread_info_t *) malloc(NUM * sizeof(thread_info_t));

    for(int i = 0; i < NUM; i++)
    {
        pool[i].tb = BLOCK_SIZE;
        pool[i].thread_num = i;


        int s = pthread_create(&pool[i].thread_id,
                                NULL,
                                &crea_thread, // metodo con el que empieza
                                &pool[i]);    // argumentos del metodo

        if(s != 0)
        {
            perror("pthread_create()");
            exit(1);
        }
    }

    // Sincroniza los threads
    /* Now join with each thread, and display its returned value. */
    for (int i = 0; i < NUM; i++) 
    {
        int s = pthread_join(pool[i].thread_id, NULL);

        if(s != 0)
        {
            perror("pthread_join()");
            exit(1);
        }
    }

    int n = (NUM * BLOCK_SIZE) - 1;
    int suma_total = (n * (n + 1) / 2);
    printf("Suma total es: %i / Comprobación: %i \n", suma.suma, suma_total);

    free(pool);      /* Free memory allocated by thread */

    return 0;
}