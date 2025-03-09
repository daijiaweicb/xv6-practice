#include <kernel/types.h>
#include <kernel/stat.h>
#include <user/user.h>

int main()
{
    int pid = fork();
    if (pid == 0)
    {
        char *argv[] = {"echo","Hello", 0};
        exec("/bin/echo", argv);
        fprintf(2,"exec failed\n");
        exit(1);
    }
    else
    {
        wait(0);
    }
    exit(0);
}


