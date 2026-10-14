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

     Use MPI_Gather para coletar todas as somas locais no processo 0.
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int id, num_processos, N;
    float soma_local = 0.0f, media_local;
    float soma_total, media_total;
    float *local_data = NULL;

    // inicio do MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processos);

    // pegar input N
    if (argc != 2) {
        if (id == 0){
            printf("Uso: mpirun -np P ./media_mpi N\n");
        }
        MPI_Finalize();
        return 1;
    }
    N = atoi(argv[1]);

    // vetor local aleatório de cada proceso
    local_data = (float *)malloc(N * sizeof(float));
    srand(time(NULL) + id);
    for (int i = 0; i < N; i++) {
        local_data[i] = rand() / (float)RAND_MAX;
    }
    
    // soma e média local
    for (int j = 0; j < N; j++) {
        soma_local += local_data[j];
    }
    media_local = soma_local / N;

    // soma global
    MPI_Reduce(&soma_local, 
        &soma_total, 
        1, 
        MPI_FLOAT, 
        MPI_SUM, 
        0, 
        MPI_COMM_WORLD);

    
    // coleta soma/média de cada processo
    float *somas = NULL, *medias = NULL;
    if (id == 0) {
        somas  = (float *)malloc(num_processos * sizeof(float));
        medias = (float *)malloc(num_processos * sizeof(float));
    }
    MPI_Gather(&soma_local,  1, MPI_FLOAT, somas,  1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Gather(&media_local, 1, MPI_FLOAT, medias, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    //saída
    if (id == 0) {
        for (int i = 0; i < num_processos; i++) {
            printf("[Processo %d] Soma local: %f, Média local: %f\n", i, somas[i], medias[i]);
        }
        media_total = soma_total / (N * num_processos);
        printf("[Soma global] %f\n", soma_total);
        printf("[Média global] %f\n", media_total);
        free(somas);
        free(medias);
    }

    free(local_data);
    MPI_Finalize();
    return 0;
}