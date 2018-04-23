#include "types.h"
#include "user.h"

int main(int argc,char* argv[]){
	if(argc>1){
		printf(2,"too many arguments.\n");
	}else{
		if(recType()==0){
			printf(1,"Recover finished.\n");
		}else{
			printf(2,"Recover failed.\n");
		}
	}
	exit();
}
