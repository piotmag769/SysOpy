#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/file.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define MAX_PRODUCERS 10
#define MAX_BYTES_IN_LINE 6000

// uncomment things to print echo
void read_from_fifo(int N, int file, int fifo)
{
    int i, line_num = 0, max_line = 0;

    char buffer[MAX_PRODUCERS][MAX_BYTES_IN_LINE] = {};

    int uwu = read(fifo, &line_num, sizeof(char));

    if(uwu == 0)
        return;

    line_num -= 48;
    max_line = MAX(line_num, max_line);
    int N_read = read(fifo, buffer[line_num - 1], N*sizeof(char));


    while(N_read != 0)
    {
//        printf("CONSUMER:\n%d%s\n", line_num, buffer[line_num - 1]);
        uwu = read(fifo, &line_num, sizeof(char));

        if(uwu == 0)
            break;

        line_num -= 48;
        max_line = MAX(line_num, max_line);
        N_read = read(fifo, buffer[line_num - 1] + strlen(buffer[line_num - 1]), N*sizeof(char));
    }

    // we can write later because producer consumer problem is all about around shared buffer, not later resource usage
    for(i = 0; i < max_line; i++)
    {
        flock(file, LOCK_EX);
        write(file, buffer[i], sizeof(char) * strlen(buffer[i]));
        write(file, "\n", sizeof(char));
        flock(file, LOCK_SH);
    }
}


int main(int argc, char** argv)
{
    if(argc != 4)
        return 1;

    char* fifo_path = argv[1];
    char* file_path = argv[2];
    int N = atoi(argv[3]);

    int fifo = open(fifo_path, O_RDONLY);
    int file = open(file_path, O_APPEND | O_WRONLY);

    if(fifo == -1 || file == -1)
        return 1;

//    printf("CONSUMER STARTED\n");

    read_from_fifo(N, file, fifo);

    close(fifo);
    close(file);

    return 0;
}
