#include <assert.h>
#include <sys/time.h>
#include <math.h>
#include "functions.h"
#include "jobScheduler.h"

void similar_items(item *A, item *B)
{
    if (A->get_common_and_uncommon().common == B->get_common_and_uncommon().common)
        return;
    if (A->get_common_and_uncommon().uncommon->ht_size() < B->get_common_and_uncommon().uncommon->ht_size())
    {
        item *tmp = A;
        A = B;
        B = tmp;
    }
    List common_a = A->get_common_and_uncommon().common;
    List common_b = B->get_common_and_uncommon().common;
    HashTable uncommon_a = A->get_common_and_uncommon().uncommon;
    HashTable uncommon_b = B->get_common_and_uncommon().uncommon;

    List tempList = uncommon_b->return_list();
    ListNode tempNode;

    tempNode = tempList->list_first();
    while (tempNode != NULL)
    {
        if (uncommon_a->search(tempNode->value, cmp_hashtable_search_address))
        {
            // ((item*)((((List)(tempNode->value))->list_first())->value))->get_common_and_uncommon().uncommon->remove(common_b);
            List t_L = (List)(tempNode->value);
            ListNode t_LN = t_L->list_first();
            item *t_i = (item *)t_LN->value;
            HashTable t_ht = t_i->get_common_and_uncommon().uncommon;
            assert(t_ht != NULL);
            // printf("removing...\n");
            int test_int = t_ht->ht_size();
            t_ht->remove(common_b);
            assert(t_ht->ht_size() == test_int - 1);
        }
        else
        {
            HashTable_Node ht_n = new hashtable_node;
            ht_n->key = tempNode->value;
            ht_n->value = tempNode->value;
            uncommon_a->insert(ht_n);

            List t_L = (List)(tempNode->value);
            ListNode t_LN = t_L->list_first();
            item *t_i = (item *)t_LN->value;
            HashTable t_ht = t_i->get_common_and_uncommon().uncommon;
            assert(t_ht != NULL);
            t_ht->remove(common_b);
            ht_n = new hashtable_node;
            ht_n->key = common_a;
            ht_n->value = common_a;
            t_ht->insert(ht_n);
        }

        tempNode = tempNode->next;
    }
    tempNode = common_b->list_first();
    while (tempNode != NULL)
    {
        item *tempItem = (item *)common_b->list_node_value(tempNode);
        tempItem->change_common_list(common_a);
        tempItem->change_uncommon_ht(uncommon_a);
        common_a->list_insert_next(common_a->list_last(), tempItem);
        tempNode = common_b->list_next(tempNode);
    }
    delete tempList;
}

void dissimilar_items(item *a, item *b)
{

    if (a->get_common_and_uncommon().uncommon->search(b->get_common_and_uncommon().common, cmp_hashtable_search_address))
    {
        return;
    }
    HashTable_Node ht_n;

    ht_n = new hashtable_node;
    ht_n->key = b->get_common_and_uncommon().common;
    ht_n->value = b->get_common_and_uncommon().common;
    a->get_common_and_uncommon().uncommon->insert(ht_n);

    ht_n = new hashtable_node;
    ht_n->key = a->get_common_and_uncommon().common;
    ht_n->value = a->get_common_and_uncommon().common;
    b->get_common_and_uncommon().uncommon->insert(ht_n);
}

void print_list(List list_to_print)
{
    printf("%p\n", list_to_print);
    item *tempItem;
    ListNode tempNode = list_to_print->list_first();
    do
    {
        tempItem = (item *)list_to_print->list_node_value(tempNode);
        printf("%s->", tempItem->get_item_full_id().c_str());
        tempNode = tempNode->next;
    } while (tempNode != NULL);
    printf("NULL\n");
}

int list_cmp(Pointer l1, Pointer l2)
{
    if (l1 == l2)
        return 0;
    else
        return 1;
}

