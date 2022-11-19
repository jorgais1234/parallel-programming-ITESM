#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <omp.h>
#include <math.h>
#define NUM_THREADS    4
#define N 7 

float tempnumeros [7][7]={{-1.0,-1,-1,-1,-1,-1,-1,},
                        {-1,-1,-1,-1,-1,-1,-1,},
                        {-1,-1,-1,-1,-1,-1,-1,},
                        {-1,-1,-1,-1,-1,-1,-1,},
                        {-1,-1,-1,-1,-1,-1,-1,},
                        {-1,-1,-1,-1,-1,-1,-1,},
                        {-1,-1,-1,-1,-1,-1,-1,}}; 

char temperatura[N][N] =   {{'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                     {'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                     {'C', 'C', 'H', 'H', 'H', 'C', 'C'},
                     {'C', 'C', 'H', 'H', 'H', 'C', 'C'},
                     {'C', 'C', 'H', 'H', 'H', 'C', 'C'},
                     {'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                     {'C', 'C', 'C', 'C', 'C', 'C', 'C'}};
int _Thread; 
//Funcion para crear la matriz de numeros dependiendo de lo ingresado en la matriz de char 
void *crearM(void *threadid){
    long tid; 
    tid = (long)threadid; 

    for (int i= 0; i<7; i++){
        for (int j=0; j<7; j++){

            if (temperatura[i][j]== 'C') {
                tempnumeros[i][j] = (rand() % 6100)/100.0f; ; 
            }
            if (temperatura[i][j]== 'H'){
                tempnumeros[i][j]= ((rand() %4100)+6000)/100.0f; 
            }   
        }
    }
    pthread_exit(NULL); 


}

int main (int argc, char** argv){
// Declaramos todas las variables a utilizar 
    int rc; 
    long t; 

// Parametros necesarios para sacar la ecuacion de calor 
    int max_iter_time = 100;
    int alpha = 5;
    int delta_x = 2;
    float delta_t = (pow(delta_x, 2))/(4 * alpha);
    float gamma = (alpha * delta_t) / (delta_x*delta_x);

    int u[max_iter_time][7][7];
    int k; 
    int j; 
    int i; 


    pthread_t threads [NUM_THREADS]; 
    //inicializamos el random 
    srand(time(NULL));

    //Numero de threads default
    int thread_num = 1;
    //igualamos numero de threads al argumento 
    if (argc > 1){
        thread_num = atoi(argv[1]);
    }

//imprimimos la matriz de H y C 

    for (int i=0; i<7; i++) { //filas
        for (int j=0; j<7; j++) { //columnas
            printf("%c",temperatura[i][j]);
        }
        printf("\n"); 
    }
// Se manda a llamar la funcion que llena la matriz de temperatura usando los threads establecidos 
    for(t=0;t<NUM_THREADS;t++){
        printf(" threads used:  %ld\n", t);
        rc = pthread_create(&threads[t], NULL, crearM, (void *)t);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
// Se imprime la matriz de numeros generada por los threads
    for (int i=0; i<7; i++) { //filas
        for (int j=0; j<7; j++) { //columnas
            printf("%1.2f ! ",tempnumeros[i][j]);
            
        }

        printf("\n"); 
    }
  
    #pragma omp parallel for private(k, i, j) shared (u, tempnumeros)
    for( k = 0; k < max_iter_time; k++){
        for( i = 0; i < 7; i++){
        for( j = 0; j < 7; j++){
            u[k][i][j] = tempnumeros[i][j];
        }
        }
    }

    #pragma omp parallel for private(k, i, j) shared (u)
    for( k = 0; k < max_iter_time - 1; k++){
        for( i = 1; i < 7 - 1; i+= delta_x){
        for( j = 1; j < 7 - 1; j+= delta_x){
            u[k + 1][i][j] = gamma * (u[k][i+1][j] + u[k][i-1][j] + u[k][i][j+1] + u[k][i][j-1] - 4*u[k][i][j]) + u[k][i][j];
        }
        }
    }

    printf("\nDear manager:");
    printf("\nThis is the heat equation for the factory:");
    printf("\ny = %d * (%f / (%d)^2)\n", alpha, delta_t, delta_x);



    return (0);
    pthread_exit(NULL);

}