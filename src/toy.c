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


// Thread que o brinquedo vai usar durante toda a simulação do sistema
void *turn_on(void *args) {
    //Brinquedo que esta sendo executado
    toy_t *toy = (toy_t *)args;

    //Fica no loop até todos os clientes sairem do parque
    while (clientes_no_parque > 0 || clientes_na_fila > 0)
    {
        //Se o brinquedo ficar cheio, ou passar um tempo minimo, o brinquedo deve ser ligado
        if (toy->capacity == 0) 
        {
            debug("Brinquedo [%d] ligado\n", toy->id);
            //Espera um tempo para a "execução" do brinquedo
            sleep(5);

            //Reiniciando a capacidade do brinquedo
            for (int i = 0; i < toy->capacity; i++)
            {
                sem_post(&sem_brinquedos[toy->id]);
            }
            toy->capacity = toys_map_capacity[toy->id];
            toy->timer = 0;
            debug("Brinquedo [%d] desligado\n", toy->id);
        }
        //Se o tempo minimo foi alcançado e pelo menos um cliente entrou no brinquedo, o brinquedo deve ser ligado
        else if(toy->timer >= toy->tempo_espera && toy->capacity < toys_map_capacity[toy->id])
        {
            debug("Brinquedo [%d] ligado\n", toy->id);
            //Espera um tempo para a "execução" do brinquedo
            sleep(5);

            //Reiniciando a capacidade do brinquedo
            for (int i = 0; i < toy->capacity; i++)
            {
                sem_post(&sem_brinquedos[toy->id]);
            }
            toy->capacity = toys_map_capacity[toy->id];
            toy->timer = 0;
            debug("Brinquedo [%d] desligado\n", toy->id);
        }        
    }

    //Finalizando a thread
    pthread_mutex_lock(&mutex_finaliza_brinquedos);
    total_brinquedos--;
    if(total_brinquedos == 0)
    {
        pthread_cond_signal(&cond_finaliza_brinquedos);
    }
    pthread_mutex_unlock(&mutex_finaliza_brinquedos);
    pthread_exit(NULL);
}

// Essa função recebe como argumento informações e deve iniciar os brinquedos.
void open_toys(toy_args *args) {
    // Iniciando os brinquedos
    int total_brinquedos = args->n;
    toy_t **brinquedos = args->toys;

    //Iniciando os mutexes e condições 
    pthread_mutex_init(&mutex_finaliza_brinquedos, NULL);
    pthread_cond_init(&cond_finaliza_brinquedos, NULL);

    //Iniciando o array de semáforos dos brinquedos e evitando erros de alocação
    sem_brinquedos = (sem_t *)malloc(total_brinquedos * sizeof(sem_t));
    if(sem_brinquedos == NULL)
    {
        perror("Erro ao alocar memória para o semaforo de brinquedos");
        exit(EXIT_FAILURE);
    }

    //Iniciando os brinquedos
    for (int i = 0; i < total_brinquedos; i++)
    {
        //guardando na variavel global a capacidade do brinquedo
        toys_map_capacity[i] = brinquedos[i]->capacity;

        //Iniciando o semáforo do brinquedo com capacidade total
        sem_init(&sem_brinquedos[i], 0, brinquedos[i]->capacity);

        //definindo um tempo de execução para o brinquedo
        brinquedos[i]->tempo_exec = rand() % 8 + 1;

        //Iniciando o timer do brinquedo
        brinquedos[i]->timer = 0;

        //Iniciando o tempo de espera do brinquedo
        brinquedos[i]->tempo_espera = brinquedos[i]->tempo_exec/2;

        //Criando a thread para o brinquedo
        if (pthread_create(&brinquedos[i]->thread, NULL, &turn_on, brinquedos[i]) != 0) {  // criando a thread passando os argumentos do enjoy
            perror("Não foi possivel criar a thread clientes");
        }
        debug("Brinquedo [%d] iniciado\n", brinquedos[i]->id);
    }
}

// Desligando os brinquedos
void close_toys(){
    //Espera todos os brinquedos serem finalizados
    pthread_mutex_lock(&mutex_finaliza_brinquedos);
    while (total_brinquedos > 0)
    {
        pthread_cond_wait(&cond_finaliza_brinquedos, &mutex_finaliza_brinquedos);
    }
    pthread_mutex_unlock(&mutex_finaliza_brinquedos);

    debug("Todos os brinquedos foram finalizados\n");
    pthread_mutex_destroy(&mutex_finaliza_brinquedos);
    pthread_cond_destroy(&cond_finaliza_brinquedos);
}