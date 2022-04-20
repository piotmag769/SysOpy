#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>


int main(int argc, char** argv)
{
    if(argc != 4)
        return 1;

    int N = atoi(argv[1]);
    int producers = atoi(argv[2]);
    int consumers = atoi(argv[3]);

    // clearing output file
    fclose(fopen("o.txt", "w"));

    mkfifo("FIFO", 0666);

    char line_num[4] = {};
    char* file_name[12] = {};
    char N_char[10] = {};
    sprintf(N_char, "%d", N);

    int i;
    for(i = 1; i < producers + 1; i++)
    {
        file_name[i] = calloc(15, sizeof(char));
        sprintf(file_name[i], "N=%d/i%d.txt", N, i);
    }

    file_name[0] = calloc(15, sizeof(char));
    sprintf(file_name[0], "./script.sh");

    file_name[producers + 1] = calloc(15, sizeof(char));
    sprintf(file_name[producers + 1], "o.txt");


    pid_t pid;
    for(i = 1; i < producers + 1; i++)
    {
        pid = fork();
        if (pid == 0) {
            sprintf(line_num, "%d", i);
            execl("./producer", "./producer", "FIFO", line_num, file_name[i], N_char, (char *) NULL);
        }
    }


    for(i = 0; i < consumers; i++)
    {
        pid = fork();
        if (pid == 0)
            execl("./consumer", "./consumer", "FIFO", "o.txt", N_char, (char *) NULL);
    }

    while(wait(NULL) != -1);

    pid = fork();
    if(pid == 0)
        execl("/usr/bin/rm", "/usr/bin/rm", "FIFO", (char*) NULL);

    pid = fork();
    if (pid == 0)
        execv("./script.sh", file_name);

    while(wait(NULL) != -1);

    for(i = 1; i < producers + 1; i++)
        free(file_name[i]);

    printf("DONE\n");

    return 0;
}
