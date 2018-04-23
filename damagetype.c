#include "types.h"
#include "user.h"

int main(int argc, char* argv[]){
	if (argc<2){
		printf(2,"need more argument.\n");
	}else{
		for(int i=1;i<argc;i++){
			char* path=argv[i];
			if(dtype(argv[i])<0){
				printf(1,"can't damage %s.\n",path);
			}else{
				printf(1,"%s damaged.\n",path);
			}
		}
	}
	exit();
}
