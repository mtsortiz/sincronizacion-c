/*
El problema del barbero dormilón (Dijkstra 1971). Una barbería se compone de una
sala de espera con n sillas y la sala de barbería donde se encuentra la silla del barbero.
Si no hay clientes a quienes atender, el barbero se pone a dormir. Si un cliente entra
y todas las sillas están ocupadas, el cliente sale de la barbería. Si el barbero está
ocupado, pero hay sillas disponibles, el cliente se sienta en una. Si el barbero está
dormido, el cliente lo despierta. Escriba un programa para coordinar al barbero y sus
clientes utilizando semáforos.
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdint.h>

#define SILLAS 3
#define CLIENTES 7
#define CANT_CORTES 5

sem_t sem_barbero, sem_cliente;
pthread_mutex_t mutex_sillas = PTHREAD_MUTEX_INITIALIZER;
pthread_t thread_cliente[CLIENTES], thread_barbero;
int sillas_disponibles = SILLAS, cortes_realizados = 0;
_Bool barbero_durmiendo = 1;

void* barbero(void* arg) {
    sem_wait(&sem_barbero);
    while(cortes_realizados < CANT_CORTES) {
        if(sillas_disponibles == SILLAS) {
            printf("Barbero durmiendo...\n");
            barbero_durmiendo = 1;
        } else {
            pthread_mutex_lock(&mutex_sillas);
                printf("Barbero llama al cliente %d y le corta el pelo.\n", cortes_realizados+1);
                sillas_disponibles++;
                cortes_realizados++;
                if(cortes_realizados == CANT_CORTES) {
                    for (int j=0; j<CLIENTES-CANT_CORTES; j++) 
                        sem_post(&sem_cliente); 
                }
            pthread_mutex_unlock(&mutex_sillas);
            sleep(2);
            printf("Barbero terminó de cortar el pelo al cliente %d\n",cortes_realizados);
        }
        sem_post(&sem_cliente);
    }
    for (int j=0; j<CLIENTES-CANT_CORTES; j++) 
        sem_post(&sem_cliente); 
    printf("Barbero ha terminado su jornada, echa a los clientes restantes y cierra la barbería.\n");
    return NULL;
}

void* cliente(void* arg) {
    int id = (int)(intptr_t) arg;

    pthread_mutex_lock(&mutex_sillas);
        if(sillas_disponibles == 0) {
            printf("No hay sillas disponibles. Cliente %d se va.\n", id);
            pthread_mutex_unlock(&mutex_sillas);
            return NULL;
        }
    pthread_mutex_unlock(&mutex_sillas);
    sem_wait(&sem_cliente);
    printf("Cliente %d entra a la barbería.\n",id);
    pthread_mutex_lock(&mutex_sillas);
        if(barbero_durmiendo) {
            printf("Cliente %d despierta al barbero.\n",id);
            barbero_durmiendo = 0;
        }
        printf("Cliente %d se sienta en la sala de espera.\n",id);
        sillas_disponibles--; 
    pthread_mutex_unlock(&mutex_sillas);
    sem_post(&sem_barbero);

    return NULL;
}


int main() {
    //Declaración de threads
    pthread_t thread_cliente[CLIENTES], thread_barbero;

    //Inicialización de semáforos
    sem_init(&sem_barbero, 0, 0);
    sem_init(&sem_cliente, 0, SILLAS);

    //Creación de threads
    pthread_create(&thread_barbero, NULL, barbero, NULL);
    for (int i=0; i<CLIENTES; i++) {
        pthread_create(&thread_cliente[i], NULL, cliente, (void*)(intptr_t)(i+1));
    }

    //Espera a que todos los threads terminen
    pthread_join(thread_barbero, NULL);
    for (int i=0; i<CLIENTES; i++) {
        pthread_join(thread_cliente[i], NULL);
    }

    //Destrucción de semáforos
    sem_destroy(&sem_barbero);
    sem_destroy(&sem_cliente);

    printf("Barbería cerrada.\n");
    return 0;
}