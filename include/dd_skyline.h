/**
 * @author heqi
 * @date 2024/07/11
 * @desc skyline查询，调用了一部分drp的安全范围查询，其实也就是查询吧
 * */
#include "../thirdparty/utils.h"
#include "../thirdparty/drq_operation.h"
#include "../thirdparty/drq_data_structure.h"
#include "openssl/bn.h"
#include "vector"
#include "unordered_map"
/**
 * 工作的流程：先范围查询，得到了id的vector，然后根据vector组织点去进行挨个查询看看
 * */
using namespace  std;
// 定义y点的结构
typedef struct {
    int d;
    BIGNUM ** values;
} point_y;
/**--------------------导出JNA的接口函数------------------*/
#ifdef __cplusplus
extern "C" {
#endif

EXPORT_SYMBOL RESULT init_algo(drq_data_set * set,char * filePath);
EXPORT_SYMBOL RESULT query_algo(drq_data_set * set,char * queryPath,char * resultFilePath);
EXPORT_SYMBOL RESULT free_algo(drq_data_set * set);

#ifdef __cplusplus
}
#endif
// 读取用来查询的y点
point_y * read_point_y(char * filePath);
// 释放y点
RESULT free_point_y(point_y * y);
// 按照vector中的id来获取各个维度的点
RESULT organize_points_by_ids(drq_data_set  * set,vector<int > * ids,unordered_map<int,void *> *ret);
// 逐次进行比较，然后写入文件
RESULT select_and_export_points(unordered_map<int,void *>  *ret,point_y * y,char * resultPath);
