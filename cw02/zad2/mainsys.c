#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char** argv)
{
    char chr;
    char *filename = NULL;

    if (argc < 2 || strlen(argv[1]) != 1)
    {
        fprintf(stderr, "Give arguments next time\n");
        return -1;
    }
    else
    {
        chr = *(argv[1]);
        filename = argv[2];
    }

//    printf("%c\n%s", chr, filename);

    int file = open(filename, O_RDONLY);

    if (file == -1)
    {
        fprintf(stderr, "Couldn't open file to read from. Errno output: %s", strerror(errno));
        return -1;
    }

    int size = lseek(file, 0, SEEK_END); // +1 not needed I think, don't want EOF to be in string

    // going back to beginning of the file
    lseek(file, 0, SEEK_SET);

    // + 1 for null terminating character '\0'
    char* buffer = calloc(size + 1, sizeof(char));

    read(file, buffer, sizeof(char)*size);

//    printf("%s", buffer);
//    printf("%ld", strlen(buffer));

    int chr_counter = 0;
    int lines_counter = 0;
    int line_had_chr = 0;
    for(int i = 0; i < size; i++)
    {
//        printf("%d\n", buffer[i]);
        if (buffer[i] == '\n')
        {
            if(line_had_chr)
                lines_counter++;

            line_had_chr = 0;
        }
        else if(buffer[i] == chr)
        {
            chr_counter++;
            line_had_chr = 1;
        }
    }

    // EOF reached, count last line if needed
    if(line_had_chr)
        lines_counter++;

    printf("\nLines with \"%c\": %d\nNumber of \"%c\" appearances: %d\n\n", chr, lines_counter, chr, chr_counter);
    free(buffer);
    return 0;
}
