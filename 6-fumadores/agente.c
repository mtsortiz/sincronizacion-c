/*Proceso agente - Cuenta con los tres materiales infinitos*/
#include "header.h" // Con las declaraciones de semáforos

void* agente(void* arg) {
    for(int i = 0; i < 10; i++) { // 10 ciclos
        sem_wait(&sem_agente);
        
        int opcion = rand() % 3;
        if (opcion == 0) {
            printf("Agente pone papel y fósforos en la mesa\n");
            sem_post(&sem_tabaco); // Despertar fumador con tabaco
        } else if (opcion == 1) {
            printf("Agente pone tabaco y fósforos en la mesa\n");
            sem_post(&sem_papel); // Despertar fumador con papel
        } else {
            printf("Agente pone tabaco y papel en la mesa\n");
            sem_post(&sem_fosforos); // Despertar fumador con fósforos
        }
    }
    
    // Despertar a todos los fumadores para que terminen
    sem_post(&sem_tabaco);
    sem_post(&sem_papel);
    sem_post(&sem_fosforos);
    
    return NULL;
}