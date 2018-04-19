#include "types.h"
#include "user.h"
#include "param.h"

int main(){
	short inode[NINODE+1]={0};
	iwalk(inode);
	exit();
}
