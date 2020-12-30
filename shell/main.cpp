#include "functions.h"
#include <sys/time.h>
#include <assert.h>

/* to do train or test or both */

/* usage:   ./exe input_folder(full) csv_file(full) "bow or tfidf"(default=tfidf) "train or test or both"(default=both) output_file(default=stdout) */
/*          ./exe ./data/2013_camera_specs ./data/sigmod_large_labelled_dataset.csv tfidf both output_file.txt */
int main(int argc, char const *argv[])
{
    string input_folder = argv[1];
    string csv_file = argv[2];
    bool bow_tfidf = 1;
    if (argc > 3)
        if (strcmp(argv[3], "bow") == 0)
            bow_tfidf = 0;

    struct timeval t1, t2;
    double elapsedTime;

    HashTable HT = new hashtable(10, del_main_ht, hashfunction);
    HashTable tf_idf = new hashtable(500, NULL, hashfunction);

    gettimeofday(&t1, NULL); // start timer

    int count = read_all_folders(input_folder, HT, tf_idf);

    assert(HT != NULL);
    gettimeofday(&t2, NULL);                              // stop timer
    elapsedTime = (t2.tv_sec - t1.tv_sec);                // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000000.0; // us to ms
    printf("%f s. for json\n", elapsedTime);

    // printf("Read all json files\n");

    gettimeofday(&t1, NULL); // start timer

    set_Bow_or_TfIdf(HT, tf_idf, count, bow_tfidf);

    gettimeofday(&t2, NULL);                              // stop timer
    elapsedTime = (t2.tv_sec - t1.tv_sec);                // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000000.0; // us to ms
    printf("%f s. for BoW and Tf-Idf\n", elapsedTime);

    gettimeofday(&t1, NULL); // start timer

    read_csv(csv_file, HT);

    gettimeofday(&t2, NULL);                              // stop timer
    elapsedTime = (t2.tv_sec - t1.tv_sec);                // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000000.0; // us to ms
    printf("%f s. for csv\n", elapsedTime);

    /* split csv */
    string line;
    int file_lines = lines_counter(csv_file.c_str());
    printf("csv file lines: %d\n", file_lines);
    int counter = 0;
    size_t buffer_size = 0;
    char *buffer = NULL;
    FILE *stream = fopen(csv_file.c_str(), "r");
    FILE *train_f = fopen("train.csv", "w");
    FILE *test_f = fopen("test.csv", "w");

    if (!stream)
    {
        printf("Error reading file: %s\n", csv_file.c_str());
        return 1;
    }
    getline(&buffer, &buffer_size, stream);
    line = buffer;
    fprintf(train_f, "%s", line.c_str());
    fprintf(test_f, "%s", line.c_str());
    while ((getline(&buffer, &buffer_size, stream) != -1))
    {
        line = buffer;
        if (counter < (file_lines * 8 / 10))
            fprintf(train_f, "%s", line.c_str());
        else
            fprintf(test_f, "%s", line.c_str());
        counter++;
    }

    fclose(train_f);
    fclose(test_f);
    fclose(stream);

    bool train_flag = 1, test_flag = 1;
    if (argc > 4)
    {
        if (strcmp(argv[4], "train") == 0)
        {
            test_flag = 0;
        }
        else if (strcmp(argv[4], "test") == 0)
        {
            train_flag = 0;
        }
    }
    double *b;
    if (train_flag)
    {
        gettimeofday(&t1, NULL); // start timer

        b = train("train.csv", HT, tf_idf, 5);
        /*  for medium: ~1.30m
            for large:  ~9m      */

        gettimeofday(&t2, NULL);                              // stop timer
        elapsedTime = (t2.tv_sec - t1.tv_sec);                // sec to ms
        elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000000.0; // us to ms
        printf("%f s. for train\n", elapsedTime);
    }
    else
    {
        b=new double[tf_idf->ht_size() + 1];
        stream = fopen("./best_W", "r");
        if (!stream)
        {
            printf("Error reading file: ./best_W\n");
            return 1;
        }
        buffer_size = 0;
        buffer = NULL;
        counter = 0;
        while ((getline(&buffer, &buffer_size, stream) != -1))
        {
            line = buffer;
            b[counter]=stod(line);
            counter++;
        }
    }

    if (test_flag)
    {
        gettimeofday(&t1, NULL); // start timer

        printf("----------test----------\n");

        test("train.csv", HT, b, tf_idf->ht_size());

        printf("------------------------\n");

        gettimeofday(&t2, NULL);                              // stop timer
        elapsedTime = (t2.tv_sec - t1.tv_sec);                // sec to ms
        elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000000.0; // us to ms
        printf("%f s. for validation/test\n", elapsedTime);
        gettimeofday(&t1, NULL); // start timer
    }

    delete b;

    if (argc > 5)
    {
        FILE *output_file = fopen(argv[5], "w");
        print_all(HT, output_file);
        fclose(output_file);
    }
    else
        print_all(HT);

    delete HT;
    delete tf_idf;

    gettimeofday(&t2, NULL);                              // stop timer
    elapsedTime = (t2.tv_sec - t1.tv_sec);                // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000000.0; // us to ms
    printf("%f s. for print/delete\n", elapsedTime);

    exit(0);
}
