#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUTOS 50

typedef struct {
    int id;
    char nome[50];
    char telefone[20];
} Fornecedor;

typedef struct{
	int id;
	char nome[50];
	float preco;
	int qnt;
	Fornecedor fornecedor;
	}Produto;

typedef struct{
	Produto produtos[MAX_PRODUTOS];
	int qtdAtual;
}Estoque;

void cadastrar(Estoque *estoque){
	printf("---CADASTRO DE PRODUTO---\n");
	
	if (estoque->qtdAtual >=MAX_PRODUTOS){
		printf("Erro: estoque esta cheio");
		return;
	}
	
	int index= estoque->qtdAtual;
	int i;
	
	int idDigitado, estoqueDigitado;
	float precoDigitado;
	
	float idTemp, qntTemp;
	
	printf("Digite o ID: ");
	if (scanf("%f", &idTemp) != 1){
		printf("\nErro: digite um ID valido\n");
		while (getchar() != '\n');
		return;
	}
	getchar();
		
	// Impede numeros quebrados no ID
	if (idTemp != (int)idTemp) {
		printf("\nErro: O ID deve ser um numero inteiro\n");
		return;
	}
	
	idDigitado = (int)idTemp;
	
	//Verificar se o ID e negativo
	if(idDigitado<=0){
		printf("\nErro: ID nao pode ser zero ou negativo\n");
		return;
	}
	
	//Verificar se o ID nao esta duplicado
	for (i=0; i<estoque->qtdAtual;i++){
		if(estoque->produtos[i].id == idDigitado){
			printf("\nErro: ID ja cadastrado\n");
			return;
		}
	}
	estoque->produtos[index].id = idDigitado;
	
	printf("Digite o nome do produto: ");
	fgets(estoque->produtos[index].nome, sizeof(estoque->produtos[index].nome), stdin);
	estoque->produtos[index].nome[strcspn(estoque->produtos[index].nome, "\n")] = '\0';
	
	printf("Digite o Preco: R$ ");
	if (scanf("%f", &precoDigitado) != 1) {
    printf("Erro: digite um preco valido.\n");
    while (getchar() != '\n' && !feof(stdin));
    return;
	}
	getchar();
	
	//Verificar se o preco e negativo
	if (precoDigitado<0){
		printf("\nO preco nao pode ser negativo\n");
		return;
	}
	estoque->produtos[index].preco = precoDigitado;
	
	printf("Digite a quantia em estoque: ");
	if (scanf("%f", &qntTemp) != 1){
		printf("\nErro: digite um valor valido\n");
		while (getchar() != '\n');
		return;
	}
	getchar();
	
	// Impede numeros quebrados no estoque
	if (qntTemp != (int)qntTemp) {
		printf("\nErro: A quantidade deve ser um numero inteiro\n");
		return;
	}
	
	estoqueDigitado = (int)qntTemp;
	
	//Verificar se a quantidade do estoque e negativo
	if(estoqueDigitado<0){
		printf("\nErro: estoque nao pode ser negativo\n");
		return;
	}
	
	estoque->produtos[index].qnt = estoqueDigitado;
	
	estoque->qtdAtual++;
	printf("\nProduto cadastrado com sucesso!\n");
}

void listar(Estoque *estoque){
	int i;
	printf("\n---LISTA DE PRODUTOS---");
	
	if(estoque->qtdAtual==0){
		printf("\nNenhum produto cadastrado");
		return;
	}
	
	for(i=0; i<estoque->qtdAtual;i++){
		printf("\n------\n");
		printf("Produto %d:\n", i+1);
		printf("ID: %d\n", estoque->produtos[i].id);
		printf("Nome: %s\n", estoque->produtos[i].nome);
		printf("Preco: %.2f\n", estoque->produtos[i].preco);
		printf("Quantidade: %d\n", estoque->produtos[i].qnt);
		}
		printf("------\n");
}