void print_uncommon(HashTable visited_lists, item *it, FILE *output_file)
{
    /* outer List vars */
    List list_of_uncommon_lists = it->get_common_and_uncommon().uncommon->return_list();
    ListNode Lnode = list_of_uncommon_lists->list_first();

    /* inner List vars */
    List uncommon_list;
    ListNode node_uncommon;

    while (Lnode != NULL)
    {
        uncommon_list = (List)Lnode->value;
        string *str = new string(((item *)(uncommon_list->list_first()->value))->get_item_full_id());
        if (visited_lists->search(str, cmp_hashtable_search)) //the uncommon list was visited before
        {
            delete str;
            Lnode = Lnode->next;
            continue;
        }
        delete str;
        node_uncommon = uncommon_list->list_first();
        while (node_uncommon != NULL)
        {
            fprintf(output_file, "%s,%s,0\n", it->get_item_full_id().c_str(), ((item *)node_uncommon->value)->get_item_full_id().c_str());
            node_uncommon = node_uncommon->next;
        }
        Lnode = Lnode->next;
    }

    delete list_of_uncommon_lists;
}

void print_commons(HashTable visited_lists, item *it, FILE *output_file)
{
    List list_to_visit = it->get_common_and_uncommon().common;
    string *str = new string(((item *)(list_to_visit->list_first()->value))->get_item_full_id());
    if (visited_lists->search(str, cmp_hashtable_search))
    {
        delete str;
        return;
    }
    ListNode nodeOne = list_to_visit->list_first();
    item *itemOne = (item *)list_to_visit->list_node_value(nodeOne);
    ListNode nodeTwo = nodeOne;
    item *itemTwo = itemOne;
    while (nodeOne != list_to_visit->list_last())
    {
        nodeTwo = list_to_visit->list_next(nodeTwo);
        itemTwo = (item *)list_to_visit->list_node_value(nodeTwo);
        fprintf(output_file, "%s,%s,1\n", itemOne->get_item_full_id().c_str(), itemTwo->get_item_full_id().c_str());
        if (nodeTwo == list_to_visit->list_last())
        {
            print_uncommon(visited_lists, itemOne, output_file);
            nodeOne = list_to_visit->list_next(nodeOne);
            itemOne = (item *)list_to_visit->list_node_value(nodeOne);
            nodeTwo = nodeOne;
            itemTwo = itemOne;
        }
    }
    print_uncommon(visited_lists, itemOne, output_file);

    HashTable_Node ht_n = new hashtable_node;
    ht_n->key = str;
    ht_n->value = list_to_visit;

    visited_lists->insert(ht_n);
}

void destroy_item(Pointer value)
{
    delete (item *)value;
}

void del_item_ht(Pointer value)
{
    delete (int *)((HashTable_Node)value)->key;
    delete (item *)((HashTable_Node)value)->value;
    delete (HashTable_Node)value;
}

int hashfunction_int(Pointer key, int buckets)
{
    return (*(int *)key) % buckets;
}

int read_files(string main_folder, string folder, HashTable htable, HashTable idf_htable) // folder is inner folder (ex. ebay.com) ./data/2013_camera_specs/buy.net/4233.json
{
    DIR *dir;
    dirent *dir_item;

    string name;

    HashTable ht_items = new hashtable(300, del_item_ht, hashfunction_int);
    HashTable_Node ht_n = new hashtable_node();
    ht_n->key = new string(folder);
    ht_n->value = ht_items;

    dir = opendir((main_folder + "/" + folder).c_str());
    dir_item = readdir(dir);
    item *it;
    List spec_list = NULL;
    while (dir_item != NULL) // for every file in dir
    {
        if (dir_item->d_type == DT_DIR) // skip "." and ".."
        {
            dir_item = readdir(dir);
            continue;
        }
        name = dir_item->d_name;
        it = new item(folder, atoi(name.substr(0, name.size() - 5).c_str()));
        // printf("parsing...\n");
        spec_list = parse(main_folder + "/" + folder + "/" + name, it->get_words_ht(), idf_htable);
        // printf("finished parsing...\n");
        it->set_spec_list(spec_list);
        HashTable_Node ht_item_n = new hashtable_node();
        ht_item_n->key = new int(it->get_item_id());
        ht_item_n->value = it;
        ht_items->insert(ht_item_n);

        dir_item = readdir(dir);
    }
    htable->insert(ht_n);
    closedir(dir);
    return ht_items->ht_size();
}

void del_main_ht(Pointer value)
{
    delete (string *)((HashTable_Node)value)->key;
    delete (HashTable)((HashTable_Node)value)->value;
    delete (HashTable_Node)value;
}

