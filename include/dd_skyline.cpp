#include "dd_skyline.h"
#include "../thirdparty/utils.h"
#include "../thirdparty/drq_data_structure.h"
#include "../thirdparty/drq_operation.h"
#include "cstdio"
#include "iostream"
#include "fstream"
#include "sstream"
#include "openssl/bn.h"

using namespace std;
// 读取用来查询的y点
point_y * read_point_y(char * filePath){

}
// 释放y点
RESULT free_point_y(point_y * y){

}
// 按照vector中的id来获取各个维度的点
RESULT organize_points_by_ids(vector<int > * ids,vector<void *> ret){

}
// 逐次进行比较，然后写入文件
RESULT select_and_export_points(vector<void *>ret,point_y * y,char resultPath){

}