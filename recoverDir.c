#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "param.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
directoryWalker(char *path, int *root, short *inode,char rpath[])
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    printf(2, "directoryWalker: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "directoryWalker: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    printf(1, "%s is not a directory.\n", path);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "directoryWalker: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "directoryWalker: cannot stat %s\n", buf);
        continue;
      }
      char* name=fmtname(buf);
      if((name[0]=='.'&&*root!=1)||(name[0]=='.'&&name[1]=='.')){
    	  continue;
      }
      if(name[0]=='.'){
    	  *root=0;
      }
      printf(1, "name: %s, inode: %d\n", name, st.ino);
      inode[st.ino]=1;
      if(st.size==0 && st.type==T_DIR && name[0]!='.'){
    	  strcpy(rpath,buf);
      }
      if(st.type==T_DIR && name[0]!='.'){
    	  directoryWalker(buf,root,inode,rpath);
      }
    }
    break;
  }
  close(fd);
}

int recovery(void){
	int root=1;
	short inode_dw[NINODE+1]={0};
	short inode_iw[NINODE+1]={0};
	char path[256]={0};
	int inums[NINODE]={0};
	int num_inum=0;
	directoryWalker(".",&root,inode_dw,path);
	iwalk(inode_iw);

	short same=1;
	for (int i=1;i<NINODE;i++){
		if (inode_dw[i]==1 && inode_iw[i]==0){
			printf(1,"directoryWalker finds inode %d, but inodeTBWalker doesn't\n",i);
			same=0;
		}else if (inode_dw[i]==0 && inode_iw[i]==1){
			printf(1,"inodeTBWalker finds inode %d, but directoryWalker doesn't\n",i);
			inums[num_inum++]=i;
			same=0;
		}
	}
	if (same==1&&path[0]==0){
		printf(1,"Recovery finished.\n");
		return 0;
	}else{
		printf(1,"Recovering %s.\n",path);
		if(recDir(path,inums,num_inum)==0){
			printf(1,"Recovery successful.\n");
			return 1;
		}else{
			printf(2,"Recovery failed.\n");
			return 0;
		}
	}
}

int
main(int argc, char *argv[])
{
	if(argc >= 2){
		printf(2,"too many arguments\n");
		exit();
	}
	while(recovery());
	exit();
}
