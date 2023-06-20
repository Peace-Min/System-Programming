#include "min.h"

int main(){
	int serv,clnt,sz,fd[2];
	pipe(fd);
	mknod(SERVER,S_IFIFO|0666,0);
	char buf[max];
	set:
	for(int i=0;i<0xFFFFF;i++){
		if( (serv=open(SERVER,O_RDONLY)) == -1)	sleep(1);
		else break;
	}	
	for(int i=0;i<0xFFFFF;i++){
		if( (clnt=open(CLIENT,O_WRONLY|O_NDELAY,0666)) == -1){
		sleep(1);
		}
		else {
			break;
		}
	}
	memset(buf,0,sizeof(buf));
	while( (sz=read(serv,buf,max))> 0){
		printf("clnt request\n");
		write(fd[1],buf,sz);
		if(fork()==0){
			dup2(fd[0],0);
			dup2(clnt,1);
			close(fd[0]);
			close(fd[1]);
			execl("/bin/sort","sort",(char *)0);
		}
		close(fd[0]);
		close(fd[1]);
		close(clnt);
		close(serv);
		wait(0);
		//close(clnt);
	}
	goto set;
	return 0;
}
