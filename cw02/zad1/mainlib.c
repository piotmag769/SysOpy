#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>


int main(int argc, char** argv)
{
    int free_memory = 0;
    char *filename1 = NULL;
    char *filename2 = NULL;

    if (argc < 3)
    {
        free_memory = 1;
        printf("Give input filename, press ENTER, then output filename\n");
        filename1 = calloc(100, sizeof(char));
        filename2 = calloc(100, sizeof(char));

        scanf("%s", filename1);
        scanf("%s", filename2);
    }
    else
    {
        filename1 = argv[1];
        filename2 = argv[2];
    }

//    printf("%s\n%s", filename1, filename2);

    FILE* file1 = fopen(filename1, "r");
    FILE* file2 = fopen(filename2, "w");

    if (file1 == NULL || file2 == NULL)
    {
        fprintf(stderr, "Couldn't open file to read from/write to. Errno output: %s", strerror(errno));

        if(free_memory)
        {
            free(filename1);
            free(filename2);
        }
        return -1;
    }

    fseek(file1, 0, SEEK_END); // +1 not needed I think, don't want EOF to be in string

    int size = ftell(file1);
    // going back to beginning of the file
    fseek(file1, 0, SEEK_SET);

    // + 1 for null terminating character '\0'
    char* buffer = calloc(size + 1, sizeof(char));

    fread(buffer, sizeof(char), size, file1);

//    printf("%s", buffer);
//    printf("%ld", strlen(buffer));

    int beg = 0;
    int was_white_space = 1;
    for(int i = 0; i < size; i++)
    {
//        printf("%d\n", buffer[i]);
        if (buffer[i] == '\n')
        {
            if(!was_white_space)
                fwrite(buffer + beg, sizeof(char), i - beg + 1, file2);
            beg = i + 1;
            was_white_space = 1;
        }
        else if(!isspace(buffer[i]))
            was_white_space = 0;
    }

    // EOF reached, write last line if not whitespace only
    if(!was_white_space)
        fwrite(buffer + beg, sizeof(char), size - beg, file2);

    if (free_memory)
    {
        free(filename1);
        free(filename2);
    }

    free(buffer);
    return 0;
}
