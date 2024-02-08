#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAMANHO 1000

typedef struct Palavra {
  char *palavra;
  int *linha;
  int repeticoes;
  struct Palavra *proximo;
} Palavra;

typedef struct noArvore {
  char *palavra;
  int *linhas;
  int ocorrencias;
  struct noArvore *esquerda;
  struct noArvore *direita;
} noArvore;

typedef struct {
  char *linha;
  int indice;
} Linhas;

void liberaArvore(noArvore *raiz) {
  if (raiz != NULL)
  {
    liberaArvore(raiz->esquerda);
    liberaArvore(raiz->direita);
    free(raiz->palavra);
    free(raiz->linhas);
    free(raiz);
  }
}

void liberaLista(Palavra *inicio) {
  Palavra *att = inicio;
  while (att != NULL)
  {
    Palavra *temp = att;
    att = att->proximo;
    free(temp->palavra);
    free(temp->linha);
    free(temp);
  }
}

void corrige(char *str) {
  for (size_t i = 0; i < strlen(str); i++)
  {
    str[i] = tolower((unsigned char)str[i]);

    if (!isalnum((unsigned char)str[i]))
    {
      str[i] = '\0';
    }
  }
}

Palavra *criaPalavraLigada(Palavra *inicio, const char *palavra, int linha) {
  Palavra *nova = (Palavra *)malloc(sizeof(Palavra));

  nova->repeticoes = 1;
  nova->linha = (int *)malloc(sizeof(int));
  nova->linha[0] = linha;
  nova->palavra = strdup(palavra);
  nova->proximo = NULL;

  if (inicio == NULL) {
    return nova;
  }

  Palavra *att = inicio;
  Palavra *anterior = NULL;

  while (att != NULL && strcmp(palavra, att->palavra) > 0) {
    anterior = att;
    att = att->proximo;
  }

  if (att != NULL && strcmp(palavra, att->palavra) == 0) {
    int linhaEncontrada = 0;

    att->repeticoes++;
    att->linha = realloc(att->linha, sizeof(int) * att->repeticoes);
    att->linha[att->repeticoes - 1] = linha;

    free(nova->palavra);
    free(nova->linha);
    free(nova);
  }
  else {
    nova->proximo = att;
    if (anterior == NULL) {
      return nova;
    }
    else {
      anterior->proximo = nova;
    }
  }
  return inicio;
}

void buscaLigada(Palavra *indice, const char *palavraBuscada, Linhas *linhas) {
  char palavraBuscadaCorrigida[TAMANHO];
  strcpy(palavraBuscadaCorrigida, palavraBuscada);
  corrige(palavraBuscadaCorrigida);

  Palavra *copia = indice;

  while (copia != NULL) {
    char palavraListaCorrigida[TAMANHO];
    strcpy(palavraListaCorrigida, copia->palavra);
    corrige(palavraListaCorrigida);

    if (strcmp(palavraListaCorrigida, palavraBuscadaCorrigida) == 0) {
      printf("Existem %d ocorrência(s) da palavra '%s' na(s) seguinte(s) linha(s):\n", copia->repeticoes, palavraBuscadaCorrigida);

      int *linhasImpressas = (int *)malloc(sizeof(int) * copia->repeticoes);
      int contadorLinhasImpressas = 0;

      for (int j = 0; j < copia->repeticoes; j++) {
        int linha = copia->linha[j];

        int linhaJaImpressa = 0;
        for (int k = 0; k < contadorLinhasImpressas; k++) {
          if (linhasImpressas[k] == linha) {
            linhaJaImpressa = 1;
            break;
          }
        }

        if (!linhaJaImpressa) {
          printf("%05d: %s\n", linha + 1, linhas[linha].linha);
          linhasImpressas[contadorLinhasImpressas++] = linha;
        }
      }

      free(linhasImpressas);
      printf("\n");
      return;
    }

    copia = copia->proximo;
  }

  printf("Palavra %s nao encontrada.\n", palavraBuscadaCorrigida);
}

noArvore *raiz = NULL;

noArvore *criarNo(const char *palavra, int linha) {
  noArvore *novoNo = (noArvore *)malloc(sizeof(noArvore));
  novoNo->palavra = strdup(palavra);
  novoNo->ocorrencias = 1;
  novoNo->linhas = (int *)malloc(sizeof(int));
  novoNo->linhas[0] = linha;
  novoNo->esquerda = NULL;
  novoNo->direita = NULL;
  return novoNo;
}

noArvore *inserePalavra(noArvore *raiz, const char *palavra, int linha) {
  if (raiz == NULL) {
    noArvore *novoNo = criarNo(palavra, linha);
    return novoNo;
  }

  int comparaResultado = strcmp(raiz->palavra, palavra);
  if (comparaResultado == 0) {
    // A palavra já existe, adiciona a linha e incrementa as ocorrências
    raiz->ocorrencias++;
    raiz->linhas = (int *)realloc(raiz->linhas, sizeof(int) * raiz->ocorrencias);
    raiz->linhas[raiz->ocorrencias - 1] = linha;
  }
  else if (comparaResultado < 0) {
    raiz->esquerda = inserePalavra(raiz->esquerda, palavra, linha);
  }
  else {
    raiz->direita = inserePalavra(raiz->direita, palavra, linha);
  }

  return raiz;
}

