#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

// #define __USE_XOPEN_EXTENDED

// this function should not be called if SA_NOCLDSTOP is set - works
void handler()
{
    printf("Child process ended/stopped...\n");
}


void info_handler(int sig, siginfo_t *info, void *ucontext)
{
    printf("Signal number: %d\nSending process PI: %d\nErrno value: %d\n", info->si_signo, info->si_pid, info->si_errno);
    printf("Signal code: %d\nExit value or signal: %d\n", info->si_code, info->si_status);
}


int main(int argc, char** argv)
{
    if (argc != 2)
        return -1;

    struct sigaction act;
    int option, child;

    // if no SA_SIGINFO
    act.sa_handler = handler;

    if(strcmp(argv[1], "SA_SIGINFO") == 0)
    {
        act.sa_flags = SA_SIGINFO;
        option = 0;
        act.sa_sigaction = info_handler;
    }
    else if(strcmp(argv[1], "SA_NOCLDSTOP") == 0)
    {
        act.sa_flags = SA_NOCLDSTOP;
        option = 1;
        printf("Stopping then killing child process, handler should be invoked only once\n\n");
    }
    else if(strcmp(argv[1], "SA_NOCLDWAIT") == 0)
    {
        act.sa_flags = SA_NOCLDWAIT;
        option = 2;
    }
        else
        return -1;

    if (option != 0)
        sigaction(SIGCHLD, &act, NULL);
    else
        sigaction(SIGUSR1, &act, NULL);


    if (option != 0) // SA_NOCLDWAIT, SA_NOCLDSTOP
    {
        child = fork();

        // try sending SIGCHLD
        if (child == 0)
            raise(SIGSTOP);

        // in main process try to wait for child process in zombie state - checking SA_NOCLDWAIT
        if (option == 2 && child != 0 && (waitpid(child, NULL, 0)) == -1)
            printf("Child process not in zombie state - works!\n\n");
        else if (option == 1 && child != 0)
        {
            sigset_t set;
            sigpending(&set);
            // https://www.gnu.org/software/libc/manual/html_node/Checking-for-Pending-Signals.html
            if (sigismember(&set, SIGCHLD))
                printf("THAT SHOULD NOT HAPPEN!\n");
            kill(child, SIGKILL);
            wait(NULL);
        }
    }
    else
    {
        raise(SIGUSR1);
    }

    return 0;
}
