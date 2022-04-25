#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct{                      //***********************************************

int id;                                        //Struct para configurar os argmentos das threads
int length;

} thread_arg, *ptr_thread_arg; //***********************************************

pthread_t threads[2];   //Declaração threads
int flag[2];
int turn;


void *thread_func(void *arg){


	ptr_thread_arg  targ = (ptr_thread_arg) arg;
	int i;

	for(i = targ->id; i < targ->length; i +=2){


		if(targ->id == 0){


			flag[0] = 1;
	   	
		       while (flag[1] == 1 && turn == 1);
	

			printf("Thread par  %d - valor %d\n", (int)pthread_self(),i);


			flag[0] = 0;
		       turn = 1;


			sched_yield();
		}else{

			flag[1] = 1;
	   	
		       while (flag[0] == 1 && turn == 0);


			printf("Thread impar  %d - valor %d\n", (int)pthread_self(),i);


			flag[1] = 0;
		        turn = 0;

			sched_yield();
		
		}		

	}

}



int main(){

	thread_arg arguments[2];
	int i, j;

       flag[0] = 0;
       flag[1] = 0;
       turn = 1;	


	for(i = 0; i < 2; i++){
		arguments[i].id = i;           
		arguments[i].length = 10; //Confugurando os parâmetros das threads

		pthread_create(&(threads[i]), NULL, thread_func, &(arguments[i])); // Criando as threads

	}
	
    for(j = 0; j < 2; j++){
		pthread_join(threads[j], NULL); //Inserir as threads na fila de pronto

    }

	return 0;


}





















