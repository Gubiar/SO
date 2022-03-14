#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>


int count_sigint = 0;

bool controle = true;
int resposta = 0;


void sigint_counter(int signum)
{
    count_sigint++;
}

void SIGTSTP_continue(int signum)
{
    printf("Você digitou Ctrl + C %d vezes.\n", count_sigint);
}

void alarmHandler(int signum)
{
    printf("Se passou 20 segundos. Saindo do programa\n");
    controle = false;
}
