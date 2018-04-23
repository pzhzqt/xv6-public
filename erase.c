#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"

int
main(int argc, char *argv[])
{
	if (argc==1){
		printf(1,"need more arguments.\n");
	}else{
		for(int i=1;i<argc;i++){
			dirErase(argv[i]);
		}
	}
	exit();
}
