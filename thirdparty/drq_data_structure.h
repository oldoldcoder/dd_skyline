/**
@author:heqi
@time:2024/04/10
@desc:分布式范围查询的操作头文件
*/
#ifndef DRQ_DATA_STRUCTURE_H
#define DRQ_DATA_STRUCTURE_H

#include "openssl/bn.h"
#include "utils.h"
#include "vector"
#ifdef _WIN32
#define EXPORT_SYMBOL __declspec(dllexport)
#else
#define EXPORT_SYMBOL __attribute__((visibility("default")))
#endif
/*-------------------JNA接口定义------------------*/
/*-----------------------------常量定义----------------------------*/
// 数据拥有者
typedef struct {
    // id
    int id;
    BIGNUM * de_val;
}drq_data_owner_unit;

typedef struct {
    // 长度
    int n;
    // 一个数据
    drq_data_owner_unit ** units;
    // 记录查询的范围
    BIGNUM ** query_range;
    // 记录查询的结果
    std::vector<int> * query_res;
}drq_data_owner;

// 数据总的持有者
typedef struct {
    int n;
    // 维度也是数据拥有者的量
    int d;
    // 数据拥有者的集合
    drq_data_owner ** owners;
    // 保存上下文
    BN_CTX * ctx;
}drq_data_set;



// data_set创建数据拥有者
RESULT drq_init_set(drq_data_set * set);
// 读取分散数据到data_owner
RESULT drq_read_data_to_owner(drq_data_set * set,const char * filePath);
// data_owner对于自己持有的每一个数据进行加密，分散到三方上面去
RESULT drq_encrypt_data_owner(drq_data_owner * owner);
// 销毁data_set
RESULT drq_free_data_set(drq_data_set * set);
// 发送通信每个data_owner开始自查
RESULT drq_notify_do_query(drq_data_set * set);
// 进行隐私集合求交
std::vector<int> drq_PSI(drq_data_set * set);
#endif //DRQ_DATA_STRUCTURE_H