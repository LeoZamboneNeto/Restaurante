#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <locale.h>

int opcao = -1;
int senha = 1;
int senhaPreferencial = 1;
int tipoSenha = 0;


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

struct Prato{
    char nome[50];
    float preco;
};

struct Prato cardapio[5];

struct Prato pratosInicio[5] = {
        {"Lasanha", 25.00},
        {"Salmão", 25.00},
        {"Feijuca Campeã", 37.00},
        {"Refrigerante", 7.00},
        {"Suco", 7.00},
};

void inicializarCardapio(struct Prato *pratosIniciais) {
    for (int i = 0; i < 5; i++) {
        strcpy(cardapio[i].nome, (pratosIniciais + i)->nome);
        cardapio[i].preco = (pratosIniciais + i)->preco;
    }
}

void mostrarCardapio(struct Prato *pratos){
    printf("\n## Cardápio ##\n");

    for (int i = 0; i < 5; i++){
            printf("%d - Nome: %s \n", i + 1, (pratos + i)->nome);
            printf("Preço: R$%.2f\n\n",(pratos + i)->preco);
    }
}

void selecionarPratos(struct Prato *pratos) {
    int escolha;
    float subtotal = 0.0;

    while (1) {
        printf("Escolha o(s) prato(s) desejado(s) (digite 0 para voltar): ");
        scanf("%d", &escolha);

        if (escolha == 0) {
            break; // Sai do loop se a escolha for 0
        }

        if (escolha < 1 || escolha > 5) {
            printf("Opção inválida!\n");
        } else {
            printf("Prato escolhido: %s - R$%.2f\n", (pratos + escolha - 1)->nome, (pratos + escolha - 1)->preco);
            subtotal += (pratos + escolha - 1)->preco;
            printf("Subtotal: R$%.2f\n", subtotal);
        }
    }


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

void mostrarFila(struct Fila *f) {
    int cont, i;

    for (cont = 0, i = f->primeiro; cont < f->nItens; cont++) {
        if (f->dados[i] != 0 && f->dados[i] % 10 == 0){   }
    }
    printf("\n");
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

void remover(struct Fila *fComum, struct Fila *fPreferencial, int tipoSenha) {
    if (tipoSenha == 1) {
        if (fComum->nItens > 0) {
            int senhaRemovida = fComum->dados[fComum->primeiro];
            fComum->primeiro = (fComum->primeiro + 1) % fComum->capacidade;
            fComum->nItens--;

            printf("Senha comum removida com sucesso: C%d\n", senhaRemovida);
        } else {
            printf("Fila comum vazia, não é possível remover.\n");
        }
    } else if (tipoSenha == 2) {
        if (fPreferencial->nItens > 0) {
            int senhaRemovida = fPreferencial->dados[fPreferencial->primeiro];
            fPreferencial->primeiro = (fPreferencial->primeiro + 1) % fPreferencial->capacidade;
            fPreferencial->nItens--;

            printf("Senha preferencial removida com sucesso: P%d\n", senhaRemovida);
        } else {
            printf("Fila preferencial vazia, não é possível remover.\n");
        }
    } else {
        printf("Tipo de senha inválido!\n");
    }
}

void menu() {
    printf("\n### MENU ###\n");
    printf("1 - Retirar senha\n");
    printf("2 - Retirar senha Preferencial\n");
    printf("3 - Senha XXX Concluida\n");
    printf("4 - Cardápio\n");

    printf("0 - Sair\n\n");
    printf("Escolha uma opcao: ");
}

void gerenciadorSenhas(){

    setlocale(LC_ALL, "Portuguese");

    //definir struct, eu poderia ter feito isso globalmente
    struct Fila minhaFila;
    struct Fila minhaFilaPreferencial;

    criarFila(&minhaFila, 5);
    criarFila(&minhaFilaPreferencial, 5);

    int opcao = -1;
    int senha = 1;
    int senhaPreferencial = 1;
    int tipoSenha = 0;



    while (opcao != 0) {

        menu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 0:{
                printf("Saindo...\n");
                break;}

            case 1:{
                    inserir(&minhaFila, senha);
                    printf("Senha %d, retirada com sucesso!\n\n", senha);
                    senha++;
                    mostrarFilas(&minhaFila, &minhaFilaPreferencial);
                    inicializarCardapio(pratosInicio);
                    mostrarCardapio(pratosInicio);
                    selecionarPratos(pratosInicio);
                    break;}

            case 2:{
                    inserir(&minhaFilaPreferencial, senhaPreferencial);
                    printf("Senha P%d retirada com sucesso!\n\n", senhaPreferencial);
                    senhaPreferencial++;
                    mostrarFilas(&minhaFila, &minhaFilaPreferencial);
                    inicializarCardapio(pratosInicio);
                    mostrarCardapio(pratosInicio);
                    selecionarPratos(pratosInicio);;
                    break;}

            case 3:{
                 printf("Escolha o tipo de senha a ser removida:\n");
                 printf("1 - Comum\n");
                 printf("2 - Preferencial\n");
                 scanf("%d", &tipoSenha);

                 remover(&minhaFila, &minhaFilaPreferencial, tipoSenha);
                 mostrarFilas(&minhaFila, &minhaFilaPreferencial);
                 break;}

             case 4:{
                 inicializarCardapio(pratosInicio);
                 mostrarCardapio(pratosInicio);
                 selecionarPratos(pratosInicio);
                 gerenciadorSenhas();}


            default:
                printf("Opção inválida!\n");
                break;}

}

}

int main() {

 gerenciadorSenhas();

return 0;}
