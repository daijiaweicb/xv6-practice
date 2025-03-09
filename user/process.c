#include <kernel/types.h>
#include <kernel/stat.h>
#include <user/user.h>

int main(int argc,char* argv[])
{
    int pid = fork();
    if(pid > 0) //in the parent, fork returns child's pid
    {
        printf("parent: child = %d\n",pid); 
        pid = wait((int*)0);
        printf("child %d is done\n",pid);
    }
    else if(pid == 0) // in the child, fork returns zero
    {
        printf("child exiting\n");
        exit(0);
    }
    else{
        printf("fork error\n");
    }
}
