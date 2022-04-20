#include <stdio.h>
#include <string.h>

// https://www.youtube.com/watch?v=NMqiaZycb_s <- check this out

// PRESS CTRL+D TO END MAIL COMMAND AFTER CALLING ./main
int main(int argc, char** argv)
{
    if (argc == 2)
    {
        if(strcmp(argv[1], "nadawca") == 0) {

            FILE *output_of_mail = popen("mail", "r");

            FILE *input_of_sort = popen("sort -k3", "w");

            char buffer[100];
            while (fgets(buffer, 100, output_of_mail) != NULL)
                if(buffer[0] == ' '|| buffer[0] == '>')
                    fwrite(buffer, sizeof(char), strlen(buffer), input_of_sort);


            pclose(output_of_mail);
            pclose(input_of_sort);
        }
        else if(strcmp(argv[1], "data") == 0)
        {
            // already sorted by default XD
            FILE *output_of_mail = popen("mail", "r");
            char buffer[100];
            while (fgets(buffer, 100, output_of_mail) != NULL)
                if(buffer[0] == ' ' || buffer[0] == '>')
                    printf("%s", buffer);

            pclose(output_of_mail);
        }
        else
            return 1;
    }
    else if (argc == 4)
    {
        // mail, subject, body
        char command[300] = {};
        strcat(command, "mail -s \"");
        int index = strlen(command);
        strcat(command + index, argv[2]);
        index = strlen(command);
        strcat(command + index, "\" ");
        index = strlen(command);
        strcat(command + index, argv[1]);
        FILE *input_of_mail = popen(command, "w");
        fwrite(argv[3], sizeof(char), strlen(argv[3]), input_of_mail);
        pclose(input_of_mail);
    }
    else
        return 1;

    return 0;
}
