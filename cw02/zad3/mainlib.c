#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

int regular_files = 0, dirs = 0, char_devs = 0, block_devs = 0, fifos = 0, slinks = 0, sockets = 0;

int print_stats(const char *path, const struct stat *stat, int flag, struct FTW *fwt)
{
    // zwykły plik - file, katalog - dir, urządzenie znakowe - char dev, urządzenie blokowe - block dev,
    // potok nazwany - fifo, link symboliczny - slink, soket - sock
    char abs_path[500] = {};
    char date[51] = {};
    realpath(path, abs_path);
    printf("\n%s\nLink count: %lu\n", abs_path, stat->st_nlink);

    // too lazy to write all if else's... so just copied printf content
    S_ISDIR(stat->st_mode) ? dirs++ : S_ISREG(stat->st_mode) ? regular_files++ :
    S_ISCHR(stat->st_mode) ? char_devs++ : S_ISBLK(stat->st_mode) ? block_devs++ :
    S_ISFIFO(stat->st_mode) ? fifos++ : S_ISLNK(stat->st_mode) ? slinks++ :
    S_ISSOCK(stat->st_mode) ? sockets++ : 0;

    printf("%s\n", S_ISDIR(stat->st_mode) ? "dir" : S_ISREG(stat->st_mode) ? "file" :
           S_ISCHR(stat->st_mode) ? "char dev" : S_ISBLK(stat->st_mode) ? "block dev" :
           S_ISFIFO(stat->st_mode) ? "fifo" : S_ISLNK(stat->st_mode) ? "slink" :
           S_ISSOCK(stat->st_mode) ? "socket" : "file type not specified in task description");

    printf("Size of file (bytes): %ld\n", stat->st_size);

    // https://stackoverflow.com/questions/10446526/get-last-modified-time-of-file-in-linux
    strftime(date, 50, "%c", localtime(&(stat->st_atime)));
    printf("Last access time: %s\n", date);
    strftime(date, 50, "%c", localtime(&(stat->st_mtime)));
    printf("Last modification time: %s\n", date);
    return 0;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Give arguments next time\n");
        return -1;
    }

    nftw(argv[1], print_stats, 10, FTW_PHYS);

    printf("\nRegular files: %d", regular_files);
    printf("\nDirectories: %d", dirs);
    printf("\nCharacter devices: %d", char_devs);
    printf("\nBlock devices: %d", block_devs);
    printf("\nFIFOs: %d", fifos);
    printf("\nSymlinks: %d", slinks);
    printf("\nSockets: %d\n", sockets);
    return 0;
}

