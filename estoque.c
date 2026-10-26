#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -- STRUCTS --//
typedef struct {
    int id;
    char nome[50];
    char telefone[20];
} Fornecedor;

typedef struct {
    int id;
    char nome[50];
    float preco;
    int qnt;
    Fornecedor fornecedor;
    int ativo;
} Produto;

typedef struct {
    Produto *produtos;
    int qtdAtual;
    int capacidade;
} Estoque;

void inicializarEstoque(Estoque *estoque);
void verificarEExpandirVetor(Estoque *estoque);
void cadastrar(Estoque *estoque);
int buscarProduto(Produto *vetor, int qtd, int codigoBusca);
void alterarStatusProduto(Produto *vetor, int qtd, int codigoBusca);
void gerarRelatorio(Estoque *estoque);
void listar(Estoque *estoque);
void salvarDadosBinario(Produto *vetor, int qtd, const char *nomeArquivo);
void carregarDadosBinario(Estoque *estoque, const char *nomeArquivo);
void liberarMemoria(Estoque *estoque);


// -- Função de cadastrar --//
void cadastrar(Estoque *estoque) {
    printf("---CADASTRO DE PRODUTO---\n");

    verificarEExpandirVetor(estoque);

    int index = estoque->qtdAtual;
    int i;

    int idDigitado, estoqueDigitado;
    float precoDigitado;

    float idTemp, qntTemp;

    printf("Digite o ID: ");

    if (scanf("%f", &idTemp) != 1) {
        printf("\nErro: digite um ID valido\n");
        while (getchar() != '\n' && !feof(stdin));
        return;
    }
    getchar();

    // Impede numeros quebrados no ID
    if (idTemp != (int)idTemp) {
        printf("\nErro: O ID deve ser um numero inteiro\n");
        return;
    }

    idDigitado = (int)idTemp;

    // Verificar se o ID e negativo
    if (idDigitado <= 0) {
        printf("\nErro: ID nao pode ser zero ou negativo\n");
        return;
    }

    // Verificar se o ID nao esta duplicado
    for (i = 0; i < estoque->qtdAtual; i++) {
        if (estoque->produtos[i].id == idDigitado) {
            printf("\nErro: ID ja cadastrado\n");
            return;
        }
    }

    estoque->produtos[index].id = idDigitado;

    printf("Digite o nome do produto: ");
    fgets(estoque->produtos[index].nome,
          sizeof(estoque->produtos[index].nome), stdin);

    estoque->produtos[index].nome[
        strcspn(estoque->produtos[index].nome, "\n")] = '\0';

    printf("Digite o Preco: R$ ");

    if (scanf("%f", &precoDigitado) != 1) {
        printf("Erro: digite um preco valido.\n");
        while (getchar() != '\n' && !feof(stdin));
        return;
    }
    getchar();

    // Verificar se o preco e negativo
    if (precoDigitado < 0) {
        printf("\nO preco nao pode ser negativo\n");
        return;
    }

    estoque->produtos[index].preco = precoDigitado;

    printf("Digite a quantia em estoque: ");

    if (scanf("%f", &qntTemp) != 1) {
        printf("\nErro: digite um valor valido\n");
        while (getchar() != '\n' && !feof(stdin));
        return;
    }
    getchar();

    // Impede numeros quebrados no estoque
    if (qntTemp != (int)qntTemp) {
        printf("\nErro: A quantidade deve ser um numero inteiro\n");
        return;
    }

    estoqueDigitado = (int)qntTemp;

    // Verificar se a quantidade do estoque e negativa
    if (estoqueDigitado < 0) {
        printf("\nErro: estoque nao pode ser negativo\n");
        return;
    }

    estoque->produtos[index].qnt = estoqueDigitado;

    // -- DADOS DO FORNECEDOR --//
    printf("\n--- DADOS DO FORNECEDOR ---\n");

    printf("Digite o ID do fornecedor: ");

    if (scanf("%d", &estoque->produtos[index].fornecedor.id) != 1) {
        printf("Erro: ID do fornecedor invalido.\n");
        while (getchar() != '\n' && !feof(stdin));
        return;
    }
    getchar();

    if (estoque->produtos[index].fornecedor.id <= 0) {
        printf("Erro: o ID do fornecedor deve ser positivo.\n");
        return;
    }

    printf("Digite o nome do fornecedor: ");
    fgets(estoque->produtos[index].fornecedor.nome,
          sizeof(estoque->produtos[index].fornecedor.nome), stdin);

    estoque->produtos[index].fornecedor.nome[
        strcspn(estoque->produtos[index].fornecedor.nome, "\n")] = '\0';

    printf("Digite o telefone do fornecedor: ");
    fgets(estoque->produtos[index].fornecedor.telefone,
          sizeof(estoque->produtos[index].fornecedor.telefone), stdin);

    estoque->produtos[index].fornecedor.telefone[
        strcspn(estoque->produtos[index].fornecedor.telefone, "\n")] = '\0';

    estoque->produtos[index].ativo = 1;
    estoque->qtdAtual++;

    printf("\nProduto cadastrado com sucesso!\n");
}


