/*
Ana Julia Yaguti Matilha
Carolina Lee
Pedro Casas Pequeno Junior
*/

/*
Enunciado:
Entrada:
O programa deve receber, via linha de comando, **um único argumento inteiro**, N: o tamanho do vetor local que será gerado por cada processo

Exemplo de uso:
mpirun -np 4 ./media_mpi 1000

Neste caso, cada um dos 4 processos criará um vetor local com 1000 valores aleatórios.

Requisitos
1. Cada processo deve gerar um vetor local de `N` valores aleatórios entre 0 e 1.
2. Cada processo deve calcular:
   - A **soma local** de seus valores
   - A **média local**
3. Utilizar `MPI_Reduce` para:
   - Obter a **soma global** de todos os dados no processo de rank 0
4. O processo de rank 0 deve:
   - Calcular a **média global**
   - Exibir:
     - A soma local e média local de todos os processos (podem ser enviados com `MPI_Gather` ou impressos individualmente)
     - A soma global
     - A média global

*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int id, num_processos; 
    int N;
    float *data = NULL;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processos);
    // printf("Hello, I am process %d of %d\n", id, num_processos);
    // 1. Cada processo deve gerar um vetor local de `N` valores aleatórios entre 0 e 1.
    data = (float *)malloc(N * sizeof(float));
    for(int i = 0; i<N; i++){
        data[i] = rand() / (float)RAND_MAX;
    }
    free(data)
    MPI_Finalize();
    return 0;
}