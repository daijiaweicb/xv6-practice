#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void process_read(int pipe_read) {
    int p, n;
    int pipe_write[2];

    if (read(pipe_read, &p, sizeof(p)) == 0) {
        close(pipe_read);
        exit(0);
    }
    printf("prime %d\n", p);

    pipe(pipe_write);
    if (fork() == 0) {
        close(pipe_write[1]);   // 关闭新管道写端
        close(pipe_read);       // 关闭原管道读端（关键修复）
        process_read(pipe_write[0]); // 递归处理新管道
        exit(0); // 确保子进程退出
    } else {
        close(pipe_write[0]);
        while (read(pipe_read, &n, sizeof(n)) > 0) {
            if (n % p != 0) {
                write(pipe_write[1], &n, sizeof(n));
                
            }
        }
        close(pipe_write[1]);
        close(pipe_read);
        wait(0);
        exit(0);
    }
}

void process_read(int pipe_read) __attribute__((noreturn));

int main() {
    int pipes[2];
    pipe(pipes);
    for (int i = 2; i <= 129;i++) {
        write(pipes[1], &i, sizeof(i));
    }
    
    close(pipes[1]);  // 关闭写端，传递EOF
    process_read(pipes[0]);

    exit(0);
}