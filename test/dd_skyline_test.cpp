#include "cstdio"
#include "iostream"
#include "fstream"
#include "sstream"
#include "openssl/bn.h"
#include "unordered_map"
#include "algorithm"
#include "dd_skyline.h"

int main() {
	drq_data_set set;
	init_algo(&set, "D:\\study\\code\\ClionProject\\dd_skyline\\data\\SKYLINE_DATA_FILE.txt");
	query_algo(&set, "D:\\study\\code\\ClionProject\\dd_skyline\\data\\SKYLINE_QUERY_RANGE_FILE.txt", "D:\\study\\code\\ClionProject\\dd_skyline\\data\\SKYLINE_QUERY_POINT_Y_FILE.txt", "D:\\study\\code\\ClionProject\\dd_skyline\\data\\SKYLINE_RES_FILE.txt");
    free_algo(&  set);
	return 0;
}