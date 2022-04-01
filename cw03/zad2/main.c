#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


double f(double x)
{
    return 4. / (x*x + 1.);
}


void computations_to_file(char* filename, double delta_x, int computations_number, double beg)
{
    FILE* file = fopen(filename, "w+");

    double res = 0;
    int i;
    for (i = 0; i < computations_number; i++)
    {
        res += f(beg) * delta_x;
        beg += delta_x;
    }

    fprintf(file, "%.60f\n", res);
    fclose(file);
}


double sum_from_files(int n)
{
    char filename[20], file_res[70];
    FILE* file;
    double res = 0;

    int i;
    for(i = 0; i < n; i++)
    {
        sprintf(filename, "w%d.txt", i + 1);
        file = fopen(filename, "r");
        fgets(file_res, 70, file);
        fclose(file);

        res += atof(file_res);
    }

    return res;
}


int main(int argc, char** argv)
{
    system("rm -f w*.txt");

    if(argc != 3)
    {
        fprintf(stderr, "Incorrect number of args\n");
        return -1;
    }

    double delta_x = atof(argv[1]);
    int n = atoi(argv[2]);

    // delta >= 0.1 kinda has no point there
    if (n <= 0 || delta_x <= 0 || delta_x > 0.1)
    {
        fprintf(stderr, "Incorrect arguments\n");
        return -1;
    }

    char filename[20];

    int child_pid = -1, computations_all = ceil(1./delta_x);
    int computations_number = floor(computations_all / n);
    double beg = -computations_number * delta_x;

    int i;
    for(i = 0; i < n; i++)
    {
        // if we are in main process
        if (child_pid != 0)
        {
            beg += computations_number * delta_x;
            child_pid = fork();
            sprintf(filename, "w%d.txt", i + 1);
            // if it is the last process, assign it all computations left
            if (i == n - 1)
                computations_number += computations_all % n;
        }
    }


    // if main process
    if (child_pid != 0)
    {
        // wait for all child processes to end
        while ((wait(NULL)) > 0);
        // calculate final result
        printf("Computation result: %.10f\n", sum_from_files(n));
    }
    else // if child process
    {
//        printf("%f  %d\n", beg, computations_number);
        computations_to_file(filename, delta_x, computations_number, beg);
    }

    return 0;
}
