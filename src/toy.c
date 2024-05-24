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


// Thread que o brinquedo vai usar durante toda a simulacao do sistema
void *turn_on(void *args){

    debug("[ON] - O brinquedo  [%d] foi ligado.\n", rand()); // Altere para o id do brinquedo

    debug("[OFF] - O brinquedo [%d] foi desligado.\n", rand()); // Altere para o id do brinquedo

    pthread_exit(NULL);
}



// Essa função recebe como argumento informações e deve iniciar os brinquedos.
void open_toys(toy_args *args){
    // Sua lógica aqui
    int total_brinquedos = args->n;  // contêm o total de brinquedos
    pthread_t brinquedo[total_brinquedos];
    /*entrando no array de brinquedos*/    
    for (int i = 0; i < total_brinquedos; i++)
    {
        brinquedo[i] = args->toys[i]->thread;  // carregando as threads dos brinquedos
        // verificando se foi possivel criar a thread
        if (pthread_create(&brinquedo[i], 0, &turn_on, NULL) != 0) {
            perror("Não foi possivel criar a thread brinquedo");
        }
    }
    // Iniciando os joins
    for (int i = 0; i < total_brinquedos; i++) {
        // verifica se o join foi bem sucedido 
        if (pthread_join(brinquedo[i], NULL) != 0) {
            perror("Não foi possivel dar join na thread brinquedo");
        }
    }
}

// Desligando os brinquedos
void close_toys(){
    // Sua lógica aqui
}