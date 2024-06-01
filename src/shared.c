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
int clientes_na_fila = 0;
int total_clientes = 0;
int total_funcionarios = 0;
int total_brinquedos = 0;
// Mutexes para a fila
pthread_mutex_t mutex_fila;
pthread_mutex_t mutex_sai_fila;

// Mutexes para finalizar as threads
pthread_mutex_t mutex_finaliza_brinquedos;
pthread_mutex_t mutex_finaliza_funcionarios;
pthread_mutex_t mutex_finaliza_clientes;

// Condições para finalizar as threads
pthread_cond_t cond_finaliza_brinquedos;
pthread_cond_t cond_finaliza_clientes;
pthread_cond_t cond_finaliza_funcionarios;

sem_t *sem_brinquedos;
sem_t sem_funcionarios;
sem_t sem_moedas;

