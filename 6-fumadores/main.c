/*
 El problema de los fumadores de cigarrillos (Patil 1971). Considere un sistema con tres
procesos fumadores y un proceso agente. Cada fumador est´a continuamente armando
y fumando cigarrillos. Sin embargo, para armar un cigarrillo, el fumador necesita tres
ingredientes: tabaco, papel y f´osforos. Uno de los procesos fumadores tiene papel, otro
tiene el tabaco y el tercero los f´osforos. El agente tiene una cantidad infinita de los
tres materiales. El agente coloca dos de los ingredientes sobre la mesa. El fumador
que tiene el ingrediente restante armar´ıa un cigarrillo y se lo fuma, avisando al agente
cuando termina. Entonces, el agente coloca dos de los tres ingredientes y se repite
el ciclo. Escriba un programa para sincronizar al agente y los fumadores utilizando
sem´aforos.

PARA COMPILAR TODOS LOS ARCHIVOS:
# Compilar cada archivo a objeto
gcc -c main.c -o main.o
gcc -c agente.c -o agente.o  
gcc -c fumador-tabaco.c -o fumador-tabaco.o
gcc -c fumador-papel.c -o fumador-papel.o
gcc -c fumador-fosforos.c -o fumador-fosforos.o

# Linkear todos los objetos
gcc main.o agente.o fumador-tabaco.o fumador-papel.o fumador-fosforos.o -o fumadores -lpthread

*/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

int ciclos_completados = 0;
int MAX_CICLOS = 10;

pthread_mutex_t mutex_ciclos = PTHREAD_MUTEX_INITIALIZER;

// Declarar semáforos globales
extern sem_t sem_agente;
extern sem_t sem_tabaco;
extern sem_t sem_papel; 
extern sem_t sem_fosforos;

// Declarar funciones de otros archivos
extern void* agente(void* arg);
extern void* fumador_tabaco(void* arg);
extern void* fumador_papel(void* arg);  
extern void* fumador_fosforos(void* arg);

// Variables globales compartidas
sem_t sem_agente;
sem_t sem_tabaco;
sem_t sem_papel;
sem_t sem_fosforos;

int main() {
    // Inicializar semáforos
    sem_init(&sem_agente, 0, 1);
    sem_init(&sem_tabaco, 0, 0);
    sem_init(&sem_papel, 0, 0);
    sem_init(&sem_fosforos, 0, 0);
    
    // Crear threads
    pthread_t th_agente, th_tabaco, th_papel, th_fosforos;
    
    pthread_create(&th_agente, NULL, agente, NULL);
    pthread_create(&th_tabaco, NULL, fumador_tabaco, NULL);
    pthread_create(&th_papel, NULL, fumador_papel, NULL);
    pthread_create(&th_fosforos, NULL, fumador_fosforos, NULL);
    
    // Esperar threads
    pthread_join(th_agente, NULL);
    pthread_join(th_tabaco, NULL);
    pthread_join(th_papel, NULL);
    pthread_join(th_fosforos, NULL);
    
    //Destruir mutex
    pthread_mutex_destroy(&mutex_ciclos);

    // Destruir semáforos
    sem_destroy(&sem_agente);
    sem_destroy(&sem_tabaco);
    sem_destroy(&sem_papel);
    sem_destroy(&sem_fosforos);

    printf("\nSecuencia terminada\n");
    return 0;
}