int read_all_folders(string dir_name, HashTable ht, HashTable idf_ht)
{
    int count = 0;
    struct dirent *de;
    DIR *dr = opendir(dir_name.c_str());
    if (!dr)
    {
        perror(dir_name.c_str());
        exit(1);
    }
    while ((de = readdir(dr)) != NULL)
    {
        // struct timeval t1, t2;
        // double elapsedTime;
        // gettimeofday(&t1, NULL);

        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
            continue;
        count += read_files(dir_name, de->d_name, ht, idf_ht);

        // gettimeofday(&t2, NULL);
        // elapsedTime = (t2.tv_sec - t1.tv_sec);                // sec to ms
        // elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000000.0; // us to ms
        // printf("%f s. for %s\n", elapsedTime, de->d_name);
    }

    closedir(dr);
    return count;
}

void read_csv(string filename, HashTable ht)
{
    HashTable ht_items;
    item *it1, *it2;
    string name1, name2, line;
    int num1, num2, similar, comma1, comma2, slash1, slash2;
    size_t buffer_size = 0;
    char *buffer = NULL;
    FILE *stream = fopen(filename.c_str(), "r");
    if (!stream)
    {
        printf("Error reading file: %s\n", filename.c_str());
        return;
    }
    getline(&buffer, &buffer_size, stream);
    while (getline(&buffer, &buffer_size, stream) != -1)
    {
        line = buffer;
        slash1 = line.find("//");
        comma1 = line.find(',', slash1);
        slash2 = line.find("//", comma1);
        comma2 = line.find(',', slash2);

        name1 = line.substr(0, slash1);
        num1 = stoi(line.substr(slash1 + 2, comma1 - slash1 + 2));
        name2 = line.substr(comma1 + 1, slash2 - comma1 + 1 - 2);
        num2 = stoi(line.substr(slash2 + 2, comma2 - slash2 + 2));
        similar = stoi(line.substr(comma2 + 1));

        // printf("%-25s // %-10d ---- %-25s // %-5d ---- %d\n",name1.c_str(), num1, name2.c_str(), num2, similar);

        ht_items = (HashTable)ht->search(&name1, cmp_hashtable_search);
        if (ht_items == NULL)
            continue;
        it1 = (item *)ht_items->search(&num1, cmp_hashtable_search_item);
        ht_items = (HashTable)ht->search(&name2, cmp_hashtable_search);
        if (ht_items == NULL)
            continue;
        it2 = (item *)ht_items->search(&num2, cmp_hashtable_search_item);
        if (similar)
        {
            similar_items(it1, it2);
        }
        else
        {
            dissimilar_items(it1, it2);
        }
    }
    free(buffer);
    fclose(stream);
}

HashTable visited_lists;
FILE *output;

void destroy_list(Pointer value)
{
    delete (List)value;
}

void print_all_commons(item *value)
{
    // printf("%s\n",((item*)value)->get_item_full_id().c_str());
    print_commons(visited_lists, value, output);
}

void del_visited_lists_ht(Pointer value)
{
    delete (string *)((HashTable_Node)value)->key;
    delete (List)((HashTable_Node)value)->value;
    delete (HashTable_Node)value;
}
void del_visited_uncommon_ht(Pointer value)
{
    delete (HashTable)((HashTable_Node)value)->value;
    ((HashTable_Node)value)->key = NULL;
    delete (HashTable_Node)value;
}
void print_all(HashTable ht, FILE *output_file)
{
    visited_lists = new hashtable(200, del_visited_lists_ht, hashfunction);
    HashTable visited_uncommon = new hashtable(200, del_visited_uncommon_ht, hashfunction_address);
    output = output_file;
    List hts = ht->return_list();
    ListNode tempNode = hts->list_first();
    while (tempNode != NULL)
    {
        List items_list = ((HashTable)tempNode->value)->return_list();
        ListNode tempNode1 = items_list->list_first();
        while (tempNode1 != NULL)
        {
            item *it = (item *)tempNode1->value;
            print_all_commons(it);
            tempNode1 = items_list->list_next(tempNode1);
            if (visited_uncommon->search(it->get_common_and_uncommon().uncommon, cmp_hashtable_search_address) == NULL)
            {
                HashTable_Node ht_n = new hashtable_node;
                ht_n->value = it->get_common_and_uncommon().uncommon;
                ht_n->key = it->get_common_and_uncommon().uncommon;
                visited_uncommon->insert(ht_n);
            }
        }
        tempNode = hts->list_next(tempNode);
        delete items_list;
    }
    delete hts;
    delete visited_lists;
    delete visited_uncommon;
}

