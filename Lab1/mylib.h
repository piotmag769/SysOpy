#ifndef _MYLIB_H_
#define _MYLIB_H_

char** create_table(int size);

int wc_files(char** main_table, int main_size, char args[]);

void remove_block(char** my_table, int index);

#endif // _MYLIB_H_