// -- Função de buscar --//
int buscarProduto(Produto *vetor, int qtd, int codigoBusca) {
    int i;

    for (i = 0; i < qtd; i++) {
        if (vetor[i].id == codigoBusca) {
            return i;
        }
    }

    return -1;
}


// -- Alocação de espaço de memoria --//
void inicializarEstoque(Estoque *estoque) {
    estoque->capacidade = 10;
    estoque->qtdAtual = 0;

    estoque->produtos = malloc(estoque->capacidade * sizeof(Produto));

    if (estoque->produtos == NULL) {
        printf("Erro ao alocar memoria!\n");
        exit(1);
    }
}

void verificarEExpandirVetor(Estoque *estoque) {
    if (estoque->qtdAtual >= estoque->capacidade) {
        int novaCapacidade = estoque->capacidade * 2;

        Produto *temp = realloc(
            estoque->produtos,
            novaCapacidade * sizeof(Produto)
        );

        if (temp == NULL) {
            printf("Erro ao realocar memoria!\n");
            exit(1);
        }

        estoque->produtos = temp;
        estoque->capacidade = novaCapacidade;
    }
}


// -- Função de alterar --//
void alterarStatusProduto(Produto *vetor, int qtd, int codigoBusca) {
    int indice = buscarProduto(vetor, qtd, codigoBusca);

    if (indice != -1) {
        vetor[indice].ativo = !vetor[indice].ativo;

        printf("\nStatus do produto [%s] alterado com sucesso para: %s\n",
               vetor[indice].nome,
               vetor[indice].ativo ? "Ativo" : "Inativo");
    } else {
        printf("\nProduto com ID %d nao encontrado.\n", codigoBusca);
    }
}


// -- Função de relatorio --//
void gerarRelatorio(Estoque *estoque) {
    int i;
    int totalUnidades = 0;
    int ativos = 0;
    int inativos = 0;
    float valorTotal = 0.0f;

    printf("\n--- RELATORIO DO ESTOQUE ---\n");

    if (estoque->qtdAtual == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    for (i = 0; i < estoque->qtdAtual; i++) {
        Produto *p = &estoque->produtos[i];

        totalUnidades += p->qnt;
        valorTotal += p->preco * p->qnt;

        if (p->ativo)
            ativos++;
        else
            inativos++;
    }

    printf("Produtos cadastrados: %d\n", estoque->qtdAtual);
    printf("Total de unidades: %d\n", totalUnidades);
    printf("Produtos ativos: %d\n", ativos);
    printf("Produtos inativos: %d\n", inativos);
    printf("Valor estimado do estoque: R$ %.2f\n", valorTotal);
    printf("--------------------\n");
}


// -- Função de listar --//
void listar(Estoque *estoque) {
    int i;

    printf("\n---LISTA DE PRODUTOS---");

    if (estoque->qtdAtual == 0) {
        printf("\nNenhum produto cadastrado");
        return;
    }

    for (i = 0; i < estoque->qtdAtual; i++) {
        printf("\n------\n");
        printf("Produto %d:\n", i + 1);
        printf("ID: %d\n", estoque->produtos[i].id);
        printf("Nome: %s\n", estoque->produtos[i].nome);
        printf("Preco: %.2f\n", estoque->produtos[i].preco);
        printf("Quantidade: %d\n", estoque->produtos[i].qnt);
        printf("Fornecedor ID: %d\n",
               estoque->produtos[i].fornecedor.id);
        printf("Fornecedor: %s\n",
               estoque->produtos[i].fornecedor.nome);
        printf("Telefone: %s\n",
               estoque->produtos[i].fornecedor.telefone);
    }

    printf("------\n");
}


// -- ARQUIVOS --//
void salvarDadosBinario(Produto *vetor, int qtd, const char *nomeArquivo) {
    FILE *file = fopen(nomeArquivo, "wb");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo para gravacao.\n");
        return;
    }

    fwrite(&qtd, sizeof(int), 1, file);

    if (qtd > 0) {
        fwrite(vetor, sizeof(Produto), qtd, file);
    }

    fclose(file);
}

