#include "min.h"

int main(int argc,char *argv[]){
	char *path[2]={"/bin","/usr/bin"};
	char *opt[10]={0, };
	int cnt=0,clnt,serv;
	char bin[max];
	memset(bin,0,sizeof(bin));
	mknod(CLIENT,S_IFIFO|0666,0);
	/*for(int i=0;i<0xFFFFF;i++){
		if( (clnt=open(CLIENT,O_RDONLY,0666)) == -1)	sleep(1);
		else break;
	}*/
	for(int i=1;i<argc;i++){
		opt[cnt]=(char *)malloc(sizeof(char)*10);
		strcpy(opt[cnt],argv[i]);
		cnt++;
	}
	for(int i=0;i<2;i++){
		strcpy(bin,path[i]);
		strcat(bin,"/");
		strcat(bin,opt[0]);
		if( access(bin,F_OK) == 0){
			break;	
		}
	}
	for(int i=0;i<0xFFFFF;i++){
		if( (serv=open(SERVER,O_WRONLY|O_NDELAY,0666)) == -1)	sleep(1);
		else break;
	}
	if(fork()==0){
		dup2(serv,1);
		execv(bin,opt);
	}
	wait(0);


	for(int i=0;i<0xFFFFF;i++){
		if( (clnt=open(CLIENT,O_RDONLY,0666)) == -1)	sleep(1);
		else break;
	}
	int sz;
	char buf[max];
	//while( (sz=read(clnt,buf,1)) >0)
	//	if( write(1,buf,sz) != sz)	break;
		
	sz=read(clnt,buf,max);
	write(1,buf,sz);
	close(clnt);
	close(serv);
	unlink(CLIENT);
	return 0;
}
