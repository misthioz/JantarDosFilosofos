#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<semaphore.h>

#define COMENDO 2
#define FOME 1
#define PENSANDO 0

sem_t garfos[5];
sem_t mutex;
int v_filosofos[5] = {1, 2, 3, 4, 5}; 
int estadoFilosofo[5];

//funcoes para facilitar recuperar os vizinhos de um Filosofo i
int vizinhoEsquerdo(int i){
	return (i+4)%5;
}

int vizinhoDireito(int i){
	return (i+1)%5
}

//verificando os estados dos viznhos de um Filosofo i
void 

int main(){
	//inicializando o vetor de semaforos (garfos)
	for(int i=0;i<5;i++){
		sem_init(&garfos[i], 0, 1);
	}

}
