#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc,char *argv[]){
	int status,pid,length=0;
	char line[256];
	char *ptr=line;
	char path[20]={0,};
	char opt[20]={0,};
	char *pathname[]= {"/bin","/usr/bin","/usr/local/bin"};
	while(1){
		reset:
		int i=0,j=0,cnt=0;
		memset(path,0,sizeof(path));
		memset(opt,0,sizeof(opt));
		fgets(line,256,stdin);
		
		while(*(ptr+i) != ' '){
			path[j]= *(ptr+i);
			i++;
			j++;
		}
		i++;
		j=0;
		while(*(ptr+i)!= '\n'){
			opt[j]=*(ptr+i);
			i++;
			j++;
		} //fgets input(line) divide path, opt

		if(!strcmp(path,"setpath")){ //setpath code start
			//memset(&pathname,0,sizeof(pathname));
			if(cnt==0){
				for(int i=0;i<3;i++)
					pathname[i]= NULL;//like memset

				if(!strcmp(opt,"\0"))
					;
				else{
					pathname[cnt]=opt;
					printf("%s\n",*pathname);
					cnt++;
				}
			}
			else{
				if(!strcmp(opt,"\0"))
					;
				else{
					pathname[cnt]=opt;
					printf("%s\n",*pathname);
					cnt++;
				}
			}
		} //setpath code end
	
		while(pathname[length] != NULL){	length++;}// path number count
								 			
		for(i=0;i<length;i++){ //main function
			chdir(pathname[i]);
			if(access(path,F_OK) == 0){
				printf("%s:vailed file:[%s]\n",pathname[i],path);
				if(fork()==0){
					/*if(!strcmp(path,"cd")){
						printf("input cd\n");
						chdir(opt);
					}*/
					if(!strcmp(path,"cat")){
						execl(path,"cat",opt,0);
						//execl(pathname[i],path,opt,0);
					}
					else{	
						printf("pid:%d, ppid%d\n",getpid(),getppid());
						if(opt[0]== 0)
							execl(path,opt,0);
						else
							execl(path,"min",opt,0);
					
					}
				}
				else{
					wait(&status);
					goto reset;
				}
			}
		}
		printf("%s:command not found\n",pathname[i-1]);
	}
 }
