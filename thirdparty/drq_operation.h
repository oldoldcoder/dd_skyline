/**
@author:heqi
@time:2024/04/10
@desc:分布式范围查询的数据结构头文件
*/
#ifndef DRQ_OPERATION_H
#define DRQ_OPERATION_H

#include "openssl/bn.h"
#include "utils.h"
#include "drq_data_structure.h"
#include <vector>
/*-----------------------------常量定义----------------------------*/
// 分布式查询的范围定义
typedef struct {
    int d;
    // 每个元素都是一个二维的数组，[0]代表了lower,[1]代表了upper
    BIGNUM *** range;
}query_range;
/*--------------------------JNA定义----------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
EXPORT_SYMBOL RESULT init_algo(drq_data_set * set,char * filePath);
EXPORT_SYMBOL RESULT query_algo(drq_data_set * set,char * queryPath,char * resultFilePath);
EXPORT_SYMBOL RESULT free_algo(drq_data_set * set);
#ifdef __cplusplus
}
#endif
/*---------------------操作方法---------------------*/
// 初始化查询范围
RESULT drq_init_query_range(query_range * ranges,int d);
// 读取查询范围
RESULT drq_read_query_range(query_range * ranges,const char *queryFile);
// 将不同维度的查询范围发送过去，获取查询的结果
RESULT drq_send_range2owner(query_range * ranges,drq_data_set * set);
// 释放query_range
RESULT drq_free_query_range(query_range * ranges);
// 将结果写入到文件中去
RESULT drq_write_res2File(std::vector<int> & ve,query_range & range,const char * resultFilePath);

#endif //DRQ_OPERATION_H