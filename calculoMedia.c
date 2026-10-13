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
    float soma_local, media_local;
    float soma_total, media_total;
    float *local_data = NULL;
    // float *total_data = NULL;
    MPI_Init(&argc, &argv); // inicializa MPI

    MPI_Comm_rank(MPI_COMM_WORLD, &id); // obtem rank/id
    MPI_Comm_size(MPI_COMM_WORLD, &num_processos);// obte, número total de processadores em excecuções
    // 1. Cada processo deve gerar um vetor local de `N` valores aleatórios entre 0 e 1.
    local_data = (float *)malloc(N * sizeof(float));
    for(int i = 0; i<N; i++){
        local_data[i] = rand() / (float)RAND_MAX;
    }

    // 2. soma e media local
    for(int j=0;j<N;j++){
        soma_local += local_data[j];
    }
    media_local = soma_local/N;
    // 4. a saída
    if(id == 0){
        printf("Processo %d: soma%f, media%f: ", id, soma_local, media_local);
        printf("\n");
        // 3. Soma global
        MPI_Reduce(&soma_local, 
            &soma_total, 
            1, 
            MPI_FLOAT, 
            MPI_SUM, 
            0, 
            MPI_COMM_WORLD);
        
        for(int k = 0; k<N;k++){
            printf("[Processo %d] Soma_local: %f, Média local: %f\n", k, soma_local, media_local);
        }
        printf("[Soma global] %f", soma_total);
        printf("[Média global] %f", media_total);
    }
    free(local_data) //libera memória
    MPI_Finalize(); // finaliza MPI
    return 0;
}