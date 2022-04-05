#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>


int SIG1, SIG2;
int sig1_caught = 0, essa = 0, mode;
sigset_t mask;
pid_t sender_pid;


int get_mode(char* mode_string)
{
    if(strcmp("KILL", mode_string) == 0)
        return 0;
    if(strcmp("SIGQUEUE", mode_string) == 0)
        return 1;
    if(strcmp("SIGRT", mode_string) == 0)
        return 2;
    return 3;
}


void set_mask()
{
    sigfillset(&mask);

    sigdelset(&mask, SIG2);
    sigdelset(&mask, SIG1);
}


void sig1_handler(int sig_num, siginfo_t *info, void *ucontext)
{
    sig1_caught++;
    sender_pid = info->si_pid;
    if (mode != 1)
        kill(sender_pid, SIG1);
    else
    {
        union sigval value;
        value.sival_int = sig1_caught;
        sigqueue(sender_pid, SIG1, value);
    }
}


void sig2_handler(int sig_num, siginfo_t *info, void *ucontext)
{
    sender_pid = info->si_pid;
    essa = 1;
    if (mode != 1)
        kill(sender_pid, SIG2);
    else
    {
        union sigval value;
        value.sival_int = sig1_caught;
        sigqueue(sender_pid, SIG2, value);
    }
}


void set_handler()
{
    struct sigaction act1;
    act1.sa_flags = SA_SIGINFO;
    act1.sa_sigaction = sig1_handler;
    sigemptyset(&act1.sa_mask);

    if (sigaction(SIG1, &act1, NULL) == -1)
        printf("Unexpected error");

    struct sigaction act2;
    act2.sa_flags = SA_SIGINFO;
    act2.sa_sigaction = sig2_handler;
    sigemptyset(&act2.sa_mask);

    if (sigaction(SIG2, &act2, NULL) == -1)
        printf("Unexpected error");
}


int main(int argc, char** argv)
{
    // one arg to know what signals to block
    if (argc != 2)
        return 1;

    mode = get_mode(argv[1]);

    if(mode == 3)
        return 1;

    printf("%d\n", getpid());

    SIG1 = (mode != 2) ? SIGUSR1 : SIGRTMIN;
    SIG2 = (mode != 2) ? SIGUSR2 : SIGRTMAX;

    set_mask();

    set_handler();

    // waiting for signals 1 and 2
    while(essa == 0)
        sigsuspend(&mask);


    printf("CATCHER: received ending signal...\n");

    printf("CATCHER: received %d signals. Ending process\n", sig1_caught);

    return 0;
}
