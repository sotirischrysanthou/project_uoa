#include "functions.h"
#include <sys/time.h>                


int main(int argc, char const *argv[])
{
    struct timeval t1, t2;
    double elapsedTime;
    // start timer
    gettimeofday(&t1, NULL);
    
    HashTable HT, tf_idf;
    int count=read_all_folders("./data/2013_camera_specs", HT, tf_idf);
    
    // stop timer
    gettimeofday(&t2, NULL);
    // compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec);      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000000.0;   // us to ms
    printf("%f s. for json\n", elapsedTime);
    printf("Read all json files\n");

    set_Bow_or_TfIdf(HT, tf_idf, count, 0);

    gettimeofday(&t1, NULL);
    read_csv("./data/sigmod_medium_labelled_dataset.csv",HT);
    gettimeofday(&t2, NULL);
    // compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec);      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000000.0;   // us to ms
    printf("%f s. for csv\n", elapsedTime);
    // start timer

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
    printf("%f s. for print/delete\n", elapsedTime);


    exit(0);
}
