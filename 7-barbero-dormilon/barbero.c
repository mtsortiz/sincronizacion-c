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
int sillas_disponibles = SILLAS;
_Bool barbero_durmiendo = 1;

void* barbero(void* arg) {
    for (int i=0; i<CANT_CORTES; i++) {
        if(sillas_disponibles == SILLAS) {
            printf("Barbero durmiendo...\n");
            barbero_durmiendo = 1;
        } else {
            sem_wait(&sem_barbero);
            pthread_mutex_lock(&mutex_sillas);
                printf("Barbero llama al cliente %d y le corta el pelo.\n", i+1);
                sillas_disponibles++;
            pthread_mutex_unlock(&mutex_sillas);
            sleep(1);
            printf("Barbero terminó de cortar el pelo al cliente %d\n",i+1);
            sem_post(&sem_cliente);
        }
    }
    printf("Barbero ha terminado su jornada laboral.\n");
    return NULL;
}

void* cliente(void* arg) {
    int id = (int)(intptr_t) arg;
    for (int i=0; i<CLIENTES; i++) {
        if(sillas_disponibles == 0)
            printf("No hay sillas disponibles. Cliente %d se va.\n", id);
        else  {
            sem_wait(&sem_cliente);
            pthread_mutex_lock(&mutex_sillas);
                if(barbero_durmiendo) {
                    printf("Cliente %d despierta al barbero.\n",id);
                    barbero_durmiendo = 0;
                }
                printf("Cliente %d se sienta en la sala de espera.\n",id);
                sillas_disponibles--;
                sem_post(&sem_barbero);
            pthread_mutex_unlock(&mutex_sillas);
        }
    }
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