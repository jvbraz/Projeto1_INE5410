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
pthread_mutex_t mutex_sai_fila;
sem_t *sem_brinquedos;
sem_t sem_funcionarios;
sem_t sem_moedas;

