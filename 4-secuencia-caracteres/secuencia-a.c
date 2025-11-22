//La secuencia permitida es: ABCDEABCDEABCDEABCDEABCDEABCDE ...

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define REPETITIONS 100

sem_t sem_a, sem_b, sem_c, sem_d, sem_e;

void* print_A(void* arg) {
    for(int i=0; i<REPETITIONS; i++) {
        sem_wait(&sem_a);
            printf("A");
        sem_post(&sem_b);
    }
}

void* print_B(void* arg) {
    for(int i=0; i<REPETITIONS; i++) {
        sem_wait(&sem_b);
            printf("B");
        sem_post(&sem_c);
    }
}

void* print_C(void* arg) {
    for(int i=0; i<REPETITIONS; i++) {
        sem_wait(&sem_c);
            printf("C");
        sem_post(&sem_d);
    }
}

void* print_D(void* arg) {
    for(int i=0; i<REPETITIONS; i++) {
        sem_wait(&sem_d);
            printf("D");
        sem_post(&sem_e);
    }
}

void* print_E(void* arg) {
    for(int i=0; i<REPETITIONS; i++) {
        sem_wait(&sem_e);
            printf("E");
        sem_post(&sem_a);
    }
}


int main() {
    //Declaracion de threads
    pthread_t thread_a, thread_b, thread_c, thread_d, thread_e;


    //Inicialización de semaforos
    sem_init(&sem_a,0,1);
    sem_init(&sem_b,0,0);
    sem_init(&sem_c,0,0);
    sem_init(&sem_d,0,0);
    sem_init(&sem_e,0,0);

    //Creacion de threads
    pthread_create(&thread_a, NULL, print_A, NULL);
    pthread_create(&thread_b, NULL, print_B, NULL);
    pthread_create(&thread_c, NULL, print_C, NULL);
    pthread_create(&thread_d, NULL, print_D, NULL);
    pthread_create(&thread_e, NULL, print_E, NULL);

    //Esperar a que los threads terminen
    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);
    pthread_join(thread_c, NULL);
    pthread_join(thread_d, NULL);
    pthread_join(thread_e, NULL);

    //Destruccion de semaforos
    sem_destroy(&sem_a);
    sem_destroy(&sem_b);
    sem_destroy(&sem_c);
    sem_destroy(&sem_d);
    sem_destroy(&sem_e);

    printf("\nSecuencia terminada\n");
    return 0;
}