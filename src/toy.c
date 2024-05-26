/**
 * Esse arquivo tem como objetivo a implementação de um brinquedo em um parque.
 * Você pode criar novas funções dentro desse arquivo caso julgue necessário.
 * ATENÇÃO: NÃO APAGUE OU EDITE O NOME OU O(S) ARGUMENTO(S) DAS FUNÇÕES FORNECIDAS
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "toy.h"
#include "shared.h"

int clients_in_list() 
{
    int random = rand() % 5;
    return random;
}

// Thread que o brinquedo vai usar durante toda a simulação do sistema
void *turn_on(void *args) {
    toy_t *toy = (toy_t *) args;
    int id = toy->id;
    /* Inicializando brinquedo */
    
    /* Enquanto tiver pessoas querendo entrar no brinquedo */
    while (1) {
        /* Adiciona cliente no brinquedo */
        sem_wait(&toy->sem_vagas);
        
        int sem_value;
        sem_getvalue(&toy->sem_vagas, &sem_value);
        
        int clients = clients_in_list();    
        /* Se é sinalizado que não tem mais vagas ou não tem clientes 
        querendo entrar no brinquedo, executa o brinquedo */
        debug("Clientes na fila: %d do brinquedo %d\n", clients, id);
        if (sem_value == 0 || clients == 0) {
            debug("[ON] - O brinquedo  [%d] foi ligado.\n", id);
            /* Espera um tempo para execução do brinquedo */
            sleep(1);
            /* Adicionando ao semáforo [capacity] vagas */
            for (int i = 0; i < toy->capacity; i++) {
                sem_post(&toy->sem_vagas);
            }
            debug("[OFF] - O brinquedo [%d] foi desligado.\n", id);
        }
    }

    /* Desligando brinquedo */
    pthread_exit(NULL);
}

// Essa função recebe como argumento informações e deve iniciar os brinquedos.
void open_toys(toy_args *args) {
    int total_brinquedos = args->n;  // contém o total de brinquedos
    pthread_t brinquedo[total_brinquedos];
    
    for (int i = 0; i < total_brinquedos; i++) {
        /* Inicializar semáforo de vagas com a capacidade do brinquedo */
        sem_init(&args->toys[i]->sem_vagas, 0, args->toys[i]->capacity);
        // criar thread para cada brinquedo
        if (pthread_create(&brinquedo[i], 0, &turn_on, args->toys[i]) != 0) {
            perror("Não foi possível criar a thread brinquedo");
        }
    }
    
    // Iniciando os joins
    for (int i = 0; i < total_brinquedos; i++) {
        // verifica se o join foi bem sucedido 
        if (pthread_join(brinquedo[i], NULL) != 0) {
            perror("Não foi possível dar join na thread brinquedo");
        }
    }
}

// Desligando os brinquedos
void close_toys(){
    // Sua lógica aqui
}