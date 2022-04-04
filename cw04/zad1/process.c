#include <stdio.h>
#include <string.h>
#include <signal.h>


int main(int argc, char** argv)
{
    int pending = 0;
    if(strcmp(argv[1], "pending") == 0)
        pending = 1;

    printf("Child exe process:\n\n");
    if (!pending)
        raise(SIGUSR1);
    else
    {
        sigset_t set;
        sigpending(&set);
        // https://www.gnu.org/software/libc/manual/html_node/Checking-for-Pending-Signals.html
        if (sigismember(&set, SIGUSR1))
            printf("Signal pending there - good job!\n");
    }

    return 0;
}
