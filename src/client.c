#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "defs.h"
#include "client.h"
#include "queue.h"
#include "shared.h"

// Declarações de funções auxiliares
void queue_enter(client_t *self);
void buy_coins(client_t *self);
void wait_ticket(client_t *self);

// Thread que implementa o fluxo do cliente no parque.
void *enjoy(void *arg) {
    client_t *cliente = (client_t *)arg;

    debug("Cliente [%d] entrou\n", cliente->id);

    // Entra na fila
    queue_enter(cliente);
    
    // Espera a sua vez de comprar os tickets
    //wait_ticket(cliente);
    buy_coins(cliente);
    
    //Entra no parque e gasta as moedas
    //IMPLEMENTAR AQUI
    while(cliente->coins > 0)
    {
        cliente->coins--;
        //int idx_toy = rand() % total_brinquedos;
        //pthread_mutex_lock(&mutex_cliente_no_brinquedo);
        //sem_wait(&brinquedos[idx_toy]);
        //pthread_mutex_unlock(&mutex_cliente_no_brinquedo);
    }

    // Finaliza a thread
    pthread_mutex_lock(&mutex_finaliza_clientes);
    total_clientes--;
    if (total_clientes == 0) {
        pthread_cond_signal(&cond_finaliza_clientes);
    }
    pthread_mutex_unlock(&mutex_finaliza_clientes);
    pthread_exit(NULL);
}

// Função onde o cliente compra as moedas para usar os brinquedos

void buy_coins(client_t *self) {
    self->coins = rand() % MAX_COINS;
}


// Função onde o cliente espera a liberação da bilheteria para adentrar ao parque.
void wait_ticket(client_t *self) {
    /* Se o turista é o primeiro da fila, ele vai comprar os tickets, se não,
       ele continua esperando na função */
    
    while (1) {
        pthread_mutex_lock(&mutex_fila);
        if (gate_queue->front->data == self->id) {
            break; // Sai do loop se o cliente é o primeiro da fila
        }
        pthread_mutex_unlock(&mutex_fila);
    }
    clientes_na_fila--;
    dequeue(gate_queue);
    pthread_mutex_unlock(&mutex_fila);
}


// Função onde o cliente entra na fila da bilheteria
void queue_enter(client_t *self) {
    pthread_mutex_lock(&mutex_fila);
    enqueue(gate_queue, self->id);
    clientes_na_fila++;
    debug("[QUEUE] - Turista [%d] entrou na fila da bilheteria\n", self->id);
    pthread_mutex_unlock(&mutex_fila);
}

// Essa função recebe como argumento informações sobre o cliente e deve iniciar os clientes.
void open_gate(client_args *args) {
    pthread_t cliente[args->n]; // criando o array de threads com os clientes
    // Iniciando o mutex e cond para a finalização dos clientes
    pthread_mutex_init(&mutex_finaliza_clientes, NULL);
    pthread_cond_init(&cond_finaliza_clientes, NULL);

    // Guardando o total de clientes
    total_clientes = args->n;

    for (int i = 0; i < args->n; i++) { // percorrendo o número de clientes
        if (pthread_create(&cliente[i], NULL, enjoy, (void *)(args->clients[i])) != 0) { // criando a thread passando os argumentos do enjoy
            perror("Não foi possível criar a thread cliente");
        }
    }

}

// Essa função deve finalizar os clientes
void close_gate() {
    // Esperando as condições para finalizar os clientes
    pthread_mutex_lock(&mutex_finaliza_clientes);
    while (total_clientes > 0) {
        pthread_cond_wait(&cond_finaliza_clientes, &mutex_finaliza_clientes);
    }
    pthread_mutex_unlock(&mutex_finaliza_clientes);

    debug("[INFO] - Todos os clientes saíram do parque!\n");

    // Destruindo os mutexes
    pthread_mutex_destroy(&mutex_finaliza_clientes);

    // Destruindo as condições
    pthread_cond_destroy(&cond_finaliza_clientes);

    // Destruindo os semáforo   
}
