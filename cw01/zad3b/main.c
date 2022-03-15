#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>

#ifdef DLL
    #include <dlfcn.h>
#endif

#ifndef DLL
    #include "mylib.h"
#endif


int main(int argc, char** argv)
{
    #ifdef DLL
        void *handle = dlopen("./libmylib.so", RTLD_LAZY);
        char** (*create_table)(int) = dlsym(handle, "create_table");
        int (*wc_files)(char**, int, char[]) = dlsym(handle, "wc_files");
        void (*remove_block)(char**, int) = dlsym(handle, "remove_block");
    #endif

    #ifdef ADDING_AND_REMOVING
        struct tms* p_main_start = malloc(sizeof(struct tms));
        struct tms* p_main_end = malloc(sizeof(struct tms));
        clock_t main_start;
    #endif

    struct tms* p_start = malloc(sizeof(struct tms));
    struct tms* p_end = malloc(sizeof(struct tms));
    clock_t start_time;

    char** main_table;
    int main_size;

    if (argc < 3 || strcmp(argv[1], "create_table") != 0)
    {
        printf("You have to create table with pointers.\n");
        return 0;
    }

    main_size = atoi(argv[2]);
    main_table = create_table(main_size);

    // start measuring main
    #ifdef ADDING_AND_REMOVING
        main_start = times(p_main_start);
    #endif

    int i;
    for (i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "wc_files") == 0) {
            int number_of_files = atoi(argv[i + 1]);
            int args_size = 0;

            int index;
            // + 1 for spaces and null terminating character '\0'
            for (index = i + 2; index < i + 2 + number_of_files; index++)
                args_size += strlen(argv[index]) + 1;

            char* args = calloc(args_size, sizeof(char));

            int curr_index = 0;
            for (index = i + 2; index < i + 2 + number_of_files; index++)
            {
                strcpy(args + curr_index, argv[index]);
                curr_index += strlen(argv[index]) + 1;
                args[curr_index - 1] = ' ';
            }

            args[curr_index - 1] = '\0';

            wc_files(main_table, main_size, args);

            i += number_of_files + 1;
            free(args);
        }

        if (strcmp(argv[i], "remove_block") == 0)
        {
            i++;

            start_time = times(p_start);
            int block_size = strlen(main_table[atoi(argv[i])]);

            remove_block(main_table, atoi(argv[i]));

            printf("\nRemoving block of size %d bytes (time of remove_block function):\n\n", block_size);
            printf("real time: %f\n", (double) (times(p_end) - start_time)/CLOCKS_PER_SEC);
            printf("user time: %f\n", (double) (p_end->tms_utime - p_start->tms_utime)/sysconf(_SC_CLK_TCK));
            printf("system time: %f\n", (double) (p_end->tms_stime - p_start->tms_stime)/sysconf(_SC_CLK_TCK));
        }
    }

    #ifdef ADDING_AND_REMOVING
        printf("\nMain execution time (\"Na przemian  kilkakrotne dodanie i usunięcie zadanej liczby bloków\"):\n\n");
        printf("real time: %f\n", (double) (times(p_main_end) - main_start)/CLOCKS_PER_SEC);
        printf("user time: %f\n", (double) (p_main_end->tms_utime - p_main_start->tms_utime)/sysconf(_SC_CLK_TCK));
        printf("system time: %f\n", (double) (p_main_end->tms_stime - p_main_start->tms_stime)/sysconf(_SC_CLK_TCK));
        free(p_start);
        free(p_end);
    #endif

    free(main_table);

    #ifdef DLL
        dlclose(handle);
    #endif

    return 0;
}