void set_Bow_or_TfIdf(HashTable ht, HashTable idf, int item_count, bool flag)
{
    List ht_l = ht->return_list();
    List idf_l = idf->return_ht_nodes();
    ListNode ht_l_node = ht_l->list_first();
    ListNode l_node = idf_l->list_first();
    int c = 0;
    while (l_node != NULL)
    {
        /*                                          single-document words                                                          low IDFs   */
        if ((((int *)(((HashTable_Node)(l_node->value))->value))[1] < 2) || ((((int *)(((HashTable_Node)(l_node->value))->value))[1] > item_count / 10)))
        {
            // printf("%d\n", c++);
            idf->remove((((HashTable_Node)(l_node->value))->key));
        }
        else
        {
            ((int *)(((HashTable_Node)(l_node->value))->value))[0] = c;
            c++;
        }

        l_node = l_node->next;
    }
    printf("items = %d , idf size = %d\n", item_count, idf->ht_size());

    while (ht_l_node != NULL)
    {
        List ht_ll = ((HashTable)(ht_l_node->value))->return_list();
        ListNode ht_ll_node = ht_ll->list_first();
        while (ht_ll_node != NULL)
        {
            item *it = (item *)(ht_ll_node->value);
            List words_list = it->get_words_ht()->return_ht_nodes();
            if (!flag)
            {
                double *bow = new double[idf->ht_size()]();
                it->set_table(bow);
                l_node = words_list->list_first();
                while (l_node != NULL)
                {
                    int *tmp = (int *)idf->search((((HashTable_Node)(l_node->value))->key), cmp_hashtable_search);
                    if (tmp != NULL)
                        bow[tmp[0]] = tmp[1] * 1.0;

                    l_node = l_node->next;
                }
            }
            else
            {
                double *tfidf = new double[idf->ht_size()]();
                l_node = words_list->list_first();
                while (l_node != NULL)
                {
                    int *tmp = (int *)idf->search((((HashTable_Node)(l_node->value))->key), cmp_hashtable_search);
                    if (tmp != NULL)
                    {
                        tfidf[tmp[0]] = ((*((int *)(((HashTable_Node)(l_node->value))->value)) * 1.0) / (it->get_words_ht()->ht_size() * 1.0)) * log((item_count * 1.0) / ((tmp[1]) * 1.0));
                    }
                    l_node = l_node->next;
                }
                it->set_table(tfidf);
            }
            delete words_list;

            ht_ll_node = ht_ll_node->next;
        }
        ht_l_node = ht_l_node->next;
        delete ht_ll;
    }
    delete ht_l;
    delete idf_l;
}

int lines_counter(const char *filename)
{
    FILE *fp;
    int count = 0;
    char c;

    fp = fopen(filename, "r");

    if (fp == NULL)
        return 0;

    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n')
            count = count + 1;

    fclose(fp);

    return count;
}

