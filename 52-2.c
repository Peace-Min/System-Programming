#include <stdio.h>
#include <string.h>

int main(){
	char min[500] = "It is useful";
	char kim[500];
	int i=0,j=0;
	while(min[i] !='\0'){
		if(min[i] !=' '){
			kim[j]=min[i];
			i++;
			j++;
		}
		else
			i++;
	}
	kim[j]=0;	
	/*for(i=0;min[i]!='\0';){
		if(min[i] ==' ')
			i++;
		else{
			kim[i]=min[i];
			i++;
		}
		
	}*/
	printf("%s", kim);
}
