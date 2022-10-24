#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime_sieve(int *p){
    int buff[1];
    close(p[1]);
    if (read(p[0], buff, 4) == 0){
        close(p[0]);
        exit(0);
    }
    int new_p[2], prime = buff[0];
    printf("prime %d\n", prime);
    pipe(new_p);
    if (fork()==0){
        close(p[0]);
        prime_sieve(new_p);
        exit(0);
    }
    else{
       while (read(p[0], buff, 4) != 0){
        if (buff[0] % prime != 0) write(new_p[1], buff, 4);
       }
       close(p[0]);
       close(new_p[0]);
       close(new_p[1]);
       while (wait(0) != -1);
       exit(0);
    }
}

int main()
{
    int p[2], buff[1];
    pipe(p);
    if (fork()==0){
        prime_sieve(p);
    }
    else{
        close(p[0]);
        for (int i=2; i<36; i++){
            buff[0] = i;
            write(p[1], buff, 4);
        }
        close(p[1]);
        while (wait(0) != -1);
    }
    exit(0);
}
