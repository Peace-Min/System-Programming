#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

int main(int argc,char *argv[]){
	int status,pid,length=0;
	char line[256];
	char *ptr=line;
	char path[20]={0,};
	char opt[20]={0,};
	char dir_path[500]={0, };
	char *pathname[]= {"/bin/","/usr/bin/","/usr/local/bin/"};
	bool exit_chk=false;
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
			if(cnt==0){
				memset(pathname,0,sizeof(pathname));
			//	for(int i=0;i<3;i++){	pathname[i]= NULL;}//like memset
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
		//printf("%d\n",length);
		for(i=0;i<length;i++){
			memset(dir_path,0,sizeof(dir_path));
			strcpy(dir_path,pathname[i]);
			strcat(dir_path,path);
			if( (access(dir_path,F_OK)) == 0){
				exit_chk=true;
				break;
			}
		}
		if(exit_chk==false){
			printf("not exist command\n");
			goto reset;
		}
		if(!strcmp(path,"cd")){ 
			chdir(opt);
		}	
		printf("%s:vailed file:[%s]\n",pathname[i],path);
		if(fork()==0){
			if(opt[0] !='\0')
				execl(dir_path,dir_path,opt,0);
			else
				execl(dir_path,dir_path,0);
		}

		wait(&status);
	}
 }
