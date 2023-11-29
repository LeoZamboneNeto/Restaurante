#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#define MAX_PEDIDOS 100

int opcao = -1;
int senha = 1;
int senhaPreferencial = 1;
int tipoSenha = 0;
int contadorPedidos = 0;

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

struct Produto {
    char nome[50];
    float valor;
};

struct NoProduto {
    struct Produto produto;
    struct NoProduto* prox;
};

struct Pedido {
    int numero;
    struct NoProduto* produtos;    // Lista encadeada de produtos
};

struct Prato {
    char nome[50];
    float preco;
    int quantidade;
};

struct Prato cardapio[15];

struct Prato pratosInicio[15] = {
    {"Lasanha", 25.00, 10},
    {"Salmão", 25.00, 8},
    {"Feijuca Campeã", 37.00, 5},
    {"Frango Grelhado", 20.00, 15},
    {"Macarrão", 22.00, 10},
    {"Refrigerante", 7.00, 30},
    {"Suco", 7.00, 20},
    {"Chá", 4.00, 15},
    {"Café", 5.00, 25},
    {"Cerveja", 11.00, 30},
    {"Sorvete", 10.00, 20},
    {"Mousse de Chocolate", 10.00, 8},
    {"Pudim", 20.00, 10},
    {"Torta de Limão", 15.00, 10},
    {"Bolo de Cenoura", 8.00, 10},
};

void inicializarCardapio(struct Prato *pratosIniciais) {
    for (int i = 0; i < 15; i++) {
        strcpy(cardapio[i].nome, (pratosIniciais + i)->nome);
        cardapio[i].preco = (pratosIniciais + i)->preco;
        cardapio[i].quantidade = (pratosIniciais + i)->quantidade;
    }
}

void mostrarCardapio(struct Prato *pratos) {
    printf("\n## Cardápio ##\n");

    for (int i = 0; i < 15; i++) {
        printf("%d - Nome: %s \n", i + 1, (pratos + i)->nome);
        printf("Preço: R$%.2f\n", (pratos + i)->preco);
        printf("\n");
       // printf("Quantidade disponível: %d\n\n", (pratos + i)->quantidade);
    }
}



void adicionarPedido(struct Fila *f, struct Pedido historico[], int *contadorPedidos) {
    if (*contadorPedidos < f->nItens) {
        struct Pedido novoPedido;

        // Limpar o buffer de entrada
        while (getchar() != '\n');

        novoPedido.numero = *contadorPedidos + 1; // Incrementa o número sequencial do pedido

        // Preencha os detalhes do novo pedido, como adicionar produtos ao pedido
        selecionarPratos(&novoPedido); // Função para selecionar os produtos para este pedido

        historico[*contadorPedidos] = novoPedido;

        printf("Pedido adicionado com sucesso!\n");
    } else {
        printf("O histórico de pedidos está cheio. Não é possível adicionar mais pedidos.\n");
    }
}

void selecionarPratos(struct Prato *pratos, struct Pedido historico[], int *contadorPedidos) {
    int escolha;
    float subtotal = 0.0;

    printf("Escolha o(s) prato(s) desejado(s) (digite 0 para voltar): ");
    scanf("%d", &escolha);

    // Verificação se a escolha é 0 (voltar)
    if (escolha == 0) {
        return;
    }

    // Lógica para associar os pratos ao pedido correto
    int numPedido = *contadorPedidos; // Considerando que o contador representa o número do pedido atual
    if (numPedido >= MAX_PEDIDOS) {
        printf("Número máximo de pedidos atingido.\n");
        return;
    }

    // Preenchimento dos detalhes do novo pedido
    historico[numPedido].numero = numPedido + 1; // Incrementa o número sequencial do pedido
    historico[numPedido].produtos = NULL; // Inicializa a lista de produtos do pedido

    while (escolha != 0) {
        if (escolha < 1 || escolha > 15) {
            printf("Opção inválida!\n");
        } else {
            if ((pratos + escolha - 1)->quantidade > 0) {
                printf("Prato escolhido: %s - R$%.2f\n", (pratos + escolha - 1)->nome, (pratos + escolha - 1)->preco);
                subtotal += (pratos + escolha - 1)->preco;

                // Adiciona o prato ao histórico de pedidos
                adicionarPratoAoPedido(&historico[numPedido], (pratos + escolha - 1)->nome, (pratos + escolha - 1)->preco);

                (pratos + escolha - 1)->quantidade--; // Reduz a quantidade disponível
                printf("Subtotal: R$%.2f\n", subtotal);
            } else {
                printf("Desculpe, o prato %s está esgotado!\n", (pratos + escolha - 1)->nome);
            }
        }

        printf("Escolha o(s) prato(s) desejado(s) (digite 0 para voltar): ");
        scanf("%d", &escolha);
    }

    (*contadorPedidos)++; // Atualiza o contador de pedidos
}


