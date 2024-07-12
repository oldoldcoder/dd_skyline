#include "dd_skyline.h"
#include "../thirdparty/utils.h"
#include "../thirdparty/drq_data_structure.h"
#include "../thirdparty/drq_operation.h"
#include "cstdio"
#include "iostream"
#include "fstream"
#include "sstream"
#include "openssl/bn.h"
#include "unordered_map"
#include "algorithm"

using namespace std;
/**-------------JNA的接口内容------------------*/
EXPORT_SYMBOL RESULT init_algo(drq_data_set * set,char * filePath){
    // 初始化set
    if(drq_init_set(set) != SUCCESS){
        cerr<<" init_set error!"<<endl;
        return ERROR;
    }
    if(drq_read_data_to_owner(set,filePath) != SUCCESS){
        cerr<<" read_data error!"<<endl;
        return ERROR;
    }
    int d = set->d;
    for(int i = 0; i < d ; i++){
        if(drq_encrypt_data_owner(set->owners[i])!= SUCCESS){
            cerr<<" encrypt_data error!"<<endl;
            return ERROR;
        }
    }

}
EXPORT_SYMBOL RESULT query_algo(drq_data_set * set,char * queryPath,char * resultFilePath){
    // 先构建查询的范围
    query_range range;
    drq_init_query_range(&range,set->d);

    drq_read_query_range(&range,queryPath);
    // 发送自查到dataOwner
    drq_send_range2owner(&range,set);

    // 内容自查
    drq_notify_do_query(set);

    // 对于vector求交集运算
    vector<int> ve = drq_PSI(set);

    drq_write_res2File(ve,range,resultFilePath);
    //  情况原有的vector的所有内容
    for(int i = 0 ; i < set->d ; ++i){
        set->owners[i]->query_res->clear();  // 清空向量
    }
    drq_free_query_range(&range);
    return SUCCESS;
}
EXPORT_SYMBOL RESULT free_algo(drq_data_set * set){
    drq_free_data_set(set);
    return SUCCESS;
}
// 读取用来查询的y点
point_y * read_point_y(char * filePath){
    ifstream infile(filePath);
    if (!infile.is_open()) {
        cerr << "Error opening file: " << filePath << endl;
        return nullptr;
    }
    auto * y = (point_y *) malloc(sizeof(point_y));
    string  line;
    // 读取数据拥有者的数量
    if (std::getline(infile, line)) {
        std::istringstream iss(line);
        iss >> y->d;
    } else {
        std::cerr << "Error: Could not read the number of data owners." << std::endl;
        return nullptr;
    }
    int idx = 0 ;
    // 读取文件的第二行
    if (getline(infile, line)) {
        stringstream ss(line);
        string value;
        while (ss >> value) {
            BIGNUM* bn = BN_new();
            if (BN_dec2bn(&bn, value.c_str()) == 0) {
                cerr << "Error converting string to BIGNUM" << endl;
                BN_free(bn);
                free(y);
                return nullptr;
            }
            y->values[idx++] = bn ;
        }
    } else {
        cerr << "Error reading second line from file" << endl;
        return nullptr;
    }
    infile.close();
    return y;
}
// 释放y点
RESULT free_point_y(point_y * y){
    int d = y->d;
    for(int i = 0; i < d ; ++i){
        BN_free(y->values[i]);
    }
    free(y->values);
    free(y);
    return SUCCESS;
}
// 按照vector中的id来获取各个维度的点
RESULT organize_points_by_ids(drq_data_set  * set,vector<int > * ids,unordered_map<int,void *>  *ret){
    // 首先来组织数据
    int d = set->d;
    int n = set->n;

    for(int i = 0 ; i < d ; ++i){
        size_t vecSize = ids->size();
        for(int j = 0 ; j < vecSize ; ++j){
            // 先查询 set->owners[i]->units[(*ids)[j]]->de_val)
            auto it = ret->find((*ids)[j]);
            if(it == ret->end()){
                // 创建新的结构，然后扔进去
                auto arr = new vector<BIGNUM *>;
                arr->push_back(set->owners[i]->units[(*ids)[j]]->de_val);
                // arr放入map
                ret->insert(make_pair((*ids)[j],arr));
            }else{
                auto arr = reinterpret_cast<vector<BIGNUM *>*>(it->second);
                arr->push_back(set->owners[i]->units[(*ids)[j]]->de_val);
            }
        }
    }
    // 按照维度遍历，则插入正常
    return SUCCESS;
}
BIGNUM* BN_abs(BIGNUM* bn) {
    BIGNUM* abs_bn = BN_dup(bn);
    if (BN_is_negative(abs_bn)) {
        BN_set_negative(abs_bn, 0);
    }
    return abs_bn;
}
bool is_dominated(vector<BIGNUM*> * x_i, vector<BIGNUM*>* x_j, point_y * y) {
    bool all_less_equal = true;
    bool at_least_one_less = false;

    BIGNUM* abs_diff_i ;
    BIGNUM* abs_diff_j ;
    BIGNUM* diff_i = BN_new();
    BIGNUM* diff_j = BN_new();
    for (size_t l = 0; l < x_i->size(); ++l) {


        BN_sub(diff_i, (*x_i)[l], y->values[l]);
        BN_sub(diff_j, (*x_i)[l], y->values[l]);
        abs_diff_i = BN_abs( diff_i);
        abs_diff_j = BN_abs( diff_j);

        if (BN_cmp(abs_diff_i, abs_diff_j) > 0) {
            all_less_equal = false;
            break;
        }
        if (BN_cmp(abs_diff_i, abs_diff_j) < 0) {
            at_least_one_less = true;
        }
        BN_free(abs_diff_i);
        BN_free(abs_diff_j);
    }
    BN_free(diff_i);
    BN_free(diff_j);
    return all_less_equal && at_least_one_less;
}


// 逐次进行比较，然后写入文件
RESULT select_and_export_points(unordered_map<int,void *>  *ret,point_y * y,char * resultPath){
    // 存储都不被支配的点
    vector<int> non_dominated;
    for(auto it = ret->begin() ; it != ret->end() ; ++it){
        bool dominated = false;
        for(auto itt = ret->begin() ; itt != ret->end() ; ++itt){
            // 获取second
            if (it != itt && is_dominated(reinterpret_cast<vector<BIGNUM *>*>(it->second), reinterpret_cast<vector<BIGNUM *>*>(itt->second), y)) {
                dominated = true;
                break;
            }
        }
        if (!dominated) {
            non_dominated.push_back(it->first);
        }
    }


    // 打开文件写文件了：
    FILE *file = fopen(resultPath, "w");
    if (file == nullptr) {
        perror("Error opening file");
        return ERROR;
    }
    fprintf(file, "target y:\n");
    int d = y->d;
    for(int i = 0 ; i < d ; ++i){
        char * str = BN_bn2dec(y->values[i]);
        fprintf(file, "%s ",str);
    }
    fprintf(file,"\nThe point that is not dominated:\n");
    // 写入查询到的不被支配的点
    for(auto & it : *ret) {
        int id = it.first;
        auto vec = reinterpret_cast<vector<BIGNUM *>*>(it.second);
        fprintf(file,"%d: ",id);
        // 使用 lambda 遍历并打印每个元素
        // 使用范围基的 for 循环遍历
        for (const auto& value : *vec) {
            char * str = BN_bn2dec(value);
            fprintf(file, "%s ",str);
        }
        fprintf(file,"\n");

    }


    return SUCCESS;
}