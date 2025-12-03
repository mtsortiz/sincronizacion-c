#ifndef FUMADORES_H
#define FUMADORES_H

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//Declaraciones de variables globales
extern int ciclos_completados;
extern int MAX_CICLOS;

extern pthread_mutex_t mutex_ciclos;

// Declaraciones de semáforos
extern sem_t sem_agente;
extern sem_t sem_tabaco;
extern sem_t sem_papel;
extern sem_t sem_fosforos;

// Declaraciones de funciones
void* agente(void* arg);
void* fumador_tabaco(void* arg);
void* fumador_papel(void* arg);
void* fumador_fosforos(void* arg);

#endif