#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

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
directoryWalker(char *path, int *root)
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
      if(st.type==T_DIR && name[0]!='.'){
    	  directoryWalker(buf,root);
      }
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;
  int root=1;

  if(argc < 2){
    directoryWalker(".",&root);
    exit();
  }
  for(i=1; i<argc; i++)
    directoryWalker(argv[i],&root);
  exit();
}
