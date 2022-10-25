#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char *path, char *target){
  char buf[512], *p, *q;
  int fd;
  struct dirent de;
  struct stat st;

  for(q=path+strlen(path); q >= path && *q != '/'; q--)
    ;
  q++;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }
  switch(st.type){
  case T_FILE:
    // printf("q: %s", q);
    if (!strcmp(target, q)) printf("%s\n", path);
    close(fd);
    break;

  case T_DIR:
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';
        if (de.inum == 0 || !strcmp(de.name, ".") || !strcmp(de.name, "..")) continue;
        strcpy(p, de.name);
        p = p + strlen(de.name);
        *p = 0;
        find(buf, target);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[]){
    if (argc == 2){
        find(".", argv[1]);
    }
    else if (argc == 3){
        find(argv[1], argv[2]);
    }
    else{
        printf("Wrong number of args!\n");
    }
    exit(0);
}