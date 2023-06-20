#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <pthread.h>
#define max 2048

struct msg{
	char pid[50];
	char cmd[100];
};
#define SVR "11"
#define SERVER "11"
#define CLIENT "22"
