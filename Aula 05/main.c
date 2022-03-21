#include "header.h"

int main()
{
    int pid;
    int status;

    pid = fork();

    if (pid < 0)
    {
        perror("Error: error no fork.\n");
    }
    else if (pid == 0)
    {

        pid = fork();

        if (pid < 0)
        {
            perror("Error: error no fork.\n");
        }
        else if (pid == 0)
        {
            wait(&status);
            sleep(1);
            printf("Eu sou o processo C %d, filho do processo %d\n", getpid(), getppid());
        } else{
            wait(&status);
            sleep(1);
            printf("Eu sou o processo B %d, filho do processo %d\n", getpid(), getppid());
        }
    }
    else
    {
        wait(&status);
        sleep(1);
        printf("Pai de todos A %d \n", getpid());
    }

    return 0;
}