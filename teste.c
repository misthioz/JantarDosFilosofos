#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<semaphore.h>
#include <pthread.h>

#define COMENDO 2
#define FOME 1
#define PENSANDO 0

sem_t semFilosofos[5];
sem_t mutex; 
int estadoFilosofo[5];

//funcoes para facilitar recuperar os vizinhos/garfos  de um Filosofo i
int esquerdo(int i){
	return (i+4)%5;
}

int direito(int i){
	return (i+1)%5
}

//verifica os estados dos vizinhos e do  Filosofo numFilosofo - se ele estiver com fome e eles nao estiverem comendo ele pode comer
void verifica(int numFilosofo){
	if(estadoFilosofos[numFilosofo] == FOME){
		if(estadoFilosofos[esquerdo(numFilosofo)] != COMENDO && estadoFilosofos[direito(numFilosofo)] != COMENDO){
			//setando o estado do filosofo
			estadoFilosfos[numFilosofo] = COMENDO;
			printf("\nO filosofo %d está comendo.",numFilosofo);
			//espera
			sleep(1);
			//sinaliza qye terminou de comer
			sem_post(&semFilosofos[numFilosofo]);

		}
			
	}
}

void pegarGarfo(int numFilosofo){
	//seta estado como fome
	estadoFilosofos[numFilosofo] = FOME;

	//entra na regiao critica, outros filosos nao podem pegar o garfo ao mesmo tempo
	sem_wait(&mutex);

	//verifica se ele pode comer (se esta com fome e os vizinhos nao estao comendo), se sim, dentro da funcao verifica() muda seu estado para comendo
	//se nao, ele espera ate que o filosofo que esta comendo sinalize que acabou de comer
	verifica(numFilosofo);

	//sai da regiao critica, agora outros filosofos podem tentar pegar o garfo
	sem_post(&mutex);

	//volta para 0
	sem_wait(&semFilosofos[numFilosofo]);
}

void soltarGarfo(int numFilosofo){
	//entra na regiao critica, outros filosofos nao podem interagir com o garfo ate este filosofo soltar
	sem_wait(&mutex);	

	//quando o filosofo termina de comer, volta para o estado pensando
	estadoFilosofos[numFilosofo] = PENSANDO;

	//chama a funcao verifica() para os vizinhos e testa se eles podem começar a comer
	verifica(esquerdo(numFilosofo));
	verifica(direito(numFilosofo));

	//sai da regiao critica
	sem_post(&mutex);
}


void filosofos(){
	//while true
	//num fiilosofo aleatorio?
	//sleep
	//fome
	//sleep
	//comer
}

int main(){
	//inicializando o vetor de semaforos (garfos)
	for(int i=0;i<5;i++){
		sem_init(&garfos[i], 0, 1);
	}

}
