/*Suponga tener n usuarios que comparten 2 impresoras. Antes de utilizar la impresora, el U[i] invoca requerir(impresora). Esta operaci´on espera hasta que una de las
impresoras est´e disponible, retornando la identidad de la impresora libre. Despu´es de
utilizar a impresora, U[i] invocan a liberar(impresora).
a) Resuelva este problema considerando que cada usuario es un thread y la sincronizaci´on se realiza utilizando sem´aforos.*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

#define IMPRESORAS 2
#define USUARIOS 5

sem_t sem_impresoras;
pthread_mutex_t mutex_impresoras = PTHREAD_MUTEX_INITIALIZER;
bool impresoras_libres[IMPRESORAS];

int requerir(int id) {
    int impresora_id = -1;
    sem_wait(&sem_impresoras);

    pthread_mutex_lock(&mutex_impresoras);
    for (int i=0; i<IMPRESORAS; i++) {
        if (impresoras_libres[i]){
            impresora_id = i;
            break;
        }
    }
    if (impresora_id != -1)
        impresoras_libres[impresora_id] = false;
    pthread_mutex_unlock(&mutex_impresoras);

    printf("Impresora %d asignada al usuario %d\n", impresora_id, id);
    return impresora_id;
}

void liberar(int impresora_id) {
    pthread_mutex_lock(&mutex_impresoras);
    impresoras_libres[impresora_id] = true;
    pthread_mutex_unlock(&mutex_impresoras);

    printf("Impresora %d liberada\n", impresora_id);
    sem_post(&sem_impresoras);
}

void* usuario(void* args) {
    int id = (int) (intptr_t) args;
    printf("Usuario %d requiere impresora\n", id);
    int impresora_id = requerir(id);
    sleep(2);

    liberar(impresora_id);

    return NULL;
}


int main() {

    //Inicialización de impresoras
    for (int i=0; i<IMPRESORAS; i++) {
        impresoras_libres[i] = true;
    }

    //Declaración de threads
    pthread_t thread_usuario[USUARIOS];

    //Inicialización de semáforos
    sem_init(&sem_impresoras, 0, IMPRESORAS);

    //Creación de threads
    for (int i=0; i<USUARIOS; i++) {
        pthread_create(&thread_usuario[i], NULL, usuario, (void*)(intptr_t)(i+1));
    }

    //Finalización de threads
    for (int i=0; i<USUARIOS; i++) {
        pthread_join(thread_usuario[i], NULL);
    }

    //Destrucción de semáforos
    sem_destroy(&sem_impresoras);

    printf("\nSecuencia terminada\n");
    return 0;
}