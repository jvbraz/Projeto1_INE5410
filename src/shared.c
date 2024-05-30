#include "shared.h"

#include <queue.h>
#include "defs.h"
#include <time.h>

// Você pode declarar novas funções (ou variaveis compartilhadas) aqui


/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
Queue *gate_queue = NULL;
int toys_map_capacity[MAX_CAPACITY_TOY];
int clientes_no_parque = 0;
int clientes_na_fila;
int total_brinquedos;
pthread_mutex_t mutex_fila;
sem_t *sem_brinquedos;
/*
int buy_coins(client_t *self) {
    int random_coins = rand() % MAX_COINS;
    self->coins = random_coins;
    return random_coins;
}
*/

// Função onde o cliente espera a liberação da bilheteria para adentrar ao parque.
//void wait_ticket(client_t *self) {
    /* Se o turista é o primeiro da fila, ele vai comprar os tickets, se não,
       ele continua esperando na função */
    /*
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
*/
