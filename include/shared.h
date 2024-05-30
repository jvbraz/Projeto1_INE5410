#ifndef __SHARED_H__
#define __SHARED_H__

#include <defs.h>
// Você pode declarar novas funções (ou variaveis compartilhadas) aqui



/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
extern Queue *gate_queue;
extern int toys_map_capacity[MAX_CAPACITY_TOY];
extern int total_clientes;
extern int clientes_na_fila;
extern int clientes_no_parque;
extern int total_brinquedos;
extern pthread_mutex_t mutex_fila;
extern sem_t sem_queue;
extern sem_t *sem_brinquedos;

#endif