/*Los servidores pueden diseñarse de modo que limiten el número de conexiones abiertas.
Por ejemplo, un servidor puede autorizar sólo N conexiones simultáneas de tipo socket.
En cuanto se establezcan las N conexiones, el servidor ya no aceptará otra conexión
entrante hasta que una conexión existente se libere*/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define MAX_CONEXIONES 5
#define CICLOS 10
sem_t sem_conexiones;
pthread_mutex_t mutex_conexion = PTHREAD_MUTEX_INITIALIZER;
int conexiones_disponibles = MAX_CONEXIONES;
int terminados = 0;

void* conectar(void* arg) {
    int id = (int)(intptr_t) arg;
    sleep(rand() % 3);
    printf("Cliente %d intentando conectarse al servidor...\n", id);

    sem_wait(&sem_conexiones);
    pthread_mutex_lock(&mutex_conexion);
    if (conexiones_disponibles > 0) {
        conexiones_disponibles--;
        pthread_mutex_unlock(&mutex_conexion);
        printf("Cliente %d conectado\n", id);
        sleep(3);
        pthread_mutex_lock(&mutex_conexion);
        conexiones_disponibles++;
        terminados++;
        if(terminados <= CICLOS - MAX_CONEXIONES) {
            printf("Cliente %d desconectado del servidor\n", id);
        }
        pthread_mutex_unlock(&mutex_conexion);
        sem_post(&sem_conexiones);
    }

    return NULL;
}

int main() {
    srand(time(NULL));

    //Declaración de threads
    pthread_t thread_usuario[CICLOS];

    //Inicialización de semáforos
    sem_init(&sem_conexiones, 0, MAX_CONEXIONES);

    //Creación de threads
    for (int i=0; i<CICLOS; i++) {
        pthread_create(&thread_usuario[i], NULL, conectar, (void*)(intptr_t)(i+1));
    }

    //Finalización de threads
    for (int i=0; i<CICLOS; i++) {
        pthread_join(thread_usuario[i], NULL);
    }

    //Destrucción de semáforos
    sem_destroy(&sem_conexiones);

    printf("\nSecuencia terminada\n");
    return 0;
}