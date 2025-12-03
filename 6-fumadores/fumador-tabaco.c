/*Proceso fumador con tabaco*/
#include "header.h"

void* fumador_tabaco(void* arg) {
    while(1) {
        sem_wait(&sem_tabaco); // Esperar que agente ponga papel+fósforos
        
        pthread_mutex_lock(&mutex_ciclos);
            if (ciclos_completados >= MAX_CICLOS) {
                pthread_mutex_unlock(&mutex_ciclos);
                break; // Terminar si se alcanzó el máximo de ciclos
            }
            ciclos_completados++;
        pthread_mutex_unlock(&mutex_ciclos);

        printf("Fumador con tabaco arma y fuma cigarrillo\n");
        sleep(2); // Simular fumar
        printf("Fumador con tabaco terminó de fumar\n");
        
        sem_post(&sem_agente); // Avisar al agente que terminó

        // Verificar de nuevo si debe terminar
        pthread_mutex_lock(&mutex_ciclos);
        if(ciclos_completados >= MAX_CICLOS) {
            pthread_mutex_unlock(&mutex_ciclos);
            break;
        }
        pthread_mutex_unlock(&mutex_ciclos);
    }
    
    return NULL;
}