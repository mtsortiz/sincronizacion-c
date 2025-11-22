//La secuencia permitida es: (A o B)CDE(A o B)CDE(A o B)CDE(A o B)CDE ...

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define REPETITIONS 100

sem_t sem_c, sem_d, sem_e, sem_ab;
int count_a = 0, count_b = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* print_A(void* arg) {
    while(1) {
        sem_wait(&sem_ab);  // Esperar turno para A o B
        pthread_mutex_lock(&mutex);
        if(count_a + count_b < REPETITIONS) {
            count_a++;
            pthread_mutex_unlock(&mutex);
            printf("A");
            sem_post(&sem_c);
        } else {
            pthread_mutex_unlock(&mutex);
            sem_post(&sem_ab); // Liberar el turno
            break;
        }
    }
    return NULL;
}

void* print_B(void* arg) {
    while(1) {
        sem_wait(&sem_ab);  // Esperar turno para A o B  
        pthread_mutex_lock(&mutex);
        if(count_a + count_b < REPETITIONS) {
            count_b++;
            pthread_mutex_unlock(&mutex);
            printf("B");
            sem_post(&sem_c);
        } else {
            pthread_mutex_unlock(&mutex);
            sem_post(&sem_ab); // Liberar el turno
            break;
        }
    }
    return NULL;
}

void* print_C(void* arg) {
    for(int i=0; i<REPETITIONS; i++) {
        sem_wait(&sem_c);
            printf("C");
        sem_post(&sem_d);
    }
    return NULL;
}

void* print_D(void* arg) {
    for(int i=0; i<REPETITIONS; i++) {
        sem_wait(&sem_d);
            printf("D");
        sem_post(&sem_e);
    }
    return NULL;
}

void* print_E(void* arg) {
    for(int i=0; i<REPETITIONS; i++) {
        sem_wait(&sem_e);
            printf("E");
        sem_post(&sem_ab);  // Liberar el turno para el próximo A o B (será aleatorio por competencia)
    }
    return NULL;
}


int main() {
    //Inicializar generador de números aleatorios
    srand(time(NULL));
    
    //Declaracion de threads
    pthread_t thread_a, thread_b, thread_c, thread_d, thread_e;

    //Inicialización de semaforos
    sem_init(&sem_c,0,0);
    sem_init(&sem_d,0,0);
    sem_init(&sem_e,0,0);
    sem_init(&sem_ab,0,1);  // A y B compiten por este único token

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
    sem_destroy(&sem_c);
    sem_destroy(&sem_d);
    sem_destroy(&sem_e);
    sem_destroy(&sem_ab);
    pthread_mutex_destroy(&mutex);

    printf("\nSecuencia terminada\n");
    printf("A ejecutado: %d veces, B ejecutado: %d veces\n", count_a, count_b);
    return 0;
}