double *train(string filename, HashTable ht, HashTable idf, int reps)
{
    HashTable ht_items;
    item *it1, *it2;
    string name1, name2, line;
    int num1, num2, similar, comma1, comma2, slash1, slash2;
    int i;
    // int counter;
    double a = 0.5;
    double e = 2.71;
    double b = 0.0;
    double f, sigma, new_b;
    double dj[idf->ht_size()];
    double x[idf->ht_size()];
    double W[idf->ht_size()] = {0.0};
    double new_W[idf->ht_size()] = {0.0};
    /* last position of the best weights array is the best b */
    double *best_W = new double[idf->ht_size() + 1]();

    for (i = 0; i < reps; i++)
    {
        // counter = 0;
        size_t buffer_size = 0;
        char *buffer = NULL;
        FILE *stream = fopen(filename.c_str(), "r");
        if (!stream)
        {
            printf("Error reading file: %s\n", filename.c_str());
            return NULL;
        }

        // split threads

        getline(&buffer, &buffer_size, stream);
        while ((getline(&buffer, &buffer_size, stream) != -1))
        {
            line = buffer;
            slash1 = line.find("//");
            comma1 = line.find(',', slash1);
            slash2 = line.find("//", comma1);
            comma2 = line.find(',', slash2);

            name1 = line.substr(0, slash1);
            num1 = stoi(line.substr(slash1 + 2, comma1 - slash1 + 2));
            name2 = line.substr(comma1 + 1, slash2 - comma1 + 1 - 2);
            num2 = stoi(line.substr(slash2 + 2, comma2 - slash2 + 2));
            similar = stoi(line.substr(comma2 + 1));

            // printf("%-25s // %-10d ---- %-25s // %-5d ---- %d\n",name1.c_str(), num1, name2.c_str(), num2, similar);

            ht_items = (HashTable)ht->search(&name1, cmp_hashtable_search);
            if (ht_items == NULL)
                continue;
            it1 = (item *)ht_items->search(&num1, cmp_hashtable_search_item);
            ht_items = (HashTable)ht->search(&name2, cmp_hashtable_search);
            if (ht_items == NULL)
                continue;
            it2 = (item *)ht_items->search(&num2, cmp_hashtable_search_item);

            /* merge tables */

            for (int j = 0; j < idf->ht_size(); j++)
            {
                x[j] = abs(it1->get_bow_tfidf()[j] - it2->get_bow_tfidf()[j]);
                // x[j] = (it1->get_bow_tfidf()[j] + it2->get_bow_tfidf()[j]) / 2;
            }

            //critical start-------------------------------------------------------------------------------------------------------------------
            f = b;
            for (int j = 0; j < idf->ht_size(); j++)
            {
                f += W[j] * x[j];
            }

            sigma = 1.0 / (1.0 + pow(e, (-1.0) * f));
            // printf("sigma %f  -f %f\n",sigma,(-1.0)*f); getchar();

            new_b = b - a * (sigma - similar);
            if ((new_b - b) > 0.00001)
            {
                b = new_b;
            }
            else
            {
                best_W[idf->ht_size()] = b;
                b = new_b;
            }

            for (int j = 0; j < idf->ht_size(); j++)
            {
                // if (best_W[j] != 0.0)
                //     continue;
                dj[j] = (sigma - similar) * x[j];
                new_W[j] = W[j] - a * dj[j];
                if ((new_W[j] - W[j]) > 0.00001)
                {
                    W[j] = new_W[j];
                    // printf("%d -- W[%d] changed\n", counter, j);
                }
                else
                {
                    best_W[j] = W[j];
                    W[j] = new_W[j];
                    // printf("%d -- W[%d] is best\n", counter, j);
                }
            }

            //critical stop-------------------------------------------------------------------------------------------------------------------

            // counter++;
            // getchar();
        }
        free(buffer);
        fclose(stream);
        //merge threads
    }
    /* write best weights and b into a file */
    FILE *W_best = fopen("./best_W", "w");
    for (i = 0; i <= idf->ht_size(); i++)
    {
        fprintf(W_best, "%f\n", best_W[i]);
    }

    return best_W;
}

struct train_data
{
    int line_count;
    string *lines;
    HashTable ht;
    HashTable idf;
    int reps;
    double *W;
};

