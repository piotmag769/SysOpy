#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"

int main()
{
    int main_size = 5;
    char** main_table = create_table(main_size);

    wc_files(main_table, main_size, "elo.txt");
    wc_files(main_table, main_size, "essa.txt");
    wc_files(main_table, main_size, "owo.txt");
    wc_files(main_table, main_size, "uwu.txt");
    wc_files(main_table, main_size, "elo.txt essa.txt owo.txt uwu.txt");

    int i;
    for(i = 0; i < main_size; i++) {
        printf("%s", main_table[i]);
        remove_block(main_table, i);
    }

    free(main_table);

    return 0;
}
