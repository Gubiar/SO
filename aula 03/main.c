#include "header.h"


int main() {

    signal(SIGALRM, alarmHandler); // Inico do programa
    signal(SIGINT, sigint_counter); /* Ouvir Ctrl + C */
    signal(SIGTSTP, SIGTSTP_continue); /* Ouvir Ctrl + Z */

    while(controle){

        if(count_sigint == 3){

            alarm(20);                      /* 20 segundos */
            pause();
            printf("Deseja continuar o programa?\n Responda com 1 para SIM e 0 para Não.\n");
            scanf("%d", &resposta);

            if(resposta == 1){
                controle = true;
                count_sigint = 0;
            }else{
                controle = 0;
                count_sigint = 0;
            }
        }

    }
    return 0;
}