void consultarHistorico(struct Pedido historico[], int contadorPedidos) {
    if (contadorPedidos == 0) {
        printf("O histórico de pedidos está vazio.\n");
    } else {
        printf("Histórico de Pedidos:\n");
        printf("%-15s%-30s%-10s\n", "Número", "Produto", "Valor");
        for (int i = 0; i < contadorPedidos; i++) {
            printf("Pedido %d:\n", historico[i].numero);
            struct NoProduto* atual = historico[i].produtos;
            while (atual != NULL) {
                printf("%-15d%-30s%-10.2f\n", historico[i].numero, atual->produto.nome, atual->produto.valor);
                atual = atual->prox;
            }
            printf("\n");  // Adiciona uma linha em branco após imprimir os produtos do pedido
        }
    }
}

void adicionarPratoAoPedido(struct Pedido* pedido, const char* nome, float valor) {
    struct NoProduto* novoProduto = (struct NoProduto*)malloc(sizeof(struct NoProduto));
    if (novoProduto == NULL) {
        printf("Erro ao alocar memória.\n");
        return;
    }
    strcpy(novoProduto->produto.nome, nome);
    novoProduto->produto.valor = valor;
    novoProduto->prox = NULL;

    if (pedido->produtos == NULL) {
        pedido->produtos = novoProduto;
    } else {
        struct NoProduto* atual = pedido->produtos;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novoProduto;
    }
}



void alterarQuantidadePrato(struct Prato *pratos) {
    int escolha, novaQuantidade;

    printf("Escolha o prato para alterar a quantidade (1-15): ");
    scanf("%d", &escolha);

    if (escolha < 1 || escolha > 15) {
        printf("Opção inválida!\n");
        return;
    }

    printf("Digite a nova quantidade para %s: ", (pratos + escolha - 1)->nome);
    scanf("%d", &novaQuantidade);

    if (novaQuantidade < 0) {
        printf("A quantidade não pode ser negativa!\n");
        return;
    }

    (pratos + escolha - 1)->quantidade = novaQuantidade;
    printf("Quantidade alterada com sucesso!\n");
}

void liberarFila(struct Fila *f) {
    free(f->dados);
}

