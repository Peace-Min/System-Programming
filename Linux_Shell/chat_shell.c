#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <errno.h>
#include <pwd.h>
#include <limits.h>

#define MAX_PATHS 5
#define MAX_PATH_LEN 500
#define MAX_CMD_LEN 256
#define MAX_ARGS 10
#define MAX_FILE_LEN 20
int execute(char* cmd_args[MAX_ARGS]);
bool find_executable(char* cmd, char* path);
void set_path();
void load_path();
void parse_cmd(char* cmd, char* cmd_args[MAX_ARGS]);
void read_cmd(char* cmd);
void print_welcome();
void print_prompt();

char* pathname[MAX_PATHS];
char* username;
char cwd[PATH_MAX];
int main() {
    char cmd[MAX_CMD_LEN];
    char* cmd_args[MAX_ARGS];
    int status = 0;

    load_path(); //memory set
    username = getpwuid(getuid())->pw_name;
    print_welcome(); 
    while (true) {
    	print_prompt();
        read_cmd(cmd);
        parse_cmd(cmd, cmd_args);
        
        if (strcmp(cmd_args[0], "setpath") == 0) { 
			set_path();
            continue;
        }
        else if (strcmp(cmd_args[0], "load") == 0) { 
			load_path();
            continue;
        }
        
        status = execute(cmd_args);
		if(strcmp(cmd_args[0], "cd") == 0){
			getcwd(cwd, PATH_MAX); // get the current working directory
    		printf("Current directory: %s\n", cwd);
		}
    }
    
   // return status;
}
void print_welcome() {
    getcwd(cwd, PATH_MAX); // get the current working directory

    printf("\n");
    printf("########################################################\n");
    printf("#                                                      #\n");
    printf("#              Welcome to My Decorated Shell!          #\n");
    printf("#                                                      #\n");
    printf("########################################################\n");
    printf("\n");
    printf("Username: %s\n", username);
    printf("Current directory: %s\n", cwd);
    printf("\n");
}
void print_prompt() {
    getcwd(cwd, PATH_MAX); // get the current working directory

    printf("\033[1;32m%s@myshell:\033[0m\033[1;34m%s\033[0m$ ", username, cwd); // print the prompt
}
void read_cmd(char* cmd) {
    printf("> ");
    fgets(cmd, MAX_CMD_LEN, stdin);
    if (cmd[strlen(cmd)-1] == '\n')
        cmd[strlen(cmd)-1] = '\0'; // remove newline character
}

void parse_cmd(char* cmd, char* cmd_args[MAX_ARGS]) {
    int i = 0;
    cmd_args[i] = strtok(cmd, " ");
    while (cmd_args[i] != NULL && i < MAX_ARGS - 1) {
        cmd_args[++i] = strtok(NULL, " ");
    }
    cmd_args[i] = NULL;
}

void load_path() {
    FILE* fp;
    char path[MAX_PATH_LEN];
    int i = 0;
	char filename[MAX_FILE_LEN];
	strcpy(filename,"myShellRc");

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: Could not open file.\n");
        return;
    }

    while (fgets(path, MAX_PATH_LEN, fp) != NULL && i < MAX_PATHS) {
        path[strcspn(path, "\n")] = '\0'; // remove newline character
        pathname[i] = malloc(sizeof(char) * (strlen(path) + 1));
        strcpy(pathname[i++], path);
    }

    fclose(fp);
}

void set_path() {
    int i;
    char path[MAX_PATH_LEN];

    for (i = 0; i < MAX_PATHS; i++) {
        printf("Enter path %d: ", i + 1);
        fgets(path, MAX_PATH_LEN, stdin);
        path[strcspn(path, "\n")] = '\0'; // remove newline character
        pathname[i] = realloc(pathname[i], sizeof(char) * (strlen(path) + 1));
        strcpy(pathname[i], path);
    }
}

bool find_executable(char* cmd, char* path) {
    char executable[MAX_PATH_LEN];
    for (int i = 0; i < MAX_PATHS; i++) {
        strcpy(executable, pathname[i]);
        strcat(executable, cmd);
        if (access(executable, X_OK) == 0) {
            strcpy(path, executable);
            return true;
        }
    }
    return false;
}

int execute(char* cmd_args[MAX_ARGS]) {
    char path[MAX_PATH_LEN];
    int status = 0;
	
    if (!find_executable(cmd_args[0], path)) {
        printf("Command not found.\n");
        return status;
    }

    if (strcmp(cmd_args[0], "cd") == 0) {
        if (cmd_args[1] == NULL) {
            printf("Error: No directory provided.\n");
            return status;
        }
        if (chdir(cmd_args[1]) != 0) {
            printf("Error: Could not change directory.\n");
            return status;
        }
        return status;
    }

    pid_t pid = fork();
    if (pid == -1) {
    printf("Error: Failed to fork process.\n");
    return status;
	}
	else if (pid == 0) {
    // child process
    	execv(path, cmd_args);
    	printf("Error: Failed to execute command.\n");
    	exit(1);
	}
	else {
    // parent process
    wait(&status);
	}

	return status;
}
