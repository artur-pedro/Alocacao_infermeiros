#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
  int **MatrizCusto;
  int TotalDeAlas;
} MatrizAlaEnfermeiro;

// Função para ler a matriz do arquivo
int LerMatrizDoArquivo(const char *arqNome, MatrizAlaEnfermeiro *matriz) {
  FILE *arquivo = fopen(arqNome, "r");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo");
    return -1;
  }

  // Ler a quantidade de alas da primeira linha do arquivo
  fscanf(arquivo, "%d", &matriz->TotalDeAlas);

  // Alocar memória para a matriz
  matriz->MatrizCusto = (int **)malloc(matriz->TotalDeAlas * sizeof(int *));
  for (int i = 0; i < matriz->TotalDeAlas; i++) {
    matriz->MatrizCusto[i] = (int *)malloc((matriz->TotalDeAlas + 1) * sizeof(int)); // Adiciona uma coluna extra
  }

  // Colocar os valores do arquivo na matriz
  for (int i = 0; i < matriz->TotalDeAlas; i++) {
    for (int j = 0; j < matriz->TotalDeAlas; j++) {
      fscanf(arquivo, "%d", &matriz->MatrizCusto[i][j]);
    }
    matriz->MatrizCusto[i][matriz->TotalDeAlas] = rand() % 101 + 100; // Preenche a coluna extra com números aleatórios entre 100 e 200
  }

  fclose(arquivo);
  return 0;
}

// Função para encontrar os menores valores
void EncontrarMenoresValores(MatrizAlaEnfermeiro *matriz, int *menoresValores) {
  // Aloca memória para o vetor de colunas ainda não usadas
  int *colunasRestantes = (int *)malloc((matriz->TotalDeAlas + 1) * sizeof(int));
  for (int i = 0; i < matriz->TotalDeAlas + 1; i++) {
    colunasRestantes[i] = i;
  }

  for (int i = 0; i < matriz->TotalDeAlas; i++) {
    int menorValor = matriz->MatrizCusto[i][colunasRestantes[0]];
    int colunaMenorValor = colunasRestantes[0];

    // Encontrar o menor valor na linha atual
    for (int j = 1; j < matriz->TotalDeAlas + 1; j++) {
      if (matriz->MatrizCusto[i][colunasRestantes[j]] < menorValor) {
        menorValor = matriz->MatrizCusto[i][colunasRestantes[j]];
        colunaMenorValor = colunasRestantes[j];
      }
    }

    // Remover a coluna usada para que não seja considerada nas próximas
    // linhas
    for (int k = 0; k < matriz->TotalDeAlas + 1; k++) {
      if (colunasRestantes[k] == colunaMenorValor) {
        for (int l = k; l < matriz->TotalDeAlas; l++) {
          colunasRestantes[l] = colunasRestantes[l + 1];
        }
        break;
      }
    }

    // Cópia do menor valor para o vetor menoresValores
    menoresValores[i] = menorValor;
  }

  // Liberar a memória alocada para colunasRestantes
  free(colunasRestantes);
}

// Função para somar os custos das alas
int SomarCustoDasAlas(int menorescustos[], int dimensaovetor) {
  if (dimensaovetor == 0)
    return 0;
  else
    return menorescustos[dimensaovetor - 1] +
           SomarCustoDasAlas(menorescustos, dimensaovetor - 1);
}

int main() {
  setlocale(LC_ALL, "Portuguese");

  // Inicializa a semente para a função rand()
  srand(time(NULL));

  MatrizAlaEnfermeiro matrizEnfermeiroAlas;
  char arqNome[100] = "D_10.txt";
  int *menoresValores = (int *)malloc(matrizEnfermeiroAlas.TotalDeAlas * sizeof(int));

  // Chama a função para ler os dados do arquivo
  LerMatrizDoArquivo(arqNome, &matrizEnfermeiroAlas);

  // Encontra o menor custo p/ enfermeiros e as alas
  EncontrarMenoresValores(&matrizEnfermeiroAlas, menoresValores);

  // Chamar a função para somar os custos das alas
  int somaAlas = SomarCustoDasAlas(menoresValores, matrizEnfermeiroAlas.TotalDeAlas);
  printf("A menor atribuição de enfermeiros (as) a alas é: %d\n", somaAlas);

  // Liberar a memória alocada para a matriz e menoresValores
  for (int i = 0; i < matrizEnfermeiroAlas.TotalDeAlas; i++)
    free(matrizEnfermeiroAlas.MatrizCusto[i]);
  free(matrizEnfermeiroAlas.MatrizCusto);
  free(menoresValores);

  return 0;
}