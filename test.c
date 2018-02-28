#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "syscall.h"

int main(void){
	if (fork()==0){
		printf(1,"fork called %d times in child process\n",getCallCount(SYS_fork));
	}else{
		printf(1,"fork called %d times in parent process\n",getCallCount(SYS_fork));
	}
	exit();
}
