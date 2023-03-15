#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

/**
 * p = get a number from left neighbor
 * print p
 * loop:
 *      n = get a number from left neighbor
 *      if (p does not divide n)
 *          send n to right neighbor
 */
void prime(int msg){
    int n;
    // no message
    if(read(msg, &n, 4) == 0)exit(0);
    printf("%d is prime\n", n);

    int pip[2]; // pipe
    pipe(pip);
    
    int pid = fork();
    
    // create a child process and send message to the child,
    // like what we do in main() function
    if (pid != 0)
    {
        close(pip[0]);
        // i'm parent
        int next_num = -1;
        // get all the number from left neighbor
        while (read(msg, &next_num, 4) != 0)
        {
            if (next_num % n != 0)
            {
                write(pip[1], &next_num, 4);// send it to the right
            }
        }
        close(pip[1]);
        wait(0);
    }
    else
    {
        // i'm child
        close(pip[1]);
        prime(pip[0]);
        close(pip[0]);
    }
}

int main(int argc, char* argv[]){
    int pip[2]; // pipe
    pipe(pip);

    int pid = fork();
    if(pid != 0){
        // root
        close(pip[0]); // do not read
        for (int i = 2; i <= 35; i++)
        {
            write(pip[1], &i, 4); // send message to childs
        }
        close(pip[1]);// write done
        wait(0);

    }else{
        // first child
        close(pip[1]); // i won't write
        prime(pip[0]); // read
        close(pip[0]); //read done

    }
    exit(0);
}
