#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>


int sig1_caught = 0, essa = 0;
sigset_t mask;


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


void set_mask(int SIG1, int SIG2)
{
    sigfillset(&mask);

    sigdelset(&mask, SIG2);
    sigdelset(&mask, SIG1);
}


void sig1_handler(int sig_num, siginfo_t *info, void *ucontext)
{
    sig1_caught++;
    // idk what should I print there, task description not clear - will comment it for now
//    printf("Signal number %d (catcher numeration) received by sender\n", info->si_value.sival_int);
}


void sig2_handler(int sig_num, siginfo_t *info, void *ucontext)
{
    essa = 1;
}


void set_handler(int SIG1, int SIG2)
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
    if (argc != 4)
        return 1;

    pid_t catcher_pid = atoi(argv[1]);
    int signals_to_send = atoi(argv[2]);

    // KILL - 0, SIGQUEUE - 1, SIGRT - 2
    int mode = get_mode(argv[3]);

    if (mode == 3)
        return 1;


    int SIG1= (mode != 2) ? SIGUSR1 : SIGRTMIN;
    int SIG2 = (mode != 2) ? SIGUSR2 : SIGRTMAX;

    set_mask(SIG1, SIG2);

    set_handler(SIG1, SIG2);

    int i;
    switch(mode)
    {
        case 0:
        case 2:
            for(i = 0; i < signals_to_send; i++)
                kill(catcher_pid, SIG1);
            kill(catcher_pid, SIG2);
            break;
        case 1:
            ;
            union sigval value;
            for(i = 0; i < signals_to_send; i++)
                sigqueue(catcher_pid, SIG1, value);
            sigqueue(catcher_pid, SIG2, value);
            break;
    }

    // waiting for signals 1 and 2 from catcher
    while(essa == 0)
        sigsuspend(&mask);

    printf("SENDER: received ending signal...\n");

    printf("SENDER: received back %d out of %d signals (%.2f)%%\n", sig1_caught, signals_to_send,
           (float)(sig1_caught)/signals_to_send * 100.0);

    return 0;
}
