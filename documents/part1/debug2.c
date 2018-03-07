#include <stdlib.h>
#include <stdio.h>

int main(){
	int *data=malloc(100*sizeof(int));
	free(data);
	printf("%d\n",data[3]);
	exit(0);
}
