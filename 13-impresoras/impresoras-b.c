/*Suponga tener n usuarios que comparten 2 impresoras. Antes de utilizar la impresora, el U[i] invoca requerir(impresora). Esta operaci´on espera hasta que una de las
impresoras est´e disponible, retornando la identidad de la impresora libre. Despu´es de
utilizar a impresora, U[i] invocan a liberar(impresora).
Resuelve este problema considerando que cada usuario tiene una prioridad ´unica.
Se otorga la impresora al usuario con mayor prioridad que est´a esperando.*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

#define IMPRESORAS 2
#define USUARIOS 5
#define PRIORIDAD_ALTA 3
#define PRIORIDAD_MEDIA 2
#define PRIORIDAD_BAJA 1

sem_t sem_alta, sem_media, sem_baja;
pthread_mutex_t mutex_impresoras = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_contadores = PTHREAD_MUTEX_INITIALIZER;
int esperando_alta= 0, esperando_media= 0, esperando_baja= 0;
bool impresoras_libres[IMPRESORAS];

int requerir(int usuario_id, int prioridad) {
    printf("Usuario %d (prioridad %d) requiere impresora\n", usuario_id, prioridad);
    
    // Incrementar contador de espera según prioridad
    pthread_mutex_lock(&mutex_contadores);
    if (prioridad == PRIORIDAD_ALTA) esperando_alta++;
    else if (prioridad == PRIORIDAD_MEDIA) esperando_media++;
    else esperando_baja++;
    pthread_mutex_unlock(&mutex_contadores);
    
    // Esperar en el semáforo correspondiente
    if (prioridad == PRIORIDAD_ALTA) {
        sem_wait(&sem_alta);
    } else if (prioridad == PRIORIDAD_MEDIA) {
        sem_wait(&sem_media);
    } else {
        sem_wait(&sem_baja);
    }
    
    // Decrementar contador al obtener acceso
    pthread_mutex_lock(&mutex_contadores);
    if (prioridad == PRIORIDAD_ALTA) esperando_alta--;
    else if (prioridad == PRIORIDAD_MEDIA) esperando_media--;
    else esperando_baja--;
    pthread_mutex_unlock(&mutex_contadores);
    
    // Asignar impresora (igual que antes)
    pthread_mutex_lock(&mutex_impresoras);
    int impresora_id = -1;
    for (int i = 0; i < IMPRESORAS; i++) {
        if (impresoras_libres[i]) {
            impresora_id = i;
            impresoras_libres[i] = false;
            break;
        }
    }
    pthread_mutex_unlock(&mutex_impresoras);
    
    printf("Usuario %d obtuvo impresora %d\n", usuario_id, impresora_id);
    return impresora_id;
}

void liberar(int impresora_id, int usuario_id) {
    printf("Usuario %d libera impresora %d\n", usuario_id, impresora_id);
    
    // Liberar la impresora
    pthread_mutex_lock(&mutex_impresoras);
    impresoras_libres[impresora_id] = true;
    pthread_mutex_unlock(&mutex_impresoras);
    
    // Despertar al siguiente usuario por orden de prioridad
    pthread_mutex_lock(&mutex_contadores);
    
    if (esperando_alta > 0) {
        printf("  -> Despertando usuario de prioridad ALTA\n");
        sem_post(&sem_alta);
    } else if (esperando_media > 0) {
        printf("  -> Despertando usuario de prioridad MEDIA\n");
        sem_post(&sem_media);
    } else if (esperando_baja > 0) {
        printf("  -> Despertando usuario de prioridad BAJA\n");
        sem_post(&sem_baja);
    }
    
    pthread_mutex_unlock(&mutex_contadores);
}

void* usuario(void* args) {
    int id = (int)(intptr_t) args;
    
    int prioridad;
    if (id <= 2) prioridad = PRIORIDAD_ALTA;
    else if (id <= 4) prioridad = PRIORIDAD_MEDIA;
    else prioridad = PRIORIDAD_BAJA;
    
    int impresora_id = requerir(id, prioridad);
    
    printf("Usuario %d usando impresora %d...\n", id, impresora_id);
    sleep(3); // Simular uso
    
    liberar(impresora_id, id);
    
    return NULL;
}


int main() {

    //Inicialización de impresoras
    for (int i=0; i<IMPRESORAS; i++) {
        impresoras_libres[i] = true;
    }

    //Declaración de threads
    pthread_t thread_usuario[USUARIOS];

    //Inicialización de semáforos
    sem_init(&sem_alta, 0, IMPRESORAS);
    sem_init(&sem_media, 0, 0);
    sem_init(&sem_baja, 0, 0);

    //Creación de threads
    for (int i=0; i<USUARIOS; i++) {
        pthread_create(&thread_usuario[i], NULL, usuario, (void*)(intptr_t)(i+1));
    }

    //Finalización de threads
    for (int i=0; i<USUARIOS; i++) {
        pthread_join(thread_usuario[i], NULL);
    }

    //Destrucción de semáforos
    sem_destroy(&sem_alta);
    sem_destroy(&sem_media);
    sem_destroy(&sem_baja);

    printf("\nSecuencia terminada\n");
    return 0;
}