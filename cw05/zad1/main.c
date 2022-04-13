#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


int mode = 0;

// max 10 commands in component, each 4 args + command, each one 29 chars
struct component
{
    char name[50];
    char commands[10][5][30];
    int commands_count;
};


// to debug
void print_component(struct component component)
{
    printf("%s\n", component.name);
    for (int i = 0; i < component.commands_count; i++)
        for(int j = 0; j < 5; j++)
            printf("%d. %s %lu\n", j, component.commands[i][j], strlen(component.commands[i][j]));
}


int read_component_name(char* line, struct component components[], int component_index)
{
    int len = strlen(line), beg = 0, temp, end;

    while (beg < len && isspace(line[beg]))
        beg++;

    if (beg == len) {
        mode = 1;
        return 0; // whitespace line, now we can check for components connections
    }

    for (temp = beg; temp < len; temp++)
        if (line[temp] == '=')
            break;

    end = temp;

    while(isspace(line[--end]));

    components[component_index].name[0] = '\0'; // to trick zeroing everything xD
    strncat(components[component_index].name, line, end - beg + 1);
    components[component_index].commands_count = 0;

    return temp;
}


// beg is on '=' char
void read_subcommands(char* line, int beg, struct component components[], int component_index)
{
    int i;
    // in case it was a whitespace only line and mode changed
    if(mode == 0)
    {
        int end, real_end, arg;
        int len = strlen(line);
        int command_index = 0;
        while(beg < len)
        {
            arg = 0;
            while(isspace(line[++beg]));

            end = beg;

            while(line[end] !=  '|' && line[end] != '\n')
                end++;

            real_end = end - 1;

            while(isspace(line[real_end]))
                real_end--;

            char new_command[50] = {};

            strncat(new_command, line + beg, real_end - beg + 1);

            for(i = 0 ; i < 5; i++)
                components[component_index].commands[command_index][i][0] = '\0';

            char *s = strtok(new_command, " ");
            while(s)
            {
                components[component_index].commands[command_index][arg][0] = '\0'; // again
                strcat(components[component_index].commands[command_index][arg], s);
                arg++;
                s = strtok(NULL, " ");
            }

            command_index++;

            beg = end + 1;
        }
        components[component_index].commands_count = command_index;
    }
}


void execute_pipe(char* line, struct component components[], int num_of_comp)
{
    int comp_indexes[10];
    int index = 0, i;
    if(line[strlen(line) - 1] == '\n')
        line[strlen(line) - 1] = '\0';

    char* s = strtok(line, " | ");
    while(s)
    {
        for(i = 0; i < num_of_comp; i++)
            if(strcmp(components[i].name, s) == 0)
            {
                comp_indexes[index++] = i;
                break;
            }
        s = strtok(NULL, " | ");
    }


    // now we execute, finally XD
    // read - 0, write - 1
    int fd[2][2];
    pipe(fd[0]);

    int last = 0;
    for(int k = 0; k < index; k++)
    {
        for (i = 0; i < components[comp_indexes[k]].commands_count; i++)
        {
            close(fd[last][0]);
            close(fd[last][1]);

            pipe(fd[last]);
            pid_t child = fork();

            if (child == 0)
            {
//                printf("%s %s %s\n", components[comp_indexes[k]].commands[i][0], components[comp_indexes[k]].commands[i][1], components[comp_indexes[k]].commands[i][2]);
                // not last one
                if (i != components[comp_indexes[k]].commands_count - 1 || k != index - 1)
                {
                    close(fd[last][0]);  // we won't read from current pipe
                    dup2(fd[last][1], STDOUT_FILENO);  // redirect output of current pipe to stdout (or rather other way round)
                }

                // not first one
                if (i + k != 0) {
                    close(fd[(last + 1) % 2][1]);  // we won't write to the last pipe
                    dup2(fd[(last + 1) % 2][0], STDIN_FILENO);  // redirect output of the last pipe to stdin
                }

                char* args[5];
                for(int x = 0; x < 5; x++)
                {
                    if (components[comp_indexes[k]].commands[i][x][0] == '\0')
                        args[x] = (char*) NULL;
                    else
                        args[x] = components[comp_indexes[k]].commands[i][x];
                }

                execv(args[0], args);

                exit(EXIT_SUCCESS);
            }
            last = (last + 1) % 2;
        }
    }

    close(fd[1][0]);
    close(fd[1][1]);
    close(fd[0][0]);
    close(fd[0][1]);
    while(wait(NULL) != -1);
}


// EXACTLY ONE WHITESPACE ONLY LINE BETWEEN COMPONENTS DEFINITIONS AND THEIR CALLS IS NEEDED!!!
// no more blank lines allowed
// for some reason grep regular expressions don't work when passing them to execv XD
int main(int argc, char** argv)
{
    if (argc != 2)
        return 1;

    char* filename = argv[1];

    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        fprintf(stderr, "Unable to open the input file\n");
        return 0;
    }

    // max 499 chars in line, max 10 components
    char line[500] = {};
    struct component components[10];

    int beg, component_index = 0;
    while(mode == 0 && fgets(line, 500, file))
    {
        beg = read_component_name(line, components, component_index);
        read_subcommands(line, beg, components, component_index);
        if (mode == 0)
            component_index++;
    }

    char newlines[10][50];

    // bcs for some reason this file descriptor corrupts if we use fork XD
    int i = 0;
    while(fgets(newlines[i++], 500, file));

    fclose(file);

    int k;
    for(k = 0; k < i; k++)
        execute_pipe(newlines[k], components, component_index);

    return 0;
}
