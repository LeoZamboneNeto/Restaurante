#include <stdio.h>
#include <stdlib.h>

#include<locale.h>

struct Fila {
    int capacidade;
    int *dados;
    int primeiro;
    int ultimo;
    int nItens;
};

void criarFila(struct Fila *f, int c) {
    f->capacidade = c;
    f->dados = (int *)malloc(f->capacidade * sizeof(int));
    f->primeiro = 0;
    f->ultimo = -1;
    f->nItens = 0;
}

void liberarFila(struct Fila *f) {
    free(f->dados);
}

void adicionarCapacidade(struct Fila *f, int item) {
    if (f->nItens == f->capacidade) {
        f->capacidade += 10;
        int *nova_capacidade = realloc(f->dados, f->capacidade * sizeof(int));

        if (nova_capacidade == NULL) {
            printf("Memória insuficiente!\n");
            return;
        }

        f->dados = nova_capacidade;
    }

    f->ultimo = (f->ultimo + 1) % f->capacidade;
    f->dados[f->ultimo] = item;
    f->nItens++;
}

void inserir(struct Fila *f, int v) {
    adicionarCapacidade(f, v);
}

void remover(struct Fila *f) {
    if (f->nItens == 0) {
        printf("Fila vazia, não é possível remover.\n");
        return;
    }

    int temp = f->dados[f->primeiro];
    f->primeiro = (f->primeiro + 1) % f->capacidade;
    f->nItens--;
    // return temp;
}

void menu() {
    printf("\n### MENU ###\n");
    printf("1 - Retirar senha\n");
    printf("2 - Retirar senha Preferencial\n");
    printf("0 - Sair\n\n");
    printf("Escolha uma opcao: ");
}

void mostrarFila(struct Fila *f){
	int cont, i;
	for ( cont=0, i= f->primeiro; cont < f->nItens; cont++){
		printf("%i\t",f->dados[i++]);
		if (i == f->capacidade)
			i=0;
}
	printf("\n\n");
}

int main() {
    setlocale(LC_ALL, "Portuguese");

    struct Fila minhaFila;
    criarFila(&minhaFila, 5);

    int opcao = -1;
    int senha = 1;
    int senhaPreferencial = 1;


    //permanecer no loop
    while (opcao != 0) {
        menu();
        scanf("%d", &opcao);


        switch (opcao) {
            case 0:{
                printf("Saindo...\n");
                break;}

            case 1:{
                inserir(&minhaFila, senha);
                printf("Senha %d, retirada com sucesso!\n", senha);
                senha++;
                break;}

            case 2:{
                inserir(&minhaFila, senhaPreferencial);
                printf("Senha P%d retirada com sucesso!\n", senhaPreferencial);
                senhaPreferencial++;
                break;}

            default:
                printf("Opção inválida!\n");
                break;}


            mostrarFila(&minhaFila);





   // liberarFila(&minhaFila);

}
return 0;}
