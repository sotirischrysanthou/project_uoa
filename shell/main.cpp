#include "functions.h"


int main(int argc, char const *argv[])
{

    item a("a", 10,NULL);
    item b("b", 20,NULL);
    item c("c", 30,NULL);
    item d("d", 40,NULL);
    foo(a.get_common_list(), b.get_common_list());
    // foo(c.get_common_list(), d.get_common_list());
    printf("\n\na\n");
    print_list(a.get_common_list());
    printf("\nb\n");
    print_list(b.get_common_list());
    printf("\nc\n");
    print_list(c.get_common_list());
    printf("\nd\n");
    print_list(d.get_common_list());

    foo(a.get_common_list(), c.get_common_list());

    printf("\n\na\n");
    print_list(a.get_common_list());
    printf("\nb\n");
    print_list(b.get_common_list());
    printf("\nc\n");
    print_list(c.get_common_list());
    printf("\nd\n");
    print_list(d.get_common_list());

    printf("\n\n");
    List visited_lists = new list(NULL);
    print_commons(visited_lists, a.get_common_list(), stdout);
    printf("\n\n");
    print_commons(visited_lists, b.get_common_list(), stdout);
    printf("\n\n");
    print_commons(visited_lists, c.get_common_list(), stdout);
    printf("\n\n");
    print_commons(visited_lists, d.get_common_list(), stdout);

    HashTable HT=read_all_folders("./data/2013_camera_specs");
    printf("Read all json files\n");
    read_csv("./data/sigmod_medium_labelled_dataset.csv",HT);
    FILE *output_file= fopen("./output_file.txt","w");
    print_all(HT,output_file);
    fclose(output_file);
    delete HT;
    return 0;
}
