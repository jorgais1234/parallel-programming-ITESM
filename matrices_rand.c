#include <stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<omp.h>
#include <time.h>   
int main(int argc, char** argv)
{   
   

    //Generamos las variables necesarias para los ciclos 
    int m, n, p, q, c, d, k, suma = 0;
    //Generamos todas las matrices necesarias 
    int primera_matriz[100][100], segunda_matriz[100][100], matriz_resultado[100][100];
    srand(time(0)); 
    //Se ingresa el numero de filas y  columnas de la primera matriz 
    printf("\nIngresar el numero de filas y columnas de la primera matriz:\n");
    scanf("%d%d", &m, &n);
     //Se ingresa el numero de filas y  columnas de la segunda matriz
    printf("\nIngresar el numero de filas y columnas de la segunda matriz:\n");
    scanf("%d%d", &p, &q);
    //Se checa si la multiplicacion de matrices es posible (numero de columnas de A == Numero de filas de B )
    if ( n != p )
    {
        printf("\nLas matrices que se introdujeron no pueden ser multiplicadas.\n");
        printf("\nEl numero de columnas de la primera matriz debe ser igual al de filas de la segunda.\n");
    }
    else
    {
        //Se general aleatoriamente los elementos de la primera matriz 
        
        for (  c = 0 ; c < m ; c++ )
            for ( d = 0 ; d < n ; d++ )
                primera_matriz[c][d]=rand()% 10 +1;
        //Se generan aleatoriamenre los elementos de la segunda matriz 
        
        for ( c = 0 ; c < p ; c++ )
            for ( d = 0 ; d < q ; d++ )
                segunda_matriz[c][d]=rand()% 10 +1;
       // se establece la thread inicia como 1 
       int threadscnt=1; 
       //se lee el argumento y se reescribe la cantidad de threads
            if(argc>1){
                threadscnt = atoi(argv[1]); 
            }
        omp_set_num_threads(threadscnt); 
        printf("OMP DEFINED, THREADCT = %d\n",threadscnt); 
        //Se ejecuta la multiplicacion de las matrices 
        //Se establecen las variables privadas 
        #pragma omp parallel for private (c,d,k) shared (primera_matriz, segunda_matriz, suma)
        for ( c = 0 ; c < m ; c++ )
        {
            for ( d = 0 ; d < q ; d++ )
            {
                for ( k = 0 ; k < p ; k++ )
                {
                    suma = suma + primera_matriz[c][k]*segunda_matriz[k][d];
                }

                matriz_resultado[c][d] = suma;
                suma = 0;
            }
        }
       
    }
    return 0;
}



//Codigo de ayuda 
//Matrix multiplication in. (s.f.). Programming simplified. Recuperado de https://www.programmingsimplified.com/c-program-multiply-matrices?adlt=strict&toWww=1&redig=1D0A155A618C49429741A14CE2DA6099



