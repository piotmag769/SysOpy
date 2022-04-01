#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("Incorrect number of args\n");
        return 0;
    }

    int n = atoi(argv[1]);

    int i;
    int child_pid = -1;

    for(i = 0; i < n; i++)
    {
        // if we are in main process
        if (child_pid != 0)
            child_pid = fork();
    }

    // if not main process
    if (child_pid == 0)
        printf("PID: %d, %d\n", getpid(), child_pid);

    return 0;
}
