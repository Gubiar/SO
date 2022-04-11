/*
* @author Gustavo Matos Lázaro 
*/


#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#include <time.h>
#include <sys/time.h>


#define N 3 //Numero de linhas e colunas das matrizes

MPI_Status status;


double matrix_a[N][N],matrix_b[N][N],matrix_c[N][N];

int main(int argc, char **argv) {

  int contadorProcessos;
  int IDProcessos;
  int contadorProcessosSecundarios;
  int entrada;
  int saida;
  int linhas;
  int deslocamento;

  struct timeval start, stop;

// Inicializaçao do MPI
  MPI_Init(&argc, &argv);

// Cada processo recebe um ID unico
  MPI_Comm_rank(MPI_COMM_WORLD, &IDProcessos);

// Numero de processos abertos é atribuido a variavel -> contadorProcessos
  MPI_Comm_size(MPI_COMM_WORLD, &contadorProcessos);

// Numero de processos secundários é atreibuido a variavel -> contadorProcessosSecundarios
  contadorProcessosSecundarios = contadorProcessos - 1;

// Processo ROOT (pai)
 if (IDProcessos == 0) {
	
// Matriz A e B são populadas com numeros randomicos
    srand ( time(NULL) );
    for (int i = 0; i<N; i++) {
      for (int j = 0; j<N; j++) {
        matrix_a[i][j]= rand()%10;
        matrix_b[i][j]= rand()%10;
      }
    }
	
  printf("\n\t\tMultiplicacao de matriz usando MPI\n");

// Matriz A
    printf("\nMatrix A\n\n");
    for (int i = 0; i<N; i++) {
      for (int j = 0; j<N; j++) {
        printf("%.0f\t", matrix_a[i][j]);
      }
	    printf("\n");
    }

// Matriz B
    printf("\nMatrix B\n\n");
    for (int i = 0; i<N; i++) {
      for (int j = 0; j<N; j++) {
        printf("%.0f\t", matrix_b[i][j]);
      }
	    printf("\n");
    }

// Determina o numero de linhas da matriz A que será enviada para cada processo secundario
    linhas = (N + contadorProcessosSecundarios -1)/contadorProcessosSecundarios;

    printf("\nLinhas %d\n", linhas);

// Variável deslocamento determina o ponto inicial da linha que foi enviada para o processo secundario
    deslocamento = 0;

// O calculo é feito nos processos secundarios

    for (saida=1; saida <= contadorProcessosSecundarios; saida++)
    {
      // Delocamento de A
      MPI_Send(&deslocamento, 1, MPI_INT, saida, 1, MPI_COMM_WORLD);

      // Numero de linhas
      MPI_Send(&linhas, 1, MPI_INT, saida, 1, MPI_COMM_WORLD);

      // Envia as linhas da Matriz A para os processos secundarios
      MPI_Send(&matrix_a[deslocamento][0], linhas*N, MPI_DOUBLE,saida,1, MPI_COMM_WORLD);

      // Matriz B
      MPI_Send(&matrix_b, N*N, MPI_DOUBLE, saida, 1, MPI_COMM_WORLD);
      
      // Deslocamento é atualizado
      deslocamento = deslocamento + linhas;
    }

// O processo ROOT espera o resultados dos secundarios para continuar
    for (int i = 1; i <= contadorProcessosSecundarios; i++)
    {
      entrada = i;

      // Recebe o deslocamento dos processos secundarios
      MPI_Recv(&deslocamento, 1, MPI_INT, entrada, 2, MPI_COMM_WORLD, &status);

      // Recebe o numero de linhas que cada processo secundarios processou
      MPI_Recv(&linhas, 1, MPI_INT, entrada, 2, MPI_COMM_WORLD, &status);

      // As linhas calculadas são armazenadas na Matriz final C
      MPI_Recv(&matrix_c[deslocamento][0], linhas*N, MPI_DOUBLE, entrada, 2, MPI_COMM_WORLD, &status);
    }

// Resultado
    printf("\nResult Matrix C = Matrix A * Matrix B:\n\n");
    for (int i = 0; i<3; i++) {
      for (int j = 0; j<3; j++)
        printf("%.0f\t", matrix_c[i][j]);
      printf ("\n");
    }
    printf ("\n");
  }

// Processos secundarios
  if (IDProcessos > 0) {

    entrada = 0;

    // O processo secundário recebe os buffers com tag 1 ( Linha 100) para calcular

    // Valor de deslocamento
    MPI_Recv(&deslocamento, 1, MPI_INT, entrada, 1, MPI_COMM_WORLD, &status);

    // Numero de linhas
    MPI_Recv(&linhas, 1, MPI_INT, entrada, 1, MPI_COMM_WORLD, &status);

    // Sub matriz
    MPI_Recv(&matrix_a, linhas*N, MPI_DOUBLE, entrada, 1, MPI_COMM_WORLD, &status);

    // Matriz B
    MPI_Recv(&matrix_b, N*N, MPI_DOUBLE, entrada, 1, MPI_COMM_WORLD, &status);

    // Calculo

    for (int k = 0; k<N; k++) {
      for (int i = 0; i<linhas; i++) {
 
        matrix_c[i][k] = 0.0; //Popula a matriz inicialmente
        
        for (int j = 0; j<N; j++)
          matrix_c[i][k] = matrix_c[i][k] + matrix_a[i][j] * matrix_b[j][k]; //Atribui o valor
      }
    }

    // O resultado calculado será enviado de volta ao processo ROOT com tag de mensagem 2
    
    // Retorna o deslocamento
    MPI_Send(&deslocamento, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);

    // Retorna o numero de linhas processado
    MPI_Send(&linhas, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);

    // Retorna a matriz resultante
    MPI_Send(&matrix_c, linhas*N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
  }

  MPI_Finalize();
}