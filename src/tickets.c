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
void atender_cliente(ticket_t *funcionario) {
    while (!is_queue_empty(gate_queue)) {  // enquanto a fila não estiver fazia
        pthread_mutex_lock(&mutex_sai_fila);  // lock para não pegar duas vezes a mesma pessoa
        int cliente = dequeue(gate_queue);  // recuperar o id do cliente
        pthread_mutex_unlock(&mutex_sai_fila);
        sem_wait(&sem_funcionarios);  // falar que um funcionario já está atendendo alguem
        sem_post(&sem_moedas);  // libera um cliente a comprar moedas(não finalizado)
        printf("cliente %d comprou tickets\n", cliente);  // MENSAGEM PARA DEBUGAR
        sem_post(&sem_funcionarios);  // funcionario liberado
    }

    //Finalizando a thread
    pthread_mutex_lock(&mutex_finaliza_funcionarios);
    total_funcionarios--;
    if(total_funcionarios == 0)
    {
        pthread_cond_signal(&cond_finaliza_funcionarios);
    }
    pthread_mutex_unlock(&mutex_finaliza_funcionarios);
}

// Thread que implementa uma bilheteria
void *sell(void *args){
    ticket_t *funcionario = (ticket_t *)args;
    debug("[INFO] - Bilheteria Abriu!\n");
    atender_cliente(funcionario);
    pthread_exit(NULL);
}

// Essa função recebe como argumento informações sobre a bilheteria e deve iniciar os atendentes.
void open_tickets(tickets_args *args){  // lista de funcionario, qtd
    // recuperando o número de funcionarios para tornar o código mais legivel
    int numero_funcionarios = args->n;

    // Iniciando o mutex e cond para a finalização dos funcionarios
    pthread_mutex_init(&mutex_finaliza_funcionarios, NULL);
    pthread_cond_init(&cond_finaliza_funcionarios, NULL);
    
    // Guardando o total de funcionários
    total_funcionarios = numero_funcionarios;

    // iniciando o semaforo para controlar os funcionarios
    sem_init(&sem_funcionarios, 0, numero_funcionarios);
    // iniciando o semaforo para controlar as moedas
    sem_init(&sem_moedas, 0, 0);
    // criando o mutex para a fila
    pthread_mutex_init(&mutex_sai_fila, NULL);

    pthread_t funcionario[numero_funcionarios];  // array de threads
    for (int i = 0; i < args->n; i++) {
        funcionario[i] = args->tickets[i]->thread;  // recebendo a thread da struct
        // verificando se for possivel criar a thread
        if (pthread_create(&funcionario[i], 0, &sell, (void *)(args->tickets[i])) != 0) {
            perror("erro ao criar a thread funcionarios");
        }
    }

    
}


// Essa função deve finalizar a bilheteria
void close_tickets(){
    // Esperando as condições para finalizar os tickets
    pthread_mutex_lock(&mutex_finaliza_funcionarios);
    while (total_funcionarios > 0)
    {
        pthread_cond_wait(&cond_finaliza_funcionarios, &mutex_finaliza_funcionarios);
    }
    pthread_mutex_unlock(&mutex_finaliza_funcionarios);    

    debug("[INFO] - Todas as bilheterias fecharam!\n");
    // Destruindo os mutexes
    pthread_mutex_destroy(&mutex_sai_fila);
    pthread_mutex_destroy(&mutex_finaliza_funcionarios);

    //Destruindo os semaforos
    sem_destroy(&sem_funcionarios);
    sem_destroy(&sem_moedas);

    // Destruindo a condição de finalização
    pthread_cond_destroy(&cond_finaliza_funcionarios);
}