void salvarHistorico(struct Pedido historico[], int contadorPedidos) {
    FILE *arquivo;
    arquivo = fopen("historico_pedidos.txt", "w");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    fprintf(arquivo, "Histórico de Pedidos:\n");
    fprintf(arquivo, "%-15s%-30s%-10s\n", "Número", "Produto", "Valor");

    for (int i = 0; i < contadorPedidos; i++) {
        fprintf(arquivo, "Pedido %d:\n", historico[i].numero);

        struct NoProduto* atual = historico[i].produtos;
        while (atual != NULL) {
            fprintf(arquivo, "%-15d%-30s%-10.2f\n", historico[i].numero, atual->produto.nome, atual->produto.valor);
            atual = atual->prox;
        }
    }

    fclose(arquivo);
    printf("Histórico de pedidos salvo com sucesso no arquivo 'historico_pedidos.txt'.\n");
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

void daraHora(){
            time_t t;
            struct tm *info;
            time(&t);
            info = localtime(&t);

            char buffer[80];
            strftime(buffer, sizeof(buffer), "%c", info);
            printf("\nData e hora do pedido: %s\n", buffer);
}

void remover(struct Fila *fComum, struct Fila *fPreferencial, int tipoSenha) {

    if (tipoSenha == 1) {
        if (fComum->nItens > 0) {
            int senhaRemovida = fComum->dados[fComum->primeiro];
            fComum->primeiro = (fComum->primeiro + 1) % fComum->capacidade;
            fComum->nItens--;

            float valorPedido = 0.0; // Variável para armazenar o valor total do pedido

            daraHora();

            printf("Senha comum removida com sucesso: C%d\n", senhaRemovida);
            printf("Tipo de Senha: Comum\n");

            for (int i = 0; i < 15; i++) {
                if (pratosInicio[i].quantidade < cardapio[i].quantidade) {
                    valorPedido += pratosInicio[i].preco;
                }
            }

            printf("Valor do pedido: R$%.2f\n\n", valorPedido);
        } else {
            printf("Fila comum vazia, não é possível remover.\n");
        }
    } else if (tipoSenha == 2) {
        if (fPreferencial->nItens > 0) {
            int senhaRemovida = fPreferencial->dados[fPreferencial->primeiro];
            fPreferencial->primeiro = (fPreferencial->primeiro + 1) % fPreferencial->capacidade;
            fPreferencial->nItens--;


            float valorPedido = 0.0; // Variável para armazenar o valor total do pedido

            daraHora();

            printf("Senha preferencial removida com sucesso: P%d\n", senhaRemovida);
            printf("Tipo de Senha: Preferencial\n");

            // Adicionando lógica para calcular o valor do pedido preferencial
            for (int i = 0; i < 15; i++) {
                if (pratosInicio[i].quantidade < cardapio[i].quantidade) {
                    valorPedido += pratosInicio[i].preco;
                }
            }

            printf("Valor do pedido: R$%.2f\n\n", valorPedido);
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
    printf("3 - Senha XXX Concluída\n");
    printf("4 - Cardápio\n");
    printf("5 - Alterar quantidade de prato\n");
    printf("6 - Consultar Histórico\n");
    printf("0 - Sair\n\n");
    printf("Escolha uma opção: ");
}

struct Pedido historico[MAX_PEDIDOS];

void gerenciadorSenhas() {
    struct Pedido historico[MAX_PEDIDOS]; // Declare o array antes do loop ou no escopo que a função pode acessar

    struct Pedido novoPedido;
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
            case 0: {
                printf("Saindo...\n");
                break;
            }

            case 1: {
                 inserir(&minhaFila, senha);
                 printf("Senha %d, retirada com sucesso!\n\n", senha);
                 senha++;
                 mostrarFilas(&minhaFila, &minhaFilaPreferencial);
                 inicializarCardapio(pratosInicio);
                 mostrarCardapio(pratosInicio);
                 selecionarPratos(pratosInicio, historico, &contadorPedidos);
                 adicionarPedido(&minhaFila, historico, &contadorPedidos);  // Passando o endereço do contador global
                 break;
}


            case 2: {
                inserir(&minhaFilaPreferencial, senhaPreferencial);
                printf("Senha P%d retirada com sucesso!\n\n", senhaPreferencial);
                senhaPreferencial++;
                mostrarFilas(&minhaFila, &minhaFilaPreferencial);
                inicializarCardapio(pratosInicio);
                mostrarCardapio(pratosInicio);
                selecionarPratos(pratosInicio, historico, &contadorPedidos);
                adicionarPedido(&minhaFila, historico, &contadorPedidos);
                 break;
            }

            case 3: {
                printf("Escolha o tipo de senha a ser removida:\n");
                printf("1 - Comum\n");
                printf("2 - Preferencial\n");
                scanf("%d", &tipoSenha);

                remover(&minhaFila, &minhaFilaPreferencial, tipoSenha);
                mostrarFilas(&minhaFila, &minhaFilaPreferencial);
                break;
            }

            case 4: {
                inicializarCardapio(pratosInicio);
                mostrarCardapio(pratosInicio);
                adicionarPedido(&minhaFila, historico, &contadorPedidos);

                gerenciadorSenhas();
            }

             case 5: {
                alterarQuantidadePrato(pratosInicio);
                break;
            }

            case 6: {
                consultarHistorico(historico, contadorPedidos);
                break;

            }


            default:
                printf("Opção inválida!\n");
                break;
        }
    }
}

int main() {
    setlocale(LC_ALL, "Portuguese");

    gerenciadorSenhas();

    // Salvar histórico ao sair do programa
  //  selecionarPratos(pratosInicio, 15, &historico[contadorPedidos]);


    return 0;
}

