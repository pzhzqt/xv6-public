#include <stdlib.h>
#include <stdio.h>

//(Qitian Zeng: A20324028)(Deman Yang: A20395988)
int main(){
	int *data=malloc(100*sizeof(int));
	free(data);
	printf("%d\n",data[3]);
	exit(0);
}
