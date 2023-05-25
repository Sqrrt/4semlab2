#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>


#define MAX_ARGS 512
#define MAX_COMMANDS 16

pid_t proc_id;


int main(int argc, char *argv[]) {
	pid_t proc_id = getpid();
	char* cmd[MAX_COMMANDS][MAX_ARGS];
	char buf[512];
	int cmd_num = 0;
	fgets(buf, 512, stdin);
	cmd[cmd_num][0] = strtok(buf, "|");
	while(cmd[cmd_num][0] != NULL){
		cmd_num++;
		cmd[cmd_num][0] = strtok(NULL, "|");
	}
	for(int i = 0; i < cmd_num; i++){
		char* arg = strtok(cmd[i][0]," \n");
		int arg_num = 0;
		while(arg != NULL){
			cmd[i][arg_num] = arg;
			arg_num++;
			arg = strtok(NULL," \n");
		}
	}
	kill(getppid(), SIGUSR1);
	int pipes_arr[cmd_num-1][2];
	for(int i = 0; i < cmd_num-1; i++){
		pipe(pipes_arr[i]);
	}
	for(int i = 0; i < cmd_num; i++){
		pid_t child_pid = fork();
		if(child_pid < 0){
			printf("fork error");
			exit(EXIT_FAILURE);
		}
		else if (child_pid == 0){
			if(i == 0){
				dup2(pipes_arr[i][1], STDOUT_FILENO);
			}			
			else if(i == cmd_num - 1){
				dup2(pipes_arr[i-1][0], STDIN_FILENO);
			}
			else{
				dup2(pipes_arr[i][1], STDOUT_FILENO);
				dup2(pipes_arr[i-1][0], STDIN_FILENO);
			}
			for(int k = 0; k < cmd_num; k++){
				close(pipes_arr[k][0]);
				close(pipes_arr[k][1]);
			}
			execvp(cmd[i][0], cmd[i]);
		}
	}
	for(int j = 0; j < cmd_num-1; j++){
		close(pipes_arr[j][0]);
		close(pipes_arr[j][1]);
	}
	for(int i = 0; i < cmd_num; i++){
		wait(NULL);
	}
	return 0;
}
