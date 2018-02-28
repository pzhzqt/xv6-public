#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "syscall.h"

int main(void){
	write(1,"123\n",4);
	printf(1,"write called %d times\n",getCallCount(SYS_write));
	exit();
}
