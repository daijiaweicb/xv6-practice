#include <kernel/types.h>
#include <kernel/stat.h>
#include <user/user.h>

int main()
{
    int p1[2];
    int p2[2];

    if(pipe(p1) < 0 || pipe(p2) < 0)
    {
        printf("pipe failed\n");
    }

    int pid = fork();
    if(pid < 0) 
    {
        printf("fork failed\n");
    }
    else if (pid == 0) // the child
    {
        close(p1[1]); // close parent write
        close(p2[0]); //close child read

        char buf;

        if(read(p1[0], &buf ,1) != 1 )
        {
            printf("child read failed\n");
            exit(1);
        }

        printf("%d: received ping\n",getpid());
        
        if(write(p2[1],&buf ,1) != 1)
        {
            printf("child write failed\n");
            exit(1);
        }
        
        close(p1[0]);
        close(p2[1]);
        exit(0);

    }
    else // the parent
    {
        close(p1[0]);
        close(p2[1]);

        char buf = 'a';

        if(write(p1[1],&buf ,1) != 1)
        {
            printf("parent write failed\n");
            exit(1);
        }

        if(read(p2[0], &buf, 1) != 1)
        {
            printf("parent read failed\n");
        }

        printf("%d: received pong\n",getpid());

        close(p1[1]);
        close(p2[0]);
        wait(0);
        exit(0);

    }
}