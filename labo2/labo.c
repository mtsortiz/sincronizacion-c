/*  10 usuarios tienen acceso a una base de datos en la que guardan su trabajo
    Para poder guardar su trabajo, cada usuario requiere entre 1 y 3 licencias.
    La cantidad de licencias disponibles es limitada, exiten 10 licencias. 
    Solo puede acceder un usuario a la vez a la base de datos 
    Se debe implementar una solución utilizando sincronización de hilos y semáforos para
    */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define NUM_LICENCIAS 10
#define MAX_LICENCIAS 3
#define MIN_LICENCIAS 1
#define USUARIOS 10

sem_t sem_licencias;
int licencias = NUM_LICENCIAS;
pthread_mutex_t mutex_licencias = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_bbdd = PTHREAD_MUTEX_INITIALIZER;

void* usuario(void* arg) {
    int id = (int)(intptr_t) arg;
    sleep(rand() % 5);
    int licencias_requeridas = (rand() % MAX_LICENCIAS) + MIN_LICENCIAS;
    pthread_mutex_lock(&mutex_licencias);
        if (licencias_requeridas <= licencias) {
            licencias -= licencias_requeridas;
            pthread_mutex_unlock(&mutex_licencias);

            for(int i=0; i<licencias_requeridas; i++) {
                sem_wait(&sem_licencias);
            }
            printf("Usuario %d requiere %d licencias.\n", id, licencias_requeridas);
            pthread_mutex_lock(&mutex_bbdd);
                printf("Usuario %d accediendo a la base de datos.\n", id);
                sleep(2);  
                printf("Usuario %d ha terminado de guardar su trabajo.\n", id);
            pthread_mutex_unlock(&mutex_bbdd);

            for(int i=0; i<licencias_requeridas; i++) {
                sem_post(&sem_licencias);
            }

            pthread_mutex_lock(&mutex_licencias);
                licencias += licencias_requeridas;
            pthread_mutex_unlock(&mutex_licencias);
        } else {
            pthread_mutex_unlock(&mutex_licencias);
            printf("Usuario %d no tiene suficientes licencias disponibles. (Requiere %d y hay %d)\n", id, licencias_requeridas, licencias);
        }
    return NULL; 
}

int main() {
    //Inicialización de la semilla para números aleatorios
    srand(time(NULL));

    //Declaración de threads
    pthread_t thread_usuario[USUARIOS];

    //Inicialización de semáforos
    sem_init(&sem_licencias, 0, NUM_LICENCIAS);

    //Creación de threads
    for(int i=0; i<USUARIOS; i++) {
        pthread_create(&thread_usuario[i], NULL, usuario, (void*)(intptr_t)(i+1));
    }

    //Finalización de threads
    for (int i=0; i<USUARIOS; i++) {
        pthread_join(thread_usuario[i], NULL);
    }

    //Destrucción de semáforos
    sem_destroy(&sem_licencias);

    printf("\nSecuencia terminada\n");
    return 0;
}
