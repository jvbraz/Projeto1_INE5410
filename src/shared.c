#include "shared.h"

#include <queue.h>
#include "defs.h"

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
pthread_mutex_t mutex_fila;
sem_t sem_brinquedos[];