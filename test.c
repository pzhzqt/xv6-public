#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "syscall.h"

int main(void){
	printf(1,"hello world!\n");
	printf(1,"SYS_write is invoked %d time(s) by printf(1,\"hello world!\\n\")\n",getCallCount(SYS_write));

	char buf[256];
	printf(1,"input a random string (not too long please): ");
	read(0,buf,256);
	printf(1,"SYS_read is invoked %d time(s) by read(0,buf,256)\n",getCallCount(SYS_read));

	int read_before=getCallCount(SYS_read);
	int write_before=getCallCount(SYS_write);
	char x[256];
	strcpy(x,buf);
	printf(1,"SYS_read is invoked %d time(s) by strcpy(x,buf)\n"
			"SYS_write is invoked %d time(s) by strcpy(x,buf)\n",
			getCallCount(SYS_read)-read_before,getCallCount(SYS_write)-write_before);

	int cur_write=getCallCount(SYS_write);
	write(1,"hello world!\n",13);
	printf(1,"SYS_write is invoked %d time(s) by write(1,\"hello world!\\n\",13)\n",getCallCount(SYS_write)
			-cur_write);

	if(fork()==0){
		printf(1,"SYS_fork is invoked %d time(s) by child process\n",getCallCount(SYS_fork));
		exit();
	}else{
		wait();
		printf(1,"SYS_fork is invoked %d time(s) by parent process\n",getCallCount(SYS_fork));

		printf(1,"In total, we invoked getCallCount %d time(s) in current process\n",
				getCallCount(SYS_getCallCount));
		exit();
	}
}
