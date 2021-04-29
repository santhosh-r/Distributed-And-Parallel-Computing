#include<stdio.h>
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
//prototype
double getRand(double min, double max);

int main(int argc, char*argv[])
{
    if(argc<2)
    {
        perror("\nUsage ./generateInput <size>\n");
        exit(-1);
    }
    FILE *fp;
    int mat_size = atoi(argv[1]), i, j;
    double min=0.00001, max=100000;
    char comma[2];
    fp = fopen("input.txt", "w");
    srand(time(NULL));
    // fprintf(fp, "%d\n", mat_size);
    for(i=0;i<mat_size;i++)
    {
        // sprintf(comma, "%s", "");
        for(j=0;j<mat_size;j++)
        {
            fprintf(fp, "%f ",getRand(min, max));
            // sprintf(comma, "%s", ",");;
        }
        // fprintf(fp, " ");
    }

    fclose(fp);

}


double getRand(double min, double max)
{
    double d = (double)rand() / RAND_MAX;
    return min + d * (max - min);
}
