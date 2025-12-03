/* Escriba un programa que controle el acceso a un archivo, de tal manera
que se permita el acceso en lectura a varios procesos o a un solo escritor en forma
exclusiva*/

#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

sem_t sem_archivo;
pthread_mutex_t mutex_lectores = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_escritores = PTHREAD_MUTEX_INITIALIZER;
int lectores = 0, escritores = 0;

void* lector(void* arg) {
    int id = (int)(intptr_t) arg;
    sleep(rand() % 5);
    printf("Lector %d quiere leer el archivo.\n", id);

    pthread_mutex_lock(&mutex_escritores);
    while(escritores > 0) {
        pthread_mutex_unlock(&mutex_escritores);
        sleep(1);
        pthread_mutex_lock(&mutex_escritores);
    }
    pthread_mutex_unlock(&mutex_escritores);

    pthread_mutex_lock(&mutex_lectores);
    lectores++;
    if (lectores == 1) {
        sem_wait(&sem_archivo);
    }
    pthread_mutex_unlock(&mutex_lectores);

    printf("Lector %d leyendo archivo...\n", id);
    sleep(2);
    printf("Lector %d terminó de leer\n", id);

    pthread_mutex_lock(&mutex_lectores);
    lectores--;
    if (lectores == 0) {
        sem_post(&sem_archivo);
    }
    pthread_mutex_unlock(&mutex_lectores);

    return NULL;
}

void* escritor(void* arg) {
    int id = (int)(intptr_t) arg;
    printf("Escritor %d quiere escribir el archivo.\n", id);

    pthread_mutex_lock(&mutex_escritores);
    escritores++;  
    pthread_mutex_unlock(&mutex_escritores);

    sem_wait(&sem_archivo);
    
    printf("Escritor %d escribiendo el archivo...\n", id);
    sleep(3);
    printf("Escritor %d terminó de escribir el archivo\n", id);

    pthread_mutex_lock(&mutex_escritores);
    escritores--;
    pthread_mutex_unlock(&mutex_escritores);

    sem_post(&sem_archivo);

    return NULL;
}


int main() {

    srand(time(NULL));

    //Declaración de threads
    pthread_t thread_lectores[5], thread_escritores[3];

    //Inicialización de semáforos
    sem_init(&sem_archivo, 0, 1);

    //Creación de threads lectores
    for(int i=0; i<5; i++) {
        pthread_create(&thread_lectores[i], NULL, lector, (void*)(intptr_t)(i+1));
    }
    //Creación de threads escritores
    for(int i=0; i<3; i++) {
        pthread_create(&thread_escritores[i], NULL, escritor, (void*)(intptr_t)(i+1));
    }

    //Finalización de threads lectores
    for(int i=0; i<5; i++) {
        pthread_join(thread_lectores[i], NULL);
    }

    //Finalización de threads escritores
    for(int i=0; i<3; i++) {
        pthread_join(thread_escritores[i], NULL);
    }
    //Destrucción de semáforos
    sem_destroy(&sem_archivo);

    printf("\nSecuencia terminada\n"); 
    return 0;
}