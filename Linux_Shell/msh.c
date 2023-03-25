#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

int main(int argc,char *argv[]){
	int status,length=0;
	char line[257];
	char *ptr=line;
	char dir_path[500]={0, };
	char *pathname[]= {"/bin/","/usr/bin/","/usr/local/bin/"};
	bool exit_chk=false;
	char *path1[3];
	char min[10];
	int i,j,s=0,cnt=0;
	bool set_chk=false;
	bool load_chk=false;
	while(1){
		reset:
		memset(line,0,sizeof(line));
		memset(min,0,sizeof(min));
		fgets(line,256,stdin);
		cnt=0,i=0,j=0,set_chk=false,load_chk=false;

		while(*(ptr+i)!= '\n'){
			if(*(ptr+i) == ' '){
				path1[cnt]=(char*)malloc(sizeof(char) *10);
				strcpy(path1[cnt],min);
				j=0;
				memset(min,0,sizeof(min));
				cnt++;
				i++;
			}
			min[j++]=*(ptr+i++);
		}

		path1[cnt]=(char *)malloc(sizeof(char)*10);
		strcpy(path1[cnt],min); //string divide

		if(!strcmp(path1[0],"load")){
			FILE *fd;
			int c=0;
			fd=fopen("myShellRc","r");

			for(int s=0;s<3;s++)
				pathname[s]=(char *)malloc(sizeof(char)*10);

			while( (fscanf(fd,"%s",&(*pathname[c]))) != EOF){	
				printf("%s\n",pathname[c]);
				c++;
			}
			fclose(fd);
			load_chk=true;
		} //load path
		if(load_chk==true) goto reset;

		if(!strcmp(path1[0],"setpath")){ //setpath code start
			memset(pathname,0,sizeof(pathname));
			char cat[]="/";
			for(s=0;s<3;s++){
				pathname[s]=(char *)malloc(sizeof(char)*10);
				printf("[%d]: Input path\n",s+1);
				scanf("%s",&(*pathname[s]));
				strcat(pathname[s],cat);
				//printf("%s",&(*pathanme[s]));
				printf("%s\n",*(pathname+s));
			}
			for(int c=0;c<=cnt;c++)
				free(path1[c]);

			set_chk=true;
		} //setpath code end
		if(set_chk==true)	goto reset;

		while(pathname[length] != NULL){	length++;}// path number count

		for(int i=0;i<length;i++){				  
			memset(dir_path,0,sizeof(dir_path));
			strcpy(dir_path,pathname[i]);
			strcat(dir_path,path1[0]);
			if( (access(dir_path,X_OK)) == 0){
				exit_chk=true;
				break;
			}
		}
		if(exit_chk!=true){
			printf("not exist command\n");
			continue;
		}
		else{
			printf("vailed file:[%s]\n",dir_path);
			if(!strcmp(path1[0],"cd")){
				chdir(path1[1]);
			}
			if(fork()==0){
				path1[++cnt]=(char *)malloc(sizeof(char)*10);
				path1[cnt]=(char *)NULL;
				execv(dir_path,path1);
			}
		}
		wait(&status);

		for(int c=0;c<=cnt;c++)
			free(path1[c]);
	}
 }
