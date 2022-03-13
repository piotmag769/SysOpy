#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


/**
 Function creating table of char**.
 @param size - number of allocated table elements
 @return Pointer to table of char**.
*/
char** create_table(int size)
{
    char** res = calloc(size, sizeof(char*));
    if (res == NULL)
        fprintf(stderr, "\nUnable to allocate table with pointers - returning NULL. Errno was %i\n", errno);
    return res;
}


/**
 Function executing wc command with -lwc options and argument args.
 It allocates table of characters to store output of above command.
 Then it assigns pointer to the memory block to first main_table element that was NULL.
 The function uses temporary file created with mkstemp function as a buffer for command line output.
 @param main_table - table of char** to which pointer to allocated memory block should be put
 @param main_size - number of elements of main_table
 @param args - arguments for wc command (usually paths to files separated with spaces)
 @return Index of main_table element to which pointer to allocated memory was assigned to. In case of any error -1 is returned.
*/
int wc_files(char** main_table, int main_size, char args[])
{
    if(main_table == NULL)
    {
        fprintf(stderr, "\nMain table is NULL.\n");
        return -1;
    }

    int iterator;
    int index = -1;
    for(iterator = 0; iterator < main_size; iterator++)
        if (main_table[iterator] == NULL)
        {
            index = iterator;
            break;
        }

    if(index == -1)
    {
        fprintf(stderr, "\nTable with pointers is full - create a new one.\n");
        return -1;
    }

    // name of temporary file
    char filename[] = "tempXXXXXX";
    // creating temporary file in working directory
    int temp = mkstemp(filename);

    if (temp == -1)
    {
        fprintf(stderr, "\nTemporary file could not be created. Errno was %i\n", errno);
        return -1;
    }

    char t1[] = "wc -lwc ";
    char t3[] = " > ";

    // strlen(t1) for "wc -lwr ", strlen(args) for args arguments, strlen(t3) for " > "
    // strlen(filename) for redirecting to temporary args
    int lengths[4] = {strlen(t1), strlen(args), strlen(t3), strlen(filename)};

    // 2 for '\n' and null terminating character '\0'
    size_t sys_arg_len = 2;

    for(iterator = 0; iterator < 5; iterator++)
        sys_arg_len += lengths[iterator];

    char* sys_arg = calloc(sys_arg_len, sizeof(char));

    if (sys_arg == NULL)
    {
        fprintf(stderr, "\nUnable to allocate table with command line command. Errno was %i\n", errno);
        remove(filename);
        return -1;
    }

    strcpy(sys_arg, t1);
    strcpy(sys_arg + strlen(t1), args);
    strcpy(sys_arg + strlen(t1) + strlen(args), t3);
    strcpy(sys_arg + lengths[0] + lengths[1] + lengths[2], filename);
    sys_arg[lengths[0] + lengths[1] + lengths[2] + lengths[3]] = '\n';
    sys_arg[lengths[0] + lengths[1] + lengths[2] + lengths[3] + 1] = '\0';

    int shell_return = system(sys_arg);

    if (shell_return == -1)
    {
        fprintf(stderr, "\nCommand line error occurred while executing command: %s Errno was %i\n", sys_arg, errno);
        free(sys_arg);
        remove(filename);
        return -1;
    }

    if (shell_return == 127)
    {
        fprintf(stderr, "\nThe following command does not exist: %s Make sure you are using unix system. Errno was %i\n", sys_arg, errno);
        free(sys_arg);
        remove(filename);
        return -1;
    }

    free(sys_arg);

    /** BŁĘDY MOŻE IDK */
    // getting size of temporary file (therefore size of wc output)
    int size = lseek(temp, 0, SEEK_END); // +1 not needed I think, don't want EOF to be in string

    // going back to beginning of the file
    lseek(temp, 0, SEEK_SET);

    // + 1 for null terminating character '\0'
    main_table[index] = calloc(size + 1, 1);

    if (main_table[index] == NULL)
    {
        fprintf(stderr, "\nUnable to allocate new memory block. Errno was %i\n", errno);
        remove(filename);
        return -1;
    }

    if (read(temp, main_table[index], size) == -1)
    {
        fprintf(stderr, "\nUnable to read from temporary file.Errno was %i\n", errno);
        remove(filename);
        return -1;
    }

    remove(filename);

    return index;
}


/**
 Function removing allocated memory that main_table[index] points too.
 Function sets main_table[index] to NULL.
 @param main_table - table with pointers to allocated memory
 @param index - index of the main_table element that points to memory that is supposed to be freed
*/
void remove_block(char** main_table, int index)
{
    free(main_table[index]);
    main_table[index] = NULL;
}
