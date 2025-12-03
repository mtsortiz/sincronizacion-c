#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define N 10

// LEEME LEEME LEEME LEEME LEEME LEEEMEEEEEEEEEE
/* Nota: Esta matriz está programada para el labo del pdf act3-proc-2025-t2
*  Si buscabas la matriz de caracteres, modificá este mismo código fuente y en vez de que genere
* matrices de numeros aleatorios, que sean caracteres aleatorios, y podrás resolver el otro ejercicio
*/

typedef struct {
    int fila;
    int columna;
    int (*a)[N];
    int (*b)[N];
    int (*c)[N];
}datos_thread;

void* suma_matrices(void* arg) {
    datos_thread* datos = (datos_thread*) arg;

    datos->c[datos->fila][datos->columna] = 
        datos->a[datos->fila][datos->columna] + 
        datos->b[datos->fila][datos->columna];
    
    return NULL;
}

int main() {

    pthread_t threads[N][N];
    datos_thread datos[N][N];

    int a[N][N], b[N][N], c[N][N];
    int i, j;

    // Inicializar la semilla para números aleatorios
    srand(time(NULL));

    // Cargar matriz A con números random (0 a 9)
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            a[i][j] = rand() % 10; 
        }
    }

    // Cargar matriz B con números random (0 a 9)
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            b[i][j] = rand() % 10;
        }
    }

    //Inicializar matriz C en 0
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            c[i][j] = 0;  
        }
    }

    // Mostrar matriz A
    printf("Matriz A:\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%4d", a[i][j]);
        }
        printf("\n");
    }

    // Mostrar matriz B
    printf("\nMatriz B:\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%4d", b[i][j]);
        }
        printf("\n");
    }

    //Creación de threads
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            datos[i][j] = (datos_thread){i, j, a, b, c};
            pthread_create(&threads[i][j], NULL, suma_matrices, &datos[i][j]);
        }
    }


    //Finalización de threads
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            pthread_join(threads[i][j], NULL);
        }
    }

    //Mostrar matriz C (suma de A + B)
    printf("\nMatriz C:\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%4d", c[i][j]);
        }
        printf("\n");
    }

    return 0;
}