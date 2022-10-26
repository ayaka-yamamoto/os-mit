#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int read_stdin(char dst[]){
    char *p = dst;
    while(read(0, p, 1)){
        if (*p == '\n'){
            *p = 0;
            return 1;
        }
        p++;
    }
    return 0;
}

int main(int argc, char *argv[])
{   
    if (argc < 2){
        printf("Wrong number of args!\n");
        exit(0);
    }
    char buf[256], *xargv[MAXARG];
    int i = 1;
    while (i < argc){
        xargv[i-1] = argv[i];
        i++;
    }
    xargv[argc] = argv[argc];
    while (read_stdin(buf) != 0){
        if (fork()==0){
            xargv[argc-1] = buf;
            exec(xargv[0], xargv);
            exit(0);
        }
    }
    wait(0);
    exit(0);
}