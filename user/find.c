#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

// fmtname("/usr/bin/find")  // return "/find"
//  fmtname("hello.txt")      // return NULL (because do not have '/')
char *fmtname(char *path) // find the last "/"
{
    char *prev = 0;
    char *curr = strchr(path, '/');
    while (curr != 0)
    {
        prev = curr;
        curr = strchr(curr + 1, '/');
    }
    return prev;
}

void find(char *currpath, char *target)
{
    char buf[512], *p;
    int fd;
    struct dirent de; // directory struct
    struct stat st;   // store file's information

    // struct dirent
    // {
    //     ushort inum;
    //     char name[DIRSIZ];
    // };

    // struct stat {
    //     int dev;     // File system's disk device
    //     uint ino;    // Inode number
    //     short type;  // Type of file
    //     short nlink; // Number of links to file
    //     uint64 size; // Size of file in bytes
    //   };

    if ((fd = open(currpath, O_RDONLY)) < 0) // only file by reading only
    {
        fprintf(2, "find: cannot open %s\n", currpath);
        return;
    }

    if (fstat(fd, &st) < 0) // get the information of the file
    {
        fprintf(2, "find: cannot stat %s\n", currpath);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_DEVICE:
    
    case T_FILE: // Processing of general documents
    {
        char *file_name = fmtname(currpath);
        int match = 1;
        if (file_name == 0 || strcmp(file_name + 1, target) != 0) // because the return value of fmtname have /, so file name nees to +1
        {
            match = 0;
        }
        if (match)
        {
            fprintf(2, "%s\n", currpath);
        }
        close(fd);
        break;
    }

    case T_DIR:
    {
        if (strlen(currpath) + 1 + DIRSIZ + 1 > sizeof(buf))
        {
            fprintf(2, "find: path too long\n");
            close(fd);
            return;
        }
        strcpy(buf, currpath);
        p = buf + strlen(buf);                          // let p point to the end of the path
        *p++ = '/';                                     // Add / to the path
        while (read(fd, &de, sizeof(de)) == sizeof(de)) // read current directory until finished
        {
            if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            {
                continue;
            }
            memmove(p, de.name, DIRSIZ); // Splice the file/directory name to the end of the buf.
            p[DIRSIZ] = '\0';
            find(buf, target);
        }
        close(fd);
        break;
    }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(2, "usage: find <directory> <target>\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}