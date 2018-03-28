//(Qitian Zeng: A20324028)(Deman Yang: A20395988)
#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "syscall.h"

int main(void){
	myMemory();
	char *arr1=malloc(4096*sizeof(char));
	arr1[0]='0';
	myMemory();
	char *arr2=malloc(1024*4096*sizeof(char));
	arr2[0]='0';
	myMemory();
	free(arr2);
	myMemory();
	char *arr3=malloc(1024*4096*sizeof(char));
	arr3[0]='0';
	myMemory();
	free(arr1);
	free(arr3);
	char *arr4=malloc(1024*1024*1024*sizeof(char));
	arr4[0]='0';
	myMemory();
	exit();
}
