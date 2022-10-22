#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main()
{
    int p[2];
    char buff[2];
    pipe(p);
    if(fork() == 0){
        int id = getpid();
        read(p[0], buff, 1);
        close(p[0]);
        printf("%d: received ping\n", id);
        write(p[1], buff, 1);
        close(p[1]);
        exit(0);
    }
    else{
        int id = getpid();
        write(p[1], "a", 1);
        close(p[1]);
        read(p[0], buff, 1);
        close(p[0]);
        wait(0);
        printf("%d: received pong\n", id);
        exit(0);
    }
}