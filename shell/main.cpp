#include "functions.h"
#include <sys/time.h>                


int main(int argc, char const *argv[])
{
    struct timeval t1, t2;
    double elapsedTime;
    // start timer
    gettimeofday(&t1, NULL);
    
    HashTable HT=read_all_folders("./data/2013_camera_specs");
    
    // stop timer
    gettimeofday(&t2, NULL);
    // compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec);      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000000.0;   // us to ms
    printf("%f ms.\n", elapsedTime);
    printf("Read all json files\n");

    // start timer
    read_csv("./data/sigmod_large_labelled_dataset.csv",HT);
    gettimeofday(&t1, NULL);

    FILE *output_file= fopen("./output_file.txt","w");
    print_all(HT,output_file);
    fclose(output_file);
    delete HT;

    // stop timer
    gettimeofday(&t2, NULL);
    // compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec);      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000000.0;   // us to ms
    printf("%f ms.\n", elapsedTime);


    exit(0);
}
