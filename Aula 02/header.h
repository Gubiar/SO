#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

char  * msg1 = "ALARME";

void myprintf(int x, int y, char *msg)
{
    int size = strlen(msg);
    char msg2[11];

    strcpy(msg2, "\033[ B\033[ C");

    msg2[2] = x + '0';
    msg2[6] = y + '0';

    write(1, msg2, 11);
    write(1, msg, size);

    int size2 = strlen(msg2);
}

void sig_handler(int signum)
{

    printf("CUIDADO\n");
}

void alarmHandler()
{
    printf("%s \n", msg1);
}

void printsinc(char *msg, int segundos)
{
    // int size = strlen(msg);

    //  while (true) {
    //      alarm(segundos);
    //      pause();
    //      signal(SIGALRM,sig_handler);
    //  }

    signal(SIGALRM, alarmHandler); /* Install signal handler */
    alarm(3);                      /* Schedule an alarm signal in three seconds */
    printf("Inicio do alarme\n");
    pause();
    printf("Fim do alarme\n");
}