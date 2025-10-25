#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define MAX_FILA 5  // tamanho maximo da fila
#define MAX_PILHA 3  // tamanho maximo da pilha
// VARIAVEIS GLOBAIS-----------------------------------------------------------------
int id = 0; // contados que sera usado para definir o id de cada peça de maneira incremental
char tipo[4] = {'I', 'O', 'T', 'L'}; // vetor com todos os tipos de peça disponíveis
// STRUCTS---------------------------------------------------------------------------
typedef struct {
    char tipo; // recebe o tipo da peça: 'I', 'O', 'T', 'L'
    int id;  // recebe um id unico automaticamente a cada nova peça gerada
} Peca;
typedef struct {  // estrutura FIFO
    Peca pecas[MAX_FILA];
    int inicio;
    int fim;
    int total;
} Fila;
typedef struct { // estrutura LIFO
    Peca pecas_r[MAX_PILHA];
    int topo;
} Pilha;
// -- FUNÇÕES UTILITÁRIAS------------------------------------------------------------
void limparBufferEntrada() { // Limpa o buffer de entrada para evitar conportamentos indesejados apos o usuario digitar uma opção
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
void pausa() { // executa uma pausa para o usuario ler todos as informações presentes na tela
    printf("\nTecle enter para continuar...\n");
    getchar();
}
// FUNÇÕES PARA GERENCIAMENTO DE PILHA (LIFO)----------------------------------------
void inicializarPilha (Pilha *p) { // determina o ponto inicial do topo da pilha (sento -1 = pilha vazia)
    p->topo = -1;
}
int pilhaCheia (Pilha *p) { // verifica se a pilha está cheia
    return p->topo == MAX_PILHA - 1;
}
int pilhaVazia (Pilha *p) { //verifica se a vazia 
    return p->topo == -1;
}
void reservarPeca (Fila *f, Pilha *p) { // transfere primeira peça da fila para o topo da pilha
    if (f->total == 0) { // verifica se existe alguma peça na fila
        return;
    } else if (pilhaCheia(p)){  // verifica se existe espaço na pilha para receber a nova peça
        printf("Todos os slots estão ocupados, não é possível reservar novas peças!\n");
        return;
    }
    // copia o item do topo do inicio da fila para o topo da pilha e atualiza todos os ponteiros (topo, inicio e total)
    p->topo++;
    p->pecas_r[p->topo] = f->pecas[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->total--;
    printf("Peça reservada com sucesso!\n");
}
void jogarPeca_r(Pilha *p, Peca *j, Fila *g) { //joga a peça do topo da pilha
    if (pilhaVazia(p)) { // verifica se existe peça disponivel para jogar
        printf("Reserva vazia, não há peças para jogar!\n");
        return;
    }
    *j = p->pecas_r[p->topo]; // joga a peça, reserva a peça jogada na variavel 'jogada' e atualiza os ponteiros
    p->topo--;
    printf("Peça jogada: %c\n", j->tipo);
    // adiciona a peça jogada a fila GAME e atualiza seus ponteiros
    g->pecas[g->fim] = *j;
    g->fim = (g->fim +1) % MAX_FILA;
    g->total++;
}
void mostrarPecas_r (Pilha *p) { //exibe todas as peças da pilha da esquerda para ireita
    if (pilhaVazia(p)) { // verifica se a pilha está vazia
        printf("Peças reservadas: [vazio]\n");
        return;
    }
    printf("Peças reservadas: ");
    for( int i = p->topo ; i >= 0 ; i-- ) { // laço que percorre toda a pilha exibindo cada item
        printf("[%c, %d] ", p->pecas_r[i].tipo, p->pecas_r[i].id );
    }
    printf("\n");
}
// FUNÇÕES PARA GERENCIAMENTO DE FILA (FIFO)-----------------------------------------
void inicializarFila (Fila *f) { // inicializa os ponteiros da fila (considerando a fila vazia)
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}
int filaCheia (Fila *f) { // verifica se a fila está cheia
    return f->total == MAX_FILA;
}
int filaVazia (Fila *f) { // verifica se a fila está vazia
    return f->total == 0;
}
void gerarPeca(Fila *f, Peca *n) { // função que acrescenta uma peça aleatória a fila
    if(filaCheia(f)){ // verifica que existe espaço na fila
        return;
    }
    int random = rand() % 4; // gera o tipo da peça aleatóriamente
    n->tipo = tipo[random]; // nova peça recebe tipo aleatorio
    n->id = id; // nova peça recebe id atual
    id++; // incrementa id para a proxima peça
    // atualiza os ponteidos do final da fila considerando a a ultima peça inserida
    f->pecas[f->fim] = *n;
    f->fim = (f->fim +1) % MAX_FILA;
    f->total++;
}
void preencherFila (Fila *f, Peca *n) { // ocupa todos os espaços da fila garantindo a fila fique cheia
    while (!filaCheia(f)) { // laço se perete enquanto a fila não estiver cheia
        gerarPeca(f,n); // chama a função gerar peça
    }
}
void remover (Fila *f, Peca *j, Fila *g) { // joga a peça do inicio da fila
    if (filaVazia(f)) { //verifica tem peça diponivel para ser jogada
        printf("Não há mais peças para jogar!\n");
        return;
    }
    // joga a peça, atualiza os ponteiros de inicio e fim da fila para a proxima peça e registra a peça jogada
    *j = f->pecas[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->total--;
    printf("Peça jogada: %c\n", j->tipo);
    // adiciona a peça jogada a fila GAME e atualiza seus ponteiros
    g->pecas[g->fim] = *j;
    g->fim = (g->fim +1) % MAX_FILA;
    g->total++;
}
void mostrarFila (Fila *f) { // exibe a fila de peças
    printf("Fila: ");
    for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX_FILA) { // laço que percorre toda a fila exibindo cada item
        printf("[%c, %d] ",f->pecas[idx].tipo, f->pecas[idx].id);
    }
    printf("\n");
}
void mostrarFila_g (Fila *g) { // exibe as ultimas 5 peças jogadas
    if(g->total > 5 ) { g->total = MAX_FILA; } // limita o contador de peças jogadas ao tamanho maximo da fila
    printf("GAME: ");
    int idx = (g->fim - 1 + MAX_FILA) % MAX_FILA;
    for (int i = 0; i < g->total; i++) {  // laço percorre por todos os item da fila e exibe em ordem inversa
        printf("   %c   ", g->pecas[idx].tipo);
        idx = (idx - 1 + MAX_FILA) % MAX_FILA;
    }
}
// FUNÇÃO PARA TROCA DE PEÇAS -------------------------------------------------------
void trocarPeca (Fila *f, Pilha *p) { //realiza a troca da peça do topo da pilha com a peça do inicio do fila
    if (p->topo < 0) { // verifica se existe peça disponivel para troca
    printf("Nada para trocar!\n");
    return;
    }
    Peca a; // variavel auxiliar
    int t = p->topo;
    int i = f->inicio;
    // realização da troca com auxilio da variavel 'a'
    a= f->pecas[i];
    f->pecas[i] = p->pecas_r[t];
    p->pecas_r[t] = a;
}
void trocarPeca_todas (Fila *f, Pilha *p) { // troca todas as peças da pilha pelas primeiras peças da fila
    if (p->topo < 0) { // verifica se existem peças para serem trocadas
        printf("Nada para trocar!\n");
        return;
    }
    int limite = p->topo; // determina o limite do laço baseado no topo da pilha
    Peca a; // variavel auxiliar
    int i = f->inicio;
    int t = p->topo;
    for (int j = 0; j <= limite; j++) { // laço percorre por toda a pilha realizando a troca com o seu respectivo par na fila com o auxilio da variavel 'a'
        a = f->pecas[i];
        f->pecas[i] = p->pecas_r[t];
        p->pecas_r[t] = a;

        // atualiza os ponteiros para a proxima repetição do laço
        t--;
        i = (i + 1) % MAX_FILA;
    }
    printf("Todas as peças da reserva foram trocadas com as primeiras da fila!\n");
}
// FUNÇÃO PARA VERIFICAÇÃO DE VITORIA------------------------------------------------
//para vencer o usuario precisa completar uma sequencia de 5 peças iguais jopgadas consecutivamente.
void checarVitoria (Fila *g, int *opcao) { // verifica se o usuario completou o objetivo do jogo.
    if (g->total < 5) return;

    int idx = (g->fim - 5 + MAX_FILA) % MAX_FILA; // variavel que recebe o valor do indice da primeira peça da fila de peças jogadas (game)
    char t0 = g->pecas[idx].tipo; // variavel recebe o valor do primeiro item da fila game
    int todosIguais = 1; // variavel recebe guarda a informação se todas as peças são iguais (1-verdadeiro / 0-falso)
    for (int i = 1; i < 5; i++) { // laço compara o primeiro item da fila 't0' com todos os demais
        idx = (idx + 1) % MAX_FILA;
        if (g->pecas[idx].tipo != t0) {
            todosIguais = 0; // caso algum item seja diferente variavel recebe falso
            break;
        }
    }
    if (todosIguais) { // se todos iguais for verdadiro exibe confirmação de vitoria
        printf ("\n     > > > > > > > > > > > > > > > > > >\n");
        mostrarFila_g(g);
        printf ("\n     > > > > > > > > > > > > > > > > > >\n");
        printf("\n* * * * * Parabéns — VOCÊ VENCEU O JOGO! * * * * *\n");
        *opcao = 0; // altera opcao para 0 encerrando o laço de execução pricipal
    }
}
// FUNÇÃO PRINCIPAL------------------------------------------------------------------
int main () {
    Fila f, game;  // variaveis fila FIFO
    Pilha p; // variavel PILHA LIFO
    Peca nova, jogada, reserva; // variaveis de auxilio para alimentação das filas e pilha
    int opcao; // variavel que registra a opcao escolhida pelo jogador
    srand(time(NULL));  // inicializa o randomizer
    inicializarFila(&f);  // inicializa fila f
    inicializarFila(&game); // inicializa fila g
    inicializarPilha(&p); // inicializa pilha p
    preencherFila(&f, &nova); // preenche a fila de peças para iniciar o game
    do { 
        // laço principal do game       
        printf ("\n==============TETRIS STACK==============\n\n");
        mostrarFila(&f); // proximas 5 peças disponivei para se jogada (geradas aleatóriamente)
        printf ("      < < < < < < < < < < < < < < < < <\n\n");
        mostrarPecas_r(&p); // peças reservadas pelo usuário (limite de 3 peças)
        printf ("                  < < < < < < < < < < <\n");
        printf ("========================================\n");
        // menu de opçoes
        printf ("(1) - Jogar peça\n");
        printf ("(2) - Reservar peça\n");
        printf ("(3) - Usar peça reservada\n");
        printf ("(4) - Inverte fila com reserva (1 peça)\n");
        printf ("(5) - Inverte fila com reserva (todas as peças)\n");
        printf ("(0) - Sair\n");
        printf ("\n     > > > > > > > > > > > > > > > > > >\n");
        mostrarFila_g(&game); // exige as ultimas 5 peças jogadas
        printf ("\n     > > > > > > > > > > > > > > > > > >\n");
        printf ("Objetivo: Formar uma sequencia de 5 peças iguais\n");        
        printf ("\nEscolha uma opcao: "); // solicita entrada do usuario
        scanf ("%d", &opcao); // recebe valor de entrada
        limparBufferEntrada();
        switch (opcao) { // switch de opções
        case 1: // trecho executado caso a opção escolida seja 1
            remover (&f, &jogada, &game);
            gerarPeca(&f, &nova);
            checarVitoria(&game, &opcao);
            pausa();
            break;       
        case 2: // trecho executado caso a opção escolida seja 2
            reservarPeca(&f, &p);
            gerarPeca(&f, &nova);
            pausa();
            break;        
        case 3: // trecho executado caso a opção escolida seja 3
            jogarPeca_r(&p, &jogada, &game);
            checarVitoria(&game, &opcao);
            pausa();
            break;
        case 4: // trecho executado caso a opção escolida seja 4
            trocarPeca(&f, &p);
            pausa();
            break;              
        case 5: // trecho executado caso a opção escolida seja 5
            trocarPeca_todas(&f, &p);
            pausa();
            break;                     
        case 0: // trecho executado caso a opção escolida seja 0 (encerra o jogo)
        printf("***** GAME OVER *****");
        break;       
        default: // caso usuário escolha uma opçao inválida retorna ao inicio o laço
            printf("\n *** Opção inválida, tente novamente! *** \n");
            pausa();          
        }
    } while (opcao != 0 ); // laço pricipal só será encerrado quando opcao for igual a 0
    return 0;
}