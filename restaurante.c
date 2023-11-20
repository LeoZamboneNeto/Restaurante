#include <stdio.h>
#include <stdlib.h>

#include<locale.h>

int opcao = -1;
int senha = 1;
int senhaPreferencial = 1;

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

void mostrarFila(struct Fila *f) {
    int cont, i;

    for (cont = 0, i = f->primeiro; cont < f->nItens; cont++) {
        if (f->dados[i] != 0 && f->dados[i] % 10 == 0){   }
    }
    printf("\n\n");
}

void mostrarFilas(struct Fila *fComum, struct Fila *fPreferencial) {
    printf("Fila Comum: ");
    for (int i = fComum->primeiro, cont = 0; cont < fComum->nItens; i = (i + 1) % fComum->capacidade, cont++) {
        printf("C%d\t", fComum->dados[i]);
    }
    printf("\n");

    printf("Fila Preferencial: ");
    for (int i = fPreferencial->primeiro, cont = 0; cont < fPreferencial->nItens; i = (i + 1) % fPreferencial->capacidade, cont++) {
        printf("P%d\t", fPreferencial->dados[i]);
    }
    printf("\n\n");
}

void menu() {
    printf("\n### MENU ###\n");
    printf("1 - Retirar senha\n");
    printf("2 - Retirar senha Preferencial\n");
    printf("3 - Senha XXX Concluida\n");
    printf("0 - Sair\n\n");
    printf("Escolha uma opcao: ");
}

int main() {
    setlocale(LC_ALL, "Portuguese");

    struct Fila minhaFila;
    struct Fila minhaFilaPreferencial;

    criarFila(&minhaFila, 5);
    criarFila(&minhaFilaPreferencial, 5);

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
                 mostrarFilas(&minhaFila, &minhaFilaPreferencial);
                break;}
                  
            case 2:{
                inserir(&minhaFilaPreferencial, senhaPreferencial);
                printf("Senha P%d retirada com sucesso!\n", senhaPreferencial);
                senhaPreferencial++;
                mostrarFilas(&minhaFila, &minhaFilaPreferencial);
                break;}
                
            case 3:{ }         
                

            default:
                printf("Opção inválida!\n");
                break;}


            mostrarFila(&minhaFila);






   // liberarFila(&minhaFila);

}
return 0;}
