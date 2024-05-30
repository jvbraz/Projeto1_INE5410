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
                sem_post(&toy->sem_clientes_no_brinquedo);
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
                sem_post(&toy->sem_clientes_no_brinquedo);
            }
            toy->capacity = toys_map_capacity[toy->id];
            toy->timer = 0;
            debug("Brinquedo [%d] desligado\n", toy->id);
        }
        
    }
    pthread_exit(NULL);
}

// Essa função recebe como argumento informações e deve iniciar os brinquedos.
void open_toys(toy_args *args) {
    // Iniciando os brinquedos
    int total_brinquedos = args->n;
    toy_t **brinquedos = args->toys;

    for (int i = 0; i < total_brinquedos; i++)
    {
        //guardando na variavel global a capacidade do brinquedo
        toys_map_capacity[i] = brinquedos[i]->capacity;

        //Iniciando o semáforo do brinquedo com capacidade total
        sem_init(&brinquedos[i]->sem_clientes_no_brinquedo,0,brinquedos[i]->capacity);

        //definindo um tempo de execução para o brinquedo
        brinquedos[i]->tempo_exec = rand() % 20;

        //Iniciando o timer do brinquedo
        brinquedos[i]->timer = 0;

        //Iniciando o tempo de espera do brinquedo
        brinquedos[i]->tempo_espera = brinquedos[i]->tempo_exec/2;

        //Criando a thread para o brinquedo
        pthread_create(&brinquedos[i]->thread, NULL, &turn_on, brinquedos[i]);
        debug("Brinquedo [%d] iniciado\n", brinquedos[i]->id);
        if (pthread_create(&brinquedos[i]->thread, 0, &turn_on, NULL) != 0) {  // criando a thread passando os argumentos do enjoy
            perror("Não foi possivel criar a thread clientes");
        }
    }
}

// Desligando os brinquedos
void close_toys(){
    // Sua lógica aqui
}