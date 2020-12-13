#include <stdio.h>

int hashfunction_address(void* value)
{
    long int a=(long int)&value;
    return a%10;
}

// a: mem_address      b: hash_table_node
int cmp_hashtable_search_address(void* a,void* b)
{
    return (a==b)?0:1;
}

int main()
{
    char a=10;
    char *pa=&a;
    char *pb=&a;
    pa=(char *)&a;
    printf("%s   \n",pa);
    // char b = (char)&a;
    printf("%d   \n",hashfunction_address(pa));
    printf("%d   \n",hashfunction_address(pb));
    printf("%d   \n",cmp_hashtable_search_address(pa,pb));
}