#include "cstdio"
#include "iostream"
#include "fstream"
#include "sstream"
#include "openssl/bn.h"
#include "unordered_map"
#include "algorithm"
#include "dd_skyline.h"

void printfTime(char* desc,clock_t start) {
    double cpu_time_used = ((double)(clock() - start)) / CLOCKS_PER_SEC;

    printf("%s used: %f seconds\n",desc, cpu_time_used);
}
int main() {
    clock_t start, end;
    start = clock();
	drq_data_set set;
	init_algo(&set, "/root/heqi/encryption_algorithm/dd_skyline/data/data.txt");
    printfTime("init",start);
    start = clock();
    query_algo(&set, "/root/heqi/encryption_algorithm/dd_skyline/data/range.txt", "/root/heqi/encryption_algorithm/dd_skyline/data/SKYLINE_QUERY_POINT_Y_FILE.txt", "/root/heqi/encryption_algorithm/dd_skyline/data/result.txt");
    free_algo(&  set);
    printfTime("query",start);
	return 0;
}