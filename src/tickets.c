/**
 * Esse arquivo tem como objetivo a implementação da bilheteria em um parque.
 * Você pode criar novas funções dentro desse arquivo caso julgue necessário.
 * ATENÇÃO: NÃO APAGUE OU EDITE O NOME OU O(S) ARGUMENTO(S) DAS FUNÇÕES FORNECIDAS.
*/
#include <stdio.h>
#include <stdlib.h>
#include <tickets.h>
#include <unistd.h>
#include <pthread.h>

#include <queue.h>
#include "shared.h"

//funções declaradas
void atender_cliente(int id) {
    
    while (is_queue_empty(clientes_na_fila)) {
        
    }
}

// Thread que implementa uma bilheteria
void *sell(void *args){

    debug("[INFO] - Bilheteria Abriu!\n");

    pthread_exit(NULL);
}

// Essa função recebe como argumento informações sobre a bilheteria e deve iniciar os atendentes.
void open_tickets(tickets_args *args){  // lista de funcionario, qtd
    // recuperando o número de funcionarios para tornar o código mais legivel
    int numero_funcionarios = args->n;
    pthread_t funcionario[numero_funcionarios];  // array de threads
    for (int i = 0; i < numero_funcionarios; i++) {
        funcionario[i] = args->tickets[i]->thread;  // recebendo a thread da struct
        // verificando se for possivel criar a thread
        if (pthread_create(&funcionario[i], 0, &sell, NULL) != 0) {
            perror("erro ao criar a thread funcionarios");
        }
    }
    // fazendo o join de cada thread
    for (int i = 0; i < numero_funcionarios; i++) {
        if (pthread_join(funcionario[i], NULL)) {  // verificando se for possivel fazer o join
            perror("Não foi possivel fazer o join da thread funcionarios");
        }
    }
}


// Essa função deve finalizar a bilheteria
void close_tickets(){
    
}