class train_job : public job
{
    train_data *data;

public:
    train_job(Pointer data_p)
    {
        data = (train_data *)data_p;
    }
    /* code to be run by threads */
    Pointer run()
    {

        HashTable ht_items;
        item *it1, *it2;
        string name1, name2, line;
        int num1, num2, similar, comma1, comma2, slash1, slash2;
        int i;
        // int counter;
        double e = 2.71;
        double b = 0.0;
        double f, sigma;
        /* last position of the delta array is the b */
        double *return_table = new double[data->idf->ht_size() + 1];

        double x[data->idf->ht_size()];
        for (i = 0; i < data->reps; i++)
        {
            // counter = 0;
            for (int n = 0; n < data->line_count; n++)
            {
                line = data->lines[n];
                slash1 = line.find("//");
                comma1 = line.find(',', slash1);
                slash2 = line.find("//", comma1);
                comma2 = line.find(',', slash2);

                name1 = line.substr(0, slash1);
                num1 = stoi(line.substr(slash1 + 2, comma1 - slash1 + 2));
                name2 = line.substr(comma1 + 1, slash2 - comma1 + 1 - 2);
                num2 = stoi(line.substr(slash2 + 2, comma2 - slash2 + 2));
                similar = stoi(line.substr(comma2 + 1));

                // printf("%-25s // %-10d ---- %-25s // %-5d ---- %d\n",name1.c_str(), num1, name2.c_str(), num2, similar);

                ht_items = (HashTable)data->ht->search(&name1, cmp_hashtable_search);
                if (ht_items == NULL)
                    continue;
                it1 = (item *)ht_items->search(&num1, cmp_hashtable_search_item);
                ht_items = (HashTable)data->ht->search(&name2, cmp_hashtable_search);
                if (ht_items == NULL)
                    continue;
                it2 = (item *)ht_items->search(&num2, cmp_hashtable_search_item);

                /* merge tables */

                for (int j = 0; j < data->idf->ht_size(); j++)
                {
                    x[j] = abs(it1->get_bow_tfidf()[j] - it2->get_bow_tfidf()[j]);
                    // x[j] = (it1->get_bow_tfidf()[j] + it2->get_bow_tfidf()[j]) / 2;
                }

                f = b;
                for (int j = 0; j < data->idf->ht_size(); j++)
                {
                    f += data->W[j] * x[j];
                }

                sigma = 1.0 / (1.0 + pow(e, (-1.0) * f));

                /* this is b */
                return_table[data->idf->ht_size()] = (sigma - similar);

                for (int j = 0; j < data->idf->ht_size(); j++)
                {
                    return_table[j] = (sigma - similar) * x[j];
                }
            }
        }
        return return_table;
    }
};

double *train_main_thread(string filename, HashTable ht, HashTable idf, int reps, int batch_size, int thread_count, int pool_size)
{
    jobScheduler* jsched=new jobScheduler(thread_count, pool_size);
    train_data *t_data = new train_data[thread_count];
    string *lines = new string[lines_counter(filename.c_str())];
    /* last position is b */
    double *W = new double[idf->ht_size() + 1]();
    double new_W[idf->ht_size() + 1];
    double *best_W = new double[idf->ht_size() + 1]();
    double a = 0.6;
    int i, all_lines;
    size_t buffer_size = 0;
    char *buffer = NULL;
    FILE *stream = fopen(filename.c_str(), "r");
    if (!stream)
    {
        printf("Error reading file: %s\n", filename.c_str());
        return NULL;
    }

    getline(&buffer, &buffer_size, stream);
    i = 0;
    while ((getline(&buffer, &buffer_size, stream) != -1))
    {
        lines[i] = buffer;
        i++;
    }
    fclose(stream);
    all_lines = i;

    int loops = all_lines / (batch_size * thread_count);
    /* if the division is not perfect, then another loop must be executed with the remaining lines */
    if (all_lines % (batch_size * thread_count) > 0)
        loops++;

    train_data *ta;
    for (int l = 0; l < loops; l++)
    {
        // printf("loop %d\t\t",l);
        /* submit jobs */
        
        for (i = 0; i < thread_count; i++)
        {
            // printf("thread %d\n", i);
            if (all_lines - (i * batch_size) <= 0)
                break;
            ta = &t_data[i];
            ta->ht = ht;
            ta->idf = idf;
            ta->reps = reps;
            ta->W = W;
            if (all_lines - ((i + 1) * batch_size) >= 0)
                ta->line_count = batch_size;
            else /* this is the final batch with fewer lines than the rest */
                ta->line_count = all_lines - (i * batch_size);
            ta->lines = &lines[i * ta->line_count];

            jsched->submit_job(new train_job(ta));
        }

        ListNode node;
        double sum;
        int div;
        jsched->wait_all();
        /* change w */
        for (i = 0; i <= idf->ht_size(); i++)
        {
       
            node = jsched->get_return_values()->list_first();
            sum = 0.0;
            div = 0;
            while (node != NULL)
            {
                sum += ((double *)node->value)[i];
                div++;
                node = node->next;
            }

            // printf("%.30f ----------------------\n",(div * 1.0));
            // getchar();
            
            new_W[i] = W[i] - a * (sum / (div * 1.0));
            if ((new_W[i] - W[i]) > 0.00001)
            {
                W[i] = new_W[i];
                // printf("%d -- W[%d] changed\n", counter, j);
            }
            else
            {
                best_W[i] = W[i];
                W[i] = new_W[i];
                // printf( "%f -- W[%d] is best\n",W[i], i);
            }
        }
    }


    /* write best weights and b into a file */
    FILE *W_best = fopen("./best_W", "w");
    for (i = 0; i <= idf->ht_size(); i++)
    {
        fprintf(W_best, "%f\n", best_W[i]);
    }
    fclose(W_best);
    delete jsched;
    delete[] lines;
    delete[] t_data;
    return best_W;
}

