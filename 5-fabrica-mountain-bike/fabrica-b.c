/*
    En la fábrica de bicicletas MountanenBike, tenemos tres operarios que 
    denominaremos OP1, OP2 y OP3. OP1 monta ruedas, OP2 monta el cuadro de 
    las bicicletas, y OP3, el manillar. Un cuarto operario, el Montador,
    se encarga de tomar dos ruedas, un cuadro y un manillar, y arma la bicicleta. 
    Sincronizar las acciones de los tres operarios y el Montador utilizando 
    semáforos

    Los operarios no tienen ningún espacio para almacenar los componentes 
    producidos pero si tiene espacio para fabricar de a uno, y en el caso del 
    OP1 tiene espacio para fabricar las dos ruedas y el Montador no podrá
    tomar ninguna pieza si esta no ha sido fabricada previamente por el 
    correspondiente operario.
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define CANTIDAD 3

sem_t montador, op1, op2, op3;

void* operario1(void* arg) {
    for (int i=0; i<CANTIDAD; i++) {
        sem_wait(&op1);
            printf("Rueda 1 fabricada\n");
        sem_post(&montador);

        sem_wait(&op1);
            printf("Rueda 2 fabricada\n");
        sem_post(&montador);
    }
    return NULL;
}

void* operario2(void* arg) {
    for (int i=0; i<CANTIDAD; i++) {
        sem_wait(&op2);
            printf("Cuadro fabricado\n");
        sem_post(&montador);
    }
    return NULL;
}

void* operario3(void* arg) {
    for (int i=0; i<CANTIDAD; i++) {
        sem_wait(&op3);
            printf("Manillar fabricado\n");
        sem_post(&montador);
    }
    return NULL;
}

void* montador_op(void* arg) {
    for (int i=0; i<CANTIDAD; i++) {
        sem_post(&op1);
        sem_post(&op1);
        sem_post(&op2);
        sem_post(&op3);

        sem_wait(&montador);
        sem_wait(&montador);
        sem_wait(&montador);
        sem_wait(&montador);
            sleep(1);
            printf("Bicicleta %d montada\n", i+1);
    }
    return NULL;
}


int main() {
    //Declaracion de Threads
    pthread_t thread_montador, thread_op1, thread_op2, thread_op3;

    //Inicializacion de semaforos
    sem_init(&montador,0,0);
    sem_init(&op1,0,0);
    sem_init(&op2,0,0);
    sem_init(&op3,0,0);

    //Creacion de threads
    pthread_create(&thread_montador, NULL, montador_op, NULL);
    pthread_create(&thread_op1, NULL, operario1, NULL);
    pthread_create(&thread_op2, NULL, operario2, NULL);
    pthread_create(&thread_op3, NULL, operario3, NULL);

    //Esperar a que los threads terminen
    pthread_join(thread_montador, NULL);
    pthread_join(thread_op1, NULL);
    pthread_join(thread_op2, NULL);
    pthread_join(thread_op3, NULL);

    //Destruccion de semaforos
    sem_destroy(&montador);
    sem_destroy(&op1);
    sem_destroy(&op2);
    sem_destroy(&op3);

    printf("\nSecuencia terminada\n");
    return 0;
}