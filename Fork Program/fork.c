#include<stdio.h>
#include<unistd.h>
void wait();
void main(){
	int pd;
	pd=fork();
	if(pd==0){
		printf("\nChild Process Id: %d",getpid());
		
	} else if(pd>0){
		wait();
		printf("\nParent Process Id: %d\n",getppid());
		
	}
	else{
		printf("\nFork failed");
	}
}
