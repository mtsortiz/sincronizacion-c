/*
El oso y las abejas. Se tienen n abejas y un hambriento oso. Comparten un tarro de
miel. Inicialmente el tarro de miel está vacío, su capacidad es M porciones de miel. El
oso duerme hasta que el tarro de miel se llene, entonces se come toda la miel y vuelve
a dormir. Cada abeja produce una porción de miel que coloca en el tarro, la abeja que
llena el tarro de miel despierta al oso. Escriba un programa para que sincronice a las
abejas y al oso.
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdint.h>

#define CANTIDAD 5
#define CAPACIDAD 10

sem_t tarro_vacio, tarro_lleno;
pthread_mutex_t mutex_tarro = PTHREAD_MUTEX_INITIALIZER;
pthread_t thread_abeja[CANTIDAD], thread_oso;
int porciones_actuales = 0;

void* abeja(void* arg) {
    int id = (int)(intptr_t) arg;
    for (int i=0; i<CAPACIDAD; i++) {
        sem_wait(&tarro_vacio);
        pthread_mutex_lock(&mutex_tarro);
            porciones_actuales++;
            printf("Abeja %d depositando porcion %d/10\n", id, porciones_actuales);
            if(porciones_actuales == CAPACIDAD)
                printf("Tarro lleno. Abeja %d despierta al oso.\n", id);
        pthread_mutex_unlock(&mutex_tarro);
        sem_post(&tarro_lleno);
    }
    return NULL;
}

void* oso(void* arg) {
    for (int i=0; i<CANTIDAD; i++) {
        for (int j=0; j<CAPACIDAD; j++) {
            sem_wait(&tarro_lleno);   
        }
        printf("Oso comiendo miel...");
        sleep(1);
        pthread_mutex_lock(&mutex_tarro);
            porciones_actuales = 0;
            printf("Oso comió toda la miel.Vuelve a dormir\n");
        pthread_mutex_unlock(&mutex_tarro);
        for (int j=0; j<CAPACIDAD; j++) {
            sem_post(&tarro_vacio);
        }
    }
    return NULL;
}


int main() {
    //Declaración de Threads
    pthread_t thread_abeja[CANTIDAD], thread_oso;

    //Inicialización de Semáforos
    sem_init(&tarro_vacio, 0, CAPACIDAD);
    sem_init(&tarro_lleno, 0, 0);

    //Creación de Threads
    pthread_create(&thread_oso, NULL, oso, NULL);
    for (int i=0; i<CANTIDAD; i++)
        pthread_create(&thread_abeja[i], NULL, abeja, (void*)(intptr_t)(i + 1));

    //Esperar a que todos los threads terminen
    pthread_join(thread_oso, NULL);
    for (int i=0; i<CANTIDAD; i++) {
        pthread_join(thread_abeja[i], NULL);
    }

    //Destrucción de Semáforos
    sem_destroy(&tarro_vacio);
    sem_destroy(&tarro_lleno);

    printf("\nSecuencia terminada\n");
    return 0;
}