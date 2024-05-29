/**
 * Esse arquivo tem como objetivo a implementação do cliente em um parque.
 * Você pode criar novas funções dentro desse arquivo caso julgue necessário.
 * ATENÇÃO: NÃO APAGUE OU EDITE O NOME OU O(S) ARGUMENTO(S) DAS FUNÇÕES FORNECIDAS
*/

#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>

#include "defs.h"
#include "client.h"
#include "queue.h"
#include "shared.h"


// Thread que implementa o fluxo do cliente no parque.
void *enjoy(void *arg){
    debug("Cliente entrou\n");
    pthread_exit(NULL);
}

// Funcao onde o cliente compra as moedas para usar os brinquedos
void buy_coins(client_t *self){
    // Sua lógica aqui
}

// Função onde o cliente espera a liberacao da bilheteria para adentrar ao parque.
void wait_ticket(client_t *self){
    // Sua lógica aqui
}

// Funcao onde o cliente entra na fila da bilheteria
void queue_enter(client_t *self){
    // Sua lógica aqui.
    debug("[WAITING] - Turista [%d] entrou na fila do portao principal\n", self->id);

    // Sua lógica aqui.
    buy_coins(self);

    // Sua lógica aqui.
    debug("[CASH] - Turista [%d] comprou [%d] moedas.\n", self->id, self->coins);
}

// Essa função recebe como argumento informações sobre o cliente e deve iniciar os clientes.
void open_gate(client_args *args){
    pthread_t cliente[args->n];  // criando o array de threads com os clientes
    for (int i = 0; i < args->n; i++) {  // percorrendo o número de clientes
        cliente[i] = args->clients[i]->toys[i]->thread;  // criando uma thread para cara cliente
        if (pthread_create(&cliente[i], 0, &enjoy, NULL) != 0) {  // criando a thread passando os argumentos do enjoy
            perror("Não foi possivel criar a thread clientes");
        }
    }
    // fazendo os joins dos clientes
    for (int i = 0; i < args->n; i++) {
        if (pthread_join(cliente[i], NULL) != 0) {  // verifica se foi possivel fazer o join
            perror("Erro foi fazer o join dos clientes");
        }
    }
}

// Essa função deve finalizar os clientes
void close_gate(){
   //Sua lógica aqui
}