void test(string filename, HashTable ht, double *W, int idf_size, bool validation)
{
    int score = 0, s1 = 0, s0 = 0;
    int total = 0, t1 = 0, t0 = 0;
    double e = 3.17;
    double sum, res;
    HashTable ht_items;
    item *it1, *it2;
    string name1, name2, line;
    int num1, num2, similar, comma1, comma2, slash1, slash2;
    size_t buffer_size = 0;
    char *buffer = NULL;
    FILE *stream = fopen(filename.c_str(), "r");
    if (!stream)
    {
        printf("Error reading file: %s\n", filename.c_str());
        return;
    }
    getline(&buffer, &buffer_size, stream);
    while (getline(&buffer, &buffer_size, stream) != -1)
    {
        line = buffer;
        slash1 = line.find("//");
        comma1 = line.find(',', slash1);
        slash2 = line.find("//", comma1);
        comma2 = line.find(',', slash2);

        name1 = line.substr(0, slash1);
        num1 = stoi(line.substr(slash1 + 2, comma1 - slash1 + 2));
        name2 = line.substr(comma1 + 1, slash2 - comma1 + 1 - 2);
        num2 = stoi(line.substr(slash2 + 2, comma2 - slash2 + 2));
        similar = stoi(line.substr(comma2 + 1));

        // printf("%-25s // %-10d ---- %-25s // %-5d ---- %d\n",name1.c_str(), num1, name2.c_str(), num2, similar);

        ht_items = (HashTable)ht->search(&name1, cmp_hashtable_search);
        if (ht_items == NULL)
            continue;
        it1 = (item *)ht_items->search(&num1, cmp_hashtable_search_item);
        ht_items = (HashTable)ht->search(&name2, cmp_hashtable_search);
        if (ht_items == NULL)
            continue;
        it2 = (item *)ht_items->search(&num2, cmp_hashtable_search_item);

        sum = W[idf_size];
        for (int j = 0; j < idf_size; j++)
        {
            sum += abs(it1->get_bow_tfidf()[j] - it2->get_bow_tfidf()[j]) * W[j];
            // sum += (it1->get_bow_tfidf()[j] + it2->get_bow_tfidf()[j]) / 2 * W[j];

            // printf("%f - %f - %f\n",it1->get_bow_tfidf()[j] , it2->get_bow_tfidf()[j], W[j]);
            // getchar();
        }
        res = 1.0 / (1.0 + pow(e, (-1.0) * sum));
        /* after processing the results, the threshhold was decided at 0.2 */
        if (similar)
        {
            t1++;
            if (res > 0.0000000000001)
            {
                s1++;
                score++;
                printf("sum = %f, res = %.100f, similar = %d\n", sum, res, similar);
            }
            else
            {
                printf("sum = %f, res = %.100f, similar = %d\n", sum, res, similar);
            }
        }
        else
        {
            t0++;
            if (res <= 0.0000000000001)
            {
                s0++;
                score++;
                printf("sum = %f, res = %.100f, similar = %d\n", sum, res, similar);

            }
            else
            {
                printf("sum = %f, res = %.100f, similar = %d\n", sum, res, similar);
            }
        }
        total++;

        // if (similar)
        // printf("sum = %f, res = %f, similar = %d\n", sum, res, similar);
    }

    printf("total score:\t %d/%d\n0s:\t\t %d/%d\n1s:\t\t %d/%d\n", score, total, s0, t0, s1, t1);
}

int findYourArg(int argc, const char *argv[], const char what[3])
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], what) == 0)
        {
            if (argc >= i + 2)
            {
                return i + 1;
            }
        }
    }
    return 0;
}
