#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]){
    // 0用于读，1用于写
    int p2c[2], c2p[2]; // 两个管道
    pipe(p2c); // 父进程 -> 子进程 的管道
    pipe(c2p); // 子进程 -> 父进程 的管道

    if(fork() != 0){
        // I'm Parent
        write(p2c[1], "!", 1); // send to child
        char buf;
        read(c2p[0], &buf, 1); // read from child  
        printf("%d: received pong!\n", getpid());
        wait(0);
    }
    else{
        // I'm Child
        char buf;
        read(p2c[0], &buf, 1); // read from parent  
        printf("%d: received ping!\n", getpid());
        write(c2p[1], "!", 1); // send to child 
    }


    exit(0);
}