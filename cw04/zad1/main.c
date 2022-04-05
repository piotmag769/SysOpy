#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>


void handler(int sid)
{
    printf("Signal handled, essa\n\n");
}


int main(int argc, char** argv)
{
    if (argc != 3)
        return -1;

    int pending = 0;

    if (strcmp(argv[1], "ignore") == 0)
        signal(SIGUSR1, SIG_IGN);
    else if (strcmp(argv[1], "handler") == 0)
        signal(SIGUSR1, handler);
    else if (strcmp(argv[1], "mask") == 0 || strcmp(argv[1], "pending") == 0)
    {
        sigset_t new_mask;
        sigemptyset(&new_mask);
        sigaddset(&new_mask, SIGUSR1);
        sigprocmask(SIG_BLOCK, &new_mask, NULL);
    }
    else
        return -1;

    if(strcmp(argv[1], "pending") == 0)
        pending = 1;


    raise(SIGUSR1);


    if ((strcmp(argv[2], "fork") == 0))
    {
        int child = fork();
        if (child == 0) {
            printf("Child fork process:\n\n");
            if (!pending)
                raise(SIGUSR1);
            else {
                sigset_t set;
                sigpending(&set);
                // https://www.gnu.org/software/libc/manual/html_node/Checking-for-Pending-Signals.html
                if (sigismember(&set, SIGUSR1))
                    printf("Signal pending there - good job!\n");
            }
        }
    }
    else if ((strcmp(argv[2], "exec") == 0))
    {
        execl("./process", "", argv[1], NULL);
    }
    else
        return -1;

    return 0;
}
