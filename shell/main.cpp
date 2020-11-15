#include "functions.h"


int main(int argc, char const *argv[])
{
    HashTable HT=read_all_folders("./data/2013_camera_specs");
    printf("Read all json files\n");
    read_csv("./data/sigmod_medium_labelled_dataset.csv",HT);
    FILE *output_file= fopen("./output_file.txt","w");
    print_all(HT,output_file);
    fclose(output_file);
    delete HT;
    return 0;
}
