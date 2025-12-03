/*Hay 3 tipos de atletas participantes: lanzadores de jabalina, lanzadores de martillo y corredores
-Un lanzador de martillo o de jabalina debe esperar para entrar a que el anterior termine
-Si hay un lanzador de jabalina o de martillo, cualquier atleta que llegue debe esperar a que el otro termine
-Si hay algun corredor adentro y llega otro corredor, entra aunque haya lanzadores esperando.
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define LANZADORES_JABALINA 3
#define LANZADORES_MARTILLO 3
#define CORREDORES 5

sem_t sem_jabalina, sem_martillo, sem_corredor;
pthread_mutex_t mutex_club = PTHREAD_MUTEX_INITIALIZER;
int corredor= 0, lanzador= 0;


void* atleta_jabalina(void* arg) {
    int id = (int)(intptr_t) arg;
    sleep(rand() % 3); 
    bool entro= false;
    printf("Lanzador de jabalina %d quiere entrar al club\n", id);
    while(!entro) {
        sem_wait(&sem_jabalina);
        pthread_mutex_lock(&mutex_club);
        if (lanzador == 0 && corredor == 0) {
            lanzador++;
            entro= true;
            pthread_mutex_unlock(&mutex_club);

            printf("Lanzador de jabalina %d entrenando...\n", id);        
            sleep(2);
            printf("Lanzador de jabalina %d terminó de entrenar.\n", id);
            
            pthread_mutex_lock(&mutex_club);
            lanzador--;
            pthread_mutex_unlock(&mutex_club);
            sem_post(&sem_jabalina);
        } else {
            pthread_mutex_unlock(&mutex_club);
            printf("Lanzador de jabalina %d esperando...\n", id);
            sem_post(&sem_jabalina);
            sleep(1);
        }
    }

    return NULL;
}

void* atleta_martillo(void* arg) {
    int id = (int)(intptr_t) arg;
    sleep(rand() % 3); 
    bool entro= false;
    printf("Lanzador de martillo %d quiere entrar al club\n", id);
    while(!entro) {
        sem_wait(&sem_martillo);
        pthread_mutex_lock(&mutex_club);
        if (lanzador == 0 && corredor == 0) {
            lanzador++;
            entro= true;
            pthread_mutex_unlock(&mutex_club);

            printf("Lanzador de martillo %d entrenando...\n", id);
            sleep(2);
            printf("Lanzador de martillo %d terminó de entrenar.\n", id);
            
            pthread_mutex_lock(&mutex_club);
            lanzador--;
            pthread_mutex_unlock(&mutex_club);
            sem_post(&sem_martillo);
        } else {
            pthread_mutex_unlock(&mutex_club);
            printf("Lanzador de martillo %d esperando...\n", id);
            sem_post(&sem_martillo);
            sleep(1);
        }
    }

    return NULL;
}

void* atleta_corredor(void* arg) {
    int id = (int)(intptr_t) arg;
    sleep(rand() % 3); 
    bool entro= false;
    printf("Corredor %d quiere entrar al club\n", id);
    while(!entro) {
        sem_wait(&sem_corredor);
        pthread_mutex_lock(&mutex_club);
        if(lanzador == 0) {
            corredor++;
            entro= true;
            pthread_mutex_unlock(&mutex_club);

            printf("Corredor %d entrenando...\n", id);
            sleep(2);
            printf("Corredor %d terminó de entrenar\n", id);
            
            sem_post(&sem_corredor);
            pthread_mutex_lock(&mutex_club);
            corredor--;
            pthread_mutex_unlock(&mutex_club);    
        } else {
            pthread_mutex_unlock(&mutex_club);
            printf("Corredor %d esperando...\n", id);
            sem_post(&sem_corredor);
            sleep(1);
        }
    }

    return NULL;
}


int main() {
    //Inicialización de la semilla para números aleatorios
    srand(time(NULL));

    //Declaración de threads 
    pthread_t thread_jabalina[LANZADORES_JABALINA];
    pthread_t thread_martillo[LANZADORES_MARTILLO];
    pthread_t thread_corredor[CORREDORES];

    //Inicialización de semáforos
    sem_init(&sem_jabalina, 0, 1);
    sem_init(&sem_martillo, 0, 1);
    sem_init(&sem_corredor, 0, CORREDORES); 

    //Creación de threads
    for (int i=0; i<3; i++) {
        pthread_create(&thread_jabalina[i], NULL, atleta_jabalina, (void*)(intptr_t)(i+1));
        pthread_create(&thread_martillo[i], NULL, atleta_martillo, (void*)(intptr_t)(i+1));
    }
    for (int i=0; i<CORREDORES; i++) {
        pthread_create(&thread_corredor[i], NULL, atleta_corredor, (void*)(intptr_t)(i+1));
    }

    //Finalización de threads
    for (int i=0; i<3; i++) {
        pthread_join(thread_jabalina[i], NULL);
        pthread_join(thread_martillo[i], NULL); 
    }
    for (int i=0; i<CORREDORES; i++) {
        pthread_join(thread_corredor[i], NULL);
    }

    //Destrucción de semáforos
    sem_destroy(&sem_jabalina);
    sem_destroy(&sem_martillo);
    sem_destroy(&sem_corredor);

    printf("\nSecuencia terminada\n");
    return 0;
}