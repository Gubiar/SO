#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <pthread.h>
 
#define SHM_SIZE 1024

typedef struct
{ //***********************************************

    int id; // Struct para configurar os argmentos das threads
    int length;

} thread_arg, *ptr_thread_arg;

const int REP = 5;
char dado;
key_t key;
int shmid,flagsid;
char *data;
int turn;

int counter = 0;
int in = 0;
int out = 0;
char buf[REP];


pthread_t threads[2]; // Declaração threads
pthread_mutex_t mut;


int producer(int n) {

    data[n] = (char) n + 0x61;	
    printf("Stored... %c \n", data[n]);
    return n;
    	
}
 
int consumer(int n) {
    
    dado = data[n];
    data[n] = ' ';  
    printf("Consumed... %c \n", dado);
    return n;
 
}


void *thread_func(void *arg) {


    ptr_thread_arg targ = (ptr_thread_arg)arg;
    int i;

    for (i = targ->id; i < targ->length; i ++) {

        if (targ->id == 0) {
            pthread_mutex_lock(&mut);
            if (turn == 0)
            {
                while(1){
                    while (counter == REP);
                    buf[in] = producer(i);
                    in = ++in % REP;	
                    counter++; 
                } 
                turn = 1;
            }else{
                i--;
            }
                pthread_mutex_unlock(&mut);
                sched_yield();

        }
        else {
            pthread_mutex_lock(&mut);

            if (turn == 1)
            {
                while(1){
                    while (counter == 0);
                    consume (i); 
                    out = ++out % REP;
                    counter--; 
                } 
                // consumer(i);
                turn = 0;
            }else{
                i--;
            }
                pthread_mutex_unlock(&mut);
                sched_yield();

        }
    }
}
 
int main()
{
    printf("The Producer x Consumer Problem\n");
    int status;
     
    key = ftok("/home", 'A');                      //  Segmento   
    shmid = shmget(key, 1024, 0644 | IPC_CREAT);   //
    data = (malloc(5*sizeof(char)));               //     1        
    data = shmat(shmid, (void *)0, 0);             //
    
    // //Peterson
    // key = ftok("/home/downloads", 'B');            //  Segmento   
    // flagsid = shmget(key, 1024, 0644 | IPC_CREAT); //    
    // flags = (malloc(3*sizeof(int)));               //     2
    // flags = shmat(flagsid, (void *)0, 0);  
            //  
    // int pid = fork();
   	
    	
   	
    // if(pid == 0){
	//     producer(5);	    
    // }else{
            
    //         consumer(5);
            
    // 	    shmdt(data);                              //segmento 3  
    // 	    shmctl(shmid, IPC_RMID, NULL);            //
    	    
    // 	    shmdt(flags);                             //segmento 4 
    // 	    shmctl(flagsid, IPC_RMID, NULL);		// 
    	    
    	    
    	    		   
    // }	

    thread_arg arguments[2];
    int i;
    turn = 0;

    for (i = 0; i < 2; i++) {
        arguments[i].id = i;
        arguments[i].length = 10; // Confugurando os parâmetros das threads

        pthread_create(&(threads[i]), NULL, thread_func, &(arguments[i])); // Criando as threads
    }

    for (int j = 0; j < 2; j++) {
        pthread_join(threads[j], NULL); // Inserir as threads na fila de pronto
    }
        
    return 0;
    
}    