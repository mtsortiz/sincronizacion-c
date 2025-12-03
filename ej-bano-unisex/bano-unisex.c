/*Hay un solo baño en el DCIC. 
    -Puede ser utilizado por mujeres y por hombres pero no al mismo tiempo
    -Se permite cualquier cantidad de hombres o mujeres en el baño al mismo tiempo siempre que sean del mismo género
    -Considere que tiene 10 mujeres y 10 hombres
    */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

#define MUJERES 10
#define HOMBRES 10
#define CICLOS 20

pthread_mutex_t mutex_gen = PTHREAD_MUTEX_INITIALIZER;
sem_t sem_baño;
int cont_mujer= 0, cont_hombre= 0, ciclos_completados= 0;

void* mujer(void* arg) {
    int id = (int)(intptr_t) arg;
    
    pthread_mutex_lock(&mutex_gen);
        if (cont_hombre == 0) {
            if (cont_mujer == 0)
                sem_wait(&sem_baño);
            cont_mujer++;
            printf("Mujer %d ingresa al baño. Hay %d dentro.\n", id, cont_mujer);
            pthread_mutex_unlock(&mutex_gen);

            sleep(1);
            
            pthread_mutex_lock(&mutex_gen);
            cont_mujer--;
            printf("Mujer %d salio del baño.\n", id);
            if (cont_mujer == 0)
                sem_post(&sem_baño);
            ciclos_completados++;
        } else {
            printf("Mujer %d esta esperando. Hay hombres en el baño\n", id);
        }
    pthread_mutex_unlock(&mutex_gen);

    return NULL;
}

void* hombre(void* arg) {
    int id = (int)(intptr_t) arg;
    
    pthread_mutex_lock(&mutex_gen);
        if (cont_mujer == 0) {
            if (cont_hombre == 0)
                sem_wait(&sem_baño);
            cont_hombre++;
            printf("Hombre %d ingresa al baño. Hay %d dentro.\n", id, cont_hombre);
            pthread_mutex_unlock(&mutex_gen);

            sleep(1);

            pthread_mutex_lock(&mutex_gen);
            cont_hombre--;
            printf("Hombre %d salio del baño.\n", id);
            if (cont_hombre == 0)
                sem_post(&sem_baño);
            ciclos_completados++;
        } else {
            printf("Hombre %d esta esperando. Hay mujeres en el baño\n", id);
        }
    pthread_mutex_unlock(&mutex_gen);

    return NULL;
}


int main() {
    //Declaración de threads
    pthread_t thread_hombre[HOMBRES], thread_mujer[MUJERES];

    //Inicialización de semáforos
    sem_init(&sem_baño, 0, 1);

    //Creación de threads
    for (int i=0; i<10; i++) {
        pthread_create(&thread_hombre[i], NULL, hombre, (void*)(intptr_t)(i + 1));
        pthread_create(&thread_mujer[i], NULL, mujer, (void*)(intptr_t)(i + 1));
    }

    //Finalización de hilos
    for (int i=0; i<10; i++) {
        pthread_join(thread_hombre[i], NULL);
        pthread_join(thread_mujer[i], NULL);
    }

    //Destrucción de semáforos
    sem_destroy(&sem_baño);

    printf("\nSecuencia terminada\n");
    return 0;
}