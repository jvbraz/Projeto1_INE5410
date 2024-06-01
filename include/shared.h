#ifndef __SHARED_H__
#define __SHARED_H__

#include <defs.h>
// Você pode declarar novas funções (ou variaveis compartilhadas) aqui
extern int toys_map_capacity[MAX_CAPACITY_TOY];
extern int clientes_na_fila;
extern int clientes_no_parque;
extern int total_clientes;
extern int total_brinquedos;
extern int total_funcionarios;
extern pthread_mutex_t mutex_fila;
extern pthread_mutex_t mutex_sai_fila;
extern pthread_mutex_t mutex_finaliza_brinquedos;
extern pthread_mutex_t mutex_finaliza_funcionarios;
extern pthread_mutex_t mutex_finaliza_clientes;
extern pthread_cond_t cond_finaliza_brinquedos;
extern pthread_cond_t cond_finaliza_clientes;
extern pthread_cond_t cond_finaliza_funcionarios;
extern sem_t sem_queue;
extern sem_t *sem_brinquedos;
extern sem_t sem_moedas;
extern sem_t sem_funcionarios;


/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
extern Queue *gate_queue;

#endif