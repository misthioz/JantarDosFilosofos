#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<semaphore.h>
#include <pthread.h>

#define COMENDO 2
#define FOME 1
#define PENSANDO 0

int vFilosofos[5] = {1, 2, 3, 4, 5};
sem_t semFilosofos[5];
sem_t arbitro; 
int estadoFilosofo[5];

//funcoes para facilitar recuperar os vizinhos/garfos  de um Filosofo i
int esquerdo(int i){
	return (i+4)%5;
}

int direito(int i){
	return (i+1)%5;
}

//verifica os estados dos vizinhos e do  Filosofo numFilosofo - se ele estiver com fome e eles nao estiverem comendo ele pode comer
void verifica(int numFilosofo){
	if(estadoFilosofo[numFilosofo] == FOME){
		if(estadoFilosofo[esquerdo(numFilosofo)] != COMENDO && estadoFilosofo[direito(numFilosofo)] != COMENDO){
			//setando o estado do filosofo
			estadoFilosofo[numFilosofo] = COMENDO;
			printf("\nO filosofo %d está comendo.",numFilosofo);
			//espera
			sleep(1);
			//sinaliza qye terminou de comer
			sem_post(&semFilosofos[numFilosofo]);

		}
			
	}
}

void pegarGarfo(int numFilosofo){
	//entra na regiao critica, outros filosos nao podem pegar o garfo ao mesmo tempo
	sem_wait(&arbitro);

	//seta estado como fome
        estadoFilosofo[numFilosofo] = FOME;
        printf("\nO filosofo %d esta com fome!", numFilosofo);


	//verifica se ele pode comer (se esta com fome e os vizinhos nao estao comendo), se sim, dentro da funcao verifica() muda seu estado para comendo
	//se nao, ele espera ate que o filosofo que esta comendo sinalize que acabou de comer
	verifica(numFilosofo);

	//sai da regiao critica, agora outros filosofos podem tentar pegar o garfo
	sem_post(&arbitro);

	//volta para 0
	sem_wait(&semFilosofos[numFilosofo]);
}

void soltarGarfo(int numFilosofo){
	//entra na regiao critica, outros filosofos nao podem interagir com o garfo ate este filosofo soltar
	sem_wait(&arbitro);	

	//quando o filosofo termina de comer, volta para o estado pensando
	estadoFilosofo[numFilosofo] = PENSANDO;

	//chama a funcao verifica() para os vizinhos e testa se eles podem começar a comer
	verifica(esquerdo(numFilosofo));
	verifica(direito(numFilosofo));

	//sai da regiao critica
	sem_post(&arbitro);
	printf("Eu, Filosofo %d, estou pensando...", numFilosofo);
}

void pensar(int numFilosofo){
	estadoFilosofo[numFilosofo] = PENSANDO;
	//printf("Eu, Filosofo %d, estou pensando...", numFilosofo);
	sleep(5);
}


void* filosofos(void* numFilosofo){
	while(1){
		int *num = numFilosofo;
		pensar(*num);
		pegarGarfo(*num);
		soltarGarfo(*num);
	}
}

int main(){
	pthread_t thread_id[5];
	//inicializando o arbitro - o arbitro controla o acesso a regiao critica - so 2 filosofos no maximo podem comer
	sem_init(&arbitro, 0, 2);
	//inicializando o vetor de semaforos
	for(int i=0;i<5;i++){
		sem_init(&semFilosofos[i], 0, 0);
		
	}

	//criando as threads (filosofos)
	for(int i=0; i<5; i++){
		pthread_create(&thread_id[i], NULL, filosofos, &vFilosofos[i]);
		printf("criei o filosofo %d...", i+1);
	}

	//inicializando threads
	for(int i =0; i<5; i++){
		pthread_join(thread_id[i], NULL);
	}


}
