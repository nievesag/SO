#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <pthread.h>

#define BUFFER_SIZE 10
#define NUM_ELEMS 5

typedef struct _thread_info_t
{
    pthread_t thread_id;        /* ID returned by pthread_create() */
    int       thread_num;       /* Application-defined thread # */
} thread_info_t;

typedef struct _buffer_t
{
    pthread_mutex_t mutex; // controla el acceso al recurso compartido

    pthread_cond_t produce; // produce!!!! 
    pthread_cond_t consume; // consume!!!! 

    int count;
    int in; // indice por donde vayas de lectura
    int out; // indice por donde vayas de escritura

    int data[BUFFER_SIZE];

} buffer_t;

buffer_t buffer = {
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_COND_INITIALIZER,
    PTHREAD_COND_INITIALIZER,
    0,
    0,
    0
};

void * produce(void* _info)
{
    thread_info_t * info = (thread_info_t*) _info; // casting al tipo

    // no produce todo el rato, produce un numero fijo de elementos
    
    for(int i = 0; i < NUM_ELEMS; i++)
    {
        pthread_mutex_lock(&buffer.mutex);

        // espero a poder producir
        while(!(buffer.count < BUFFER_SIZE))
        {
            pthread_cond_wait(&buffer.produce, &buffer.mutex);
        }

        // produzco
        int item = 100 * info->thread_num + 1;
        printf("Productor: [Num de thread] %i | [Item] %i \n", info->thread_num, item);

        buffer.data[buffer.in] = item;
        buffer.in = (buffer.in + 1) % BUFFER_SIZE;
        buffer.count++;

        pthread_cond_signal(&buffer.consume);
        pthread_mutex_unlock(&buffer.mutex);

        sleep(1);
    }

}

void * consume(void* _info)
{
    thread_info_t * info = (thread_info_t*) _info; // casting al tipo

    while(1) // consume todo el rato
    {
        pthread_mutex_lock(&buffer.mutex);

        // espero a poder consumir
        // predicado: puedo consumir mientras count > 0
        while(!(buffer.count > 0)) // 
        {
            pthread_cond_wait(&buffer.consume, &buffer.mutex);
        }

        // cuando salgo del while es que puedo consumir

        // para debug
        int item = buffer.data[buffer.out];
        printf("Consumidor: [Num de thread] %i | [Item] %i \n", info->thread_num,item);

        // buffer circular
        // mover el puntero de escritura
        buffer.out = ((buffer.out + 1) % BUFFER_SIZE); // dejas un huec
        buffer.count--;

        // dice: "PRODUCE!" ya que ha dejado un hueco en el buffer
        pthread_cond_signal(&buffer.produce);
        pthread_mutex_unlock(&buffer.mutex);

        if(item == -1)
        {
            break;
        }

        sleep(2);
    }
}

int main(int argc, char* argv[])
{   
    int p = atoi(argv[1]);
    int c = atoi(argv[2]);

    thread_info_t * pool = (thread_info_t *) malloc((p+c) * sizeof(thread_info_t));

    for(int i = 0; i < p; i++) // creas productores
    {
        pool[i].thread_num = i;

        int s = pthread_create(&pool[i].thread_id,
                                NULL,
                                &produce, // metodo con el que empieza
                                &pool[i]);    // argumentos del metodo

        if(s != 0)
        {
            perror("pthread_create()");
            exit(1);
        }
    }

    for(int i = p; i < p+c; i++) // creas consumidores
    {
        pool[i].thread_num = i;

        int s = pthread_create(&pool[i].thread_id,
                                NULL,
                                &consume, // metodo con el que empieza
                                &pool[i]);    // argumentos del metodo

        if(s != 0)
        {
            perror("pthread_create()");
            exit(1);
        }
    }

    // sincronizar productores
    for (int i = 0; i < p; i++) 
    {
        int s = pthread_join(pool[i].thread_id, NULL);

        printf("Thread productor %i terminó\n", pool[i].thread_num);

        if(s != 0)
        {
            perror("pthread_join()");
            exit(1);
        }
    }

    printf("Se han producido todos los elementos \n");

    for(int i = 0; i < c; i++)
    {
        pthread_mutex_lock(&buffer.mutex);

        // espero a poder producir
        while(!(buffer.count < BUFFER_SIZE))
        {
            pthread_cond_wait(&buffer.produce, &buffer.mutex);
        }

        // produzco PILDORA
        buffer.data[buffer.in] = -1;
        buffer.in = (buffer.in + 1) % BUFFER_SIZE;
        buffer.count++;

        pthread_cond_signal(&buffer.consume);
        pthread_mutex_unlock(&buffer.mutex);

        sleep(1);
    }


    // sincronizar consumidores
    for (int i = p; i < p+c; i++) 
    {
        int s = pthread_join(pool[i].thread_id, NULL);

        printf("Thread consumidor %i terminó\n", pool[i].thread_num);

        if(s != 0)
        {
            perror("pthread_join()");
            exit(1);
        }
    }

    free(pool);

    return 0;
}