void resultadoArvore(noArvore *raiz, const char *palavra, Linhas *linhas) {
  if (raiz != NULL) {
    int comparaResultado = strcmp(raiz->palavra, palavra);
    if (comparaResultado == 0) {
      printf("Existem %d ocorrência(s) da palavra '%s' na(s) seguinte(s) linha(s):\n", raiz->ocorrencias, palavra);

      int *linhasImpressas = (int *)malloc(sizeof(int) * raiz->ocorrencias);
      int contadorLinhasImpressas = 0;

      for (int i = 0; i < raiz->ocorrencias; i++) {
        int linha = raiz->linhas[i];

        int linhaJaImpressa = 0;
        for (int j = 0; j < contadorLinhasImpressas; j++) {
          if (linhasImpressas[j] == linha) {
            linhaJaImpressa = 1;
            break;
          }
        }

        if (!linhaJaImpressa) {
          printf("%05d: %s\n", linha, linhas[linha - 1].linha);
          linhasImpressas[contadorLinhasImpressas++] = linha;
        }
      }

      free(linhasImpressas);
    }
    else if (comparaResultado < 0) {
      resultadoArvore(raiz->esquerda, palavra, linhas);
    }
    else {
      resultadoArvore(raiz->direita, palavra, linhas);
    }
  }
  else {
    printf("Palavra %s nao encontrada.\n", palavra);
  }
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Uso: %s <nome_arquivo> <tipo_indice>\n", argv[0]);
    return 1;
  }

  char *palavra;
  FILE *in;
  char *linha;
  char *quebra_de_linha;
  Linhas *linhas = NULL;
  int contador_linha;
  Palavra *novaPalavra = NULL;
  Palavra *indice = NULL;
  clock_t tempo = clock();
  char *guardaPonteiro;

  in = fopen(argv[1], "r");

  if (!in) {
    perror("Erro ao abrir o arquivo");
    return 1;
  }

  contador_linha = 0;
  linha = (char *)malloc((TAMANHO + 1) * sizeof(char));

  while (fgets(linha, TAMANHO, in)) {
    if ((quebra_de_linha = strrchr(linha, '\n'))) *quebra_de_linha = 0;
    contador_linha++;
  }

  rewind(in);

  linhas = (Linhas *)malloc(sizeof(Linhas) * contador_linha);

  for (int i = 0; i < contador_linha; i++) {
    if (fgets(linha, TAMANHO, in) != NULL) {
      if ((quebra_de_linha = strrchr(linha, '\n'))) *quebra_de_linha = 0;

      linhas[i].linha = strdup(linha);
      linhas[i].indice = i + 1;

    }
    else {
      perror("Erro ao ler uma linha do arquivo");
      fclose(in);
      free(linha);
      free(linhas);
      return 1;
    }
  }

  printf("Tipo de indice: '%s'\n", argv[2]);
  printf("Arquivo texto: '%s'\n", argv[1]);

  for (int i = 0; i < contador_linha; i++) {
    guardaPonteiro = strdup(linhas[i].linha);
    char *palavra = strtok(guardaPonteiro, " -/");
    while (palavra != NULL) {
      corrige(palavra);

      raiz = inserePalavra(raiz, palavra, i + 1);

      indice = criaPalavraLigada(indice, palavra, i);

      palavra = strtok(NULL, " -/");
    }

    free(guardaPonteiro);
  }

  tempo = clock() - tempo;

  printf("tempo para carregar o arquivo e construir o indice: %lf ms\n", ((double)tempo) / CLOCKS_PER_SEC);

  char tipoIndice[TAMANHO];
  strcpy(tipoIndice, argv[2]);

  char comando[TAMANHO];
  char palavraBuscada[TAMANHO];

  while (1) {
    printf("> ");

    if (fgets(comando, TAMANHO, stdin) == NULL) {
      printf("Erro ao ler o comando.\n");
      break;
    }

    comando[strcspn(comando, "\n")] = '\0';

    if (strstr(comando, "busca") != NULL) {
      sscanf(comando, "busca %[^\n]", palavraBuscada);

      tempo = clock();
      if (strcmp(tipoIndice, "arvore") == 0) {
        resultadoArvore(raiz, palavraBuscada, linhas);
      }
      else if (strcmp(tipoIndice, "lista") == 0) {
        buscaLigada(indice, palavraBuscada, linhas);
      }
      else {
        printf("Tipo de indice invalido!\n");
      }
      tempo = clock() - tempo;
      printf("Tempo de busca: %f ms\n", ((double)tempo) / CLOCKS_PER_SEC);
    }
    else if (strcmp(comando, "fim") == 0) {
      break;
    }
    else {
      printf("Comando invalido!\n");
    }
  }

  free(linha);
  free(linhas);
  liberaArvore(raiz);
  liberaLista(indice);

  fclose(in);

  return 0;
}