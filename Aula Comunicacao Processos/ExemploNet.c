#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1
#define STDOUT 1


int main() {
    int n, fd[2];
    pid_t pid;

    if (pipe(fd) < 0){

        fprintf(stderr, "Erro no tubo\n");
        _exit(1);
    }

    if( (pid = fork()) < 0){
        fprintf(stderr, "Erro no fork\n");
        _exit(1);
    }


    if(pid > 0){

        #define MAX 128
        char line[MAX];
        close(fd[WRITE]);
        n = read(fd[READ], line, MAX);
        write(STDOUT, &line[0], n);
        close(fd[READ]);
        kill(pid, SIGKILL);
        _exit(0);

    }

    if(pid == 0){

        #define LEN 8
        char msg[LEN] = {'T', 'e', 's', 't', 'a', 'd', 'o', '\n'};
        close(fd[READ]);
        write(fd[WRITE], &msg[0], LEN);
        close(fd[WRITE]);
        pause();

    }


}