int main(void){
	int op;
	Estoque estoque;

	estoque.qtdAtual=0;
    do{
        printf("\n---Menu--\n");
        printf("1 - Cadastrar\n");
        printf("2 - Buscar\n");
        printf("3 - Atualizar\n");
        printf("4 - Remover\n");
        printf("5 - Listar\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &op);
        getchar();
        
        switch(op){
        	case 0:
        		printf("Encerrando...\n");
        		break;
        	case 1:
        		cadastrar(&estoque);
        		break;
        		
        	case 2:
        		break;
        		
        	case 3:
        		break;
        		
        	case 4:
        		break;
        		
        	case 5:
        		listar(&estoque);
        		break;
        		
        	default:
        		printf("Opcao invalida\n");
    	}
    }while(op!=0);
		
    //free();
	//=NULL;
   return 0; 
}
// INTEGRANTE 2
void inicializarEstoque(Produto **vetor, int *capacidade, int *qtd){
    *capacidade = 10;
    *qtd = 0;
    *vetor = (Produto *) malloc((*capacidade) * sizeof(Produto));
    if(*vetor == NULL){
        printf("Erro ao alocar memoria!\n");
        exit(1);
    }
}

void verificarEExpandirVetor(Produto **vetor, int *capacidade, int qtdAtual){
    if(*capacidade == 0){
        inicializarEstoque(vetor, capacidade, &qtdAtual);
        return;
    }
    if(qtdAtual >= *capacidade){
        *capacidade *= 2;
        Produto *temp = (Produto *) realloc(*vetor, (*capacidade) * sizeof(Produto));
        if(temp == NULL){
            printf("Erro ao realocar memoria!\n");
            exit(1);
        }
        *vetor = temp;
    }
}

int buscarProduto(Produto *vetor, int qtd, int codigoBusca){
    int i;
    for (i = 0; i < qtd; i++){
        if(vetor[i].id == codigoBusca){
            return i;
        }
    }
    return -1;
}

void alterarStatusProduto(Produto *vetor, int qtd, int codigoBusca){
    int indice = buscarProduto(vetor, qtd, codigoBusca);
    if(indice != -1){
        vetor[indice].ativo = !vetor[indice].ativo;
        printf("\nStatus do produto [%s] alterado com sucesso para: %s\n", vetor[indice].nome, vetor[indice].ativo ? "Ativo" : "Inativo");
    }else{
        printf("\nProduto com ID %d nao encontrado.\n", codigoBusca);
    }
}

void salvarDadosBinario(Produto *vetor, int qtd, const char *nomeArquivo){
    FILE *file = fopen(nomeArquivo, "wb");
    if(file == NULL){
        printf("Erro ao abrir o arquivo para gravacao.\n");
        return;
    }

    fwrite(&qtd, sizeof(int), 1, file);

    if(qtd > 0){
        fwrite(vetor, sizeof(Produto), qtd, file);
    }
    fclose(file);
}

void carregarDadosBinario(Produto **vetor, int *qtd, int *capacidade, const char *nomeArquivo){
    FILE *file = fopen(nomeArquivo, "rb");
    if(file == NULL){
        inicializarEstoque(vetor, capacidade, qtd);
        return;
    }

    fseek(file, 0, SEEK_END);
    long tamanhoArquivo = ftell(file);
    rewind(file);

    if(tamanhoArquivo <= sizeof(int)){
        *qtd = 0;
        *capacidade = 10;
        *vetor = (Produto *) malloc ((*capacidade) * sizeof(Produto));
        fclose(file);
        return;
    }

    fread(qtd, sizeof(int), 1, file);

    *capacidade = (*qtd > 0) ? *qtd * 2 : 10;
    *vetor = (Produto *) malloc ((*capacidade) * sizeof(Produto));

    if(*vetor != NULL && *qtd > 0){
        fread(*vetor, sizeof(Produto), *qtd, file);
    }
    fclose(file);
}

void liberarMemoria(Produto **vetor){
    free(*vetor);
    *vetor = NULL;
}