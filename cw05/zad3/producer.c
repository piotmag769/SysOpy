#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// uncomment things to print echo
void write_to_fifo(int N, int line_num, FILE* file, int fifo)
{

    char* buffer = calloc(N + 1, sizeof(char));
    buffer[0] = line_num + '0';

    int N_read = fread(buffer + 1, sizeof(char), N, file);
//    int i;
    while(N_read != 0)
    {
//        for(i = 0; i < N_read + 1; i++)
//            printf("%c", buffer[i]);
//        printf("\n");

        sleep(1);

        write(fifo, buffer, sizeof(char)*(N_read + 1));
        N_read = fread(buffer + 1, sizeof(char), N, file);
    }

    free(buffer);
}


int main(int argc, char** argv)
{
    if(argc != 5)
        return 1;

    char* fifo_path = argv[1];
    int line_num = atoi(argv[2]);
    char* file_path = argv[3];
    int N = atoi(argv[4]);

    int fifo = open(fifo_path, O_WRONLY);
    FILE* file = fopen(file_path, "r");

    if(fifo == -1 || file == NULL)
        return 1;

//    printf("PRODUCER STARTED, N:%d\n", N);

    write_to_fifo(N, line_num, file, fifo);

    close(fifo);
    fclose(file);

    return 0;
}
