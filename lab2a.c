#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define SECONDS_TO_TERMINATE 3

void sig_handler(int sig){
	printf("A received signal B \n");
}

int main(){
	pid_t pid;
	pid = fork();
	signal(SIGUSR1, sig_handler);
	if(pid < 0){
		printf("fork error");
	}
	else if(pid == 0){
		execl("./lab2","program B", NULL);
	}
	pause();
	sleep(SECONDS_TO_TERMINATE);
	if(waitpid(pid, NULL, WNOHANG) == 0){
		printf("B didn't finish within %d seconds\n", SECONDS_TO_TERMINATE);
		killpg(getpgid(pid), SIGTERM);
		printf("B terminated\n");
	}
	return 0;
}
