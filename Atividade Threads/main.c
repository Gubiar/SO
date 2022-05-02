#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int turn;


typedef struct
{ //***********************************************

    int id; // Struct para configurar os argmentos das threads
    int length;

} thread_arg, *ptr_thread_arg; //***********************************************

pthread_t threads[2]; // Declaração threads
pthread_mutex_t mut;

void *thread_func(void *arg) {


    ptr_thread_arg targ = (ptr_thread_arg)arg;
    int i;

    for (i = targ->id; i < targ->length; i += 2)
    {

        if (targ->id == 0)
        {
            pthread_mutex_lock(&mut);
            if (turn == 0)
            {
                printf("Thread par  %d - valor %d\n", (int)pthread_self(), i);
                turn = 1;
            }else{
                i-=2;
            }
                pthread_mutex_unlock(&mut);
                sched_yield();

        }
        else
        {
            pthread_mutex_lock(&mut);

            if (turn == 1)
            {
                printf("Thread impar  %d - valor %d\n", (int)pthread_self(), i);
                turn = 0;
            }else{
                i-=2;
            }
                pthread_mutex_unlock(&mut);
                sched_yield();

        }
    }
}

int main() {

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