void carregarDadosBinario(Estoque *estoque, const char *nomeArquivo) {
    FILE *file = fopen(nomeArquivo, "rb");

    if (file == NULL) {
        inicializarEstoque(estoque);
        return;
    }

    int qtd;

    if (fread(&qtd, sizeof(int), 1, file) != 1 || qtd < 0) {
        fclose(file);
        inicializarEstoque(estoque);
        return;
    }

    estoque->capacidade = (qtd > 10) ? qtd * 2 : 10;
    estoque->qtdAtual = 0;

    estoque->produtos = malloc(estoque->capacidade * sizeof(Produto));

    if (estoque->produtos == NULL) {
        fclose(file);
        printf("Erro ao alocar memoria!\n");
        exit(1);
    }

    if (qtd > 0) {
        if (fread(estoque->produtos, sizeof(Produto),
                  qtd, file) != (size_t)qtd) {
            printf("Erro ao carregar os produtos.\n");
            free(estoque->produtos);
            fclose(file);
            inicializarEstoque(estoque);
            return;
        }
    }

    estoque->qtdAtual = qtd;

    fclose(file);
}

void liberarMemoria(Estoque *estoque) {
    free(estoque->produtos);
    estoque->produtos = NULL;
    estoque->qtdAtual = 0;
    estoque->capacidade = 0;
}


// -- MAIN --//
int main(void) {
    int op;
    int codigoBusca;
    int indice;
    Estoque estoque;

    carregarDadosBinario(&estoque, "estoque.dat");

    do {
        printf("\n---Menu--\n");
        printf("1 - Cadastrar\n");
        printf("2 - Buscar\n");
        printf("3 - Alterar\n");
        printf("4 - Relatorio\n");
        printf("5 - Listar\n");
        printf("0 - Sair\n");
        printf("Opcao: ");

        scanf("%d", &op);
        getchar();

        switch (op) {
            case 0:
                salvarDadosBinario(
                    estoque.produtos,
                    estoque.qtdAtual,
                    "estoque.dat"
                );

                printf("Encerrando...\n");
                break;

            case 1:
                cadastrar(&estoque);
                break;

            case 2:
                printf("Digite o ID do produto: ");
                scanf("%d", &codigoBusca);
                getchar();

                indice = buscarProduto(
                    estoque.produtos,
                    estoque.qtdAtual,
                    codigoBusca
                );

                if (indice != -1) {
                    printf("\nProduto encontrado!\n");
                    printf("ID: %d\n", estoque.produtos[indice].id);
                    printf("Nome: %s\n", estoque.produtos[indice].nome);
                    printf("Preco: R$ %.2f\n", estoque.produtos[indice].preco);
                    printf("Quantidade: %d\n", estoque.produtos[indice].qnt);
                    printf("Status: %s\n", estoque.produtos[indice].ativo ? "Ativo" : "Inativo");
					printf("ID do fornecedor: %d\n", estoque.produtos[indice].fornecedor.id);  
					printf("Nome do fornecedor: %s\n", estoque.produtos[indice].fornecedor.nome);
					printf("Telefone do fornecedor: %s\n", estoque.produtos[indice].fornecedor.telefone);              
				} 
				else {
                    printf("\nProduto nao encontrado!\n");
                }
                break;

            case 3:
                printf("Digite o ID do produto: ");
                scanf("%d", &codigoBusca);
                getchar();

                alterarStatusProduto(
                    estoque.produtos,
                    estoque.qtdAtual,
                    codigoBusca
                );
                break;

            case 4:
                gerarRelatorio(&estoque);
                break;

            case 5:
                listar(&estoque);
                break;

            default:
                printf("Opcao invalida\n");
        }

    } while (op != 0);

    liberarMemoria(&estoque);

    return 0;
}
```