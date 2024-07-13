#include <cstring>
#include <algorithm>
#include "openssl/bn.h"
#include "utils.h"
#include "drq_data_structure.h"
#include "iostream"
#include "sstream"
#include "fstream"

using namespace std;

// data_set创建数据拥有者
RESULT drq_init_set(drq_data_set* set) {
	set->ctx = BN_CTX_new();
	set->d = -1;
	set->n = -1;
	set->owners = nullptr;
	return SUCCESS;
}

// 读取分散数据到data_owner
RESULT drq_read_data_to_owner(drq_data_set* set, const char* filePath) {
	ifstream file(filePath);
	if (!file.is_open()) {
		perror("Error opening file");
		return ERROR;
	}

	string line;
	// 初始化data
	int totalLines, dim;
	if (!getline(file, line)) {
		perror("Error reading file");
		return ERROR;
	}
	stringstream ss(line);
	ss >> totalLines >> dim; // 读取第一行中的两个数字
	set->n = totalLines;
	set->d = dim;

	// 创建d个数据拥有者
	set->owners = (drq_data_owner**)malloc(sizeof(drq_data_owner*) * set->d);
	for (int i = 0; i < dim; ++i) {
		set->owners[i] = (drq_data_owner*)malloc(sizeof(drq_data_owner));
		// 初始化units
		set->owners[i]->units = (drq_data_owner_unit**)malloc(sizeof(drq_data_owner_unit*) * set->n);
		set->owners[i]->n = set->n;
		set->owners[i]->query_res = new vector<int>;
		// 初始化range
		set->owners[i]->query_range = (BIGNUM**)malloc(sizeof(BIGNUM*) * 2);
		set->owners[i]->query_range[0] = BN_CTX_get(set->ctx);
		set->owners[i]->query_range[1] = BN_CTX_get(set->ctx);
		for (int j = 0; j < set->n; ++j) {
			set->owners[i]->units[j] = (drq_data_owner_unit*)malloc(sizeof(drq_data_owner_unit));
			// 数据拥有者的编号
			set->owners[i]->units[j]->id = j;
		}
	}

	// 下面进行读取文件的数据
	int n = 0;
	while (getline(file, line)) {
		stringstream ss(line);
		string token;
		int idx = 0;
		while (ss >> token) {
			BIGNUM* tmp = BN_CTX_get(set->ctx);
			if (!BN_dec2bn(&tmp, token.c_str())) {
				cerr << "File has some error format about data" << endl;
				return ERROR;
			}
			set->owners[idx++]->units[n]->de_val = tmp;
		}
		if (idx != dim) {
			cerr << "Error reading data dimension from file" << endl;
			return ERROR;
		}
		n++;
	}
	if (n != set->n) {
		cerr << "Error reading data rows from file" << endl;
		return ERROR;
	}
	return SUCCESS;
}
// data_owner对于自己持有的每一个数据进行加密，分散到三方上面去
RESULT drq_encrypt_data_owner(drq_data_owner* owner) {
	// 暂时不需要加密
	return SUCCESS;
}
// 销毁data_set
RESULT drq_free_data_set(drq_data_set* set) {
	if (set == nullptr) {
		return ERROR; // 检查指针是否为空
	}

	int n = set->n;
	int d = set->d;

	for (int i = 0; i < d; ++i) {
		if (set->owners[i] != nullptr) {
			// 删除查询结果
			delete set->owners[i]->query_res;

			// 释放units
			if (set->owners[i]->units != nullptr) {
				for (int j = 0; j < n; j++) {
					free(set->owners[i]->units[j]);
				}
				free(set->owners[i]->units);
			}

			// 释放query_range
			if (set->owners[i]->query_range != nullptr) {
				free(set->owners[i]->query_range);
			}
		}
		free(set->owners[i]);
	}

	// 释放owners
	free(set->owners);

	// 释放上下文
	BN_CTX_free(set->ctx);

	// 释放set本身
	free(set);

	return SUCCESS;
}

static RESULT query_self(drq_data_owner* owner) {
	int n = owner->n;
	BIGNUM* left = owner->query_range[0];
	BIGNUM* right = owner->query_range[1];
	auto ve = owner->query_res;
	for (int i = 0; i < n; ++i) {
		BIGNUM* unit = owner->units[i]->de_val;
		// 判断 unit 是否在 [left, right] 范围内
		if (BN_cmp(unit, left) >= 0 && BN_cmp(unit, right) <= 0) {
			ve->push_back(owner->units[i]->id);  // 如果在范围内，将其添加到 query_res 中
		}
	}

	return SUCCESS;
}
// 发送通信每个data_owner开始自查
RESULT drq_notify_do_query(drq_data_set* set) {
	// 这个时候已经将范围提供给了每一个data_owner
	int d = set->d;
	for (int i = 0; i < d; ++i) {
		// 自查之后结果存储到vector之中
		query_self(set->owners[i]);
	}
	return SUCCESS;
}
static std::vector<int> intersection(const std::vector<int>& v1, const std::vector<int>& v2) {
	std::vector<int> result;
	std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(result));
	return result;
}
// 进行隐私集合求交
vector<int> drq_PSI(drq_data_set* set) {
	int d = set->d;
	for (int i = 0; i < d; ++i) {
		// 先对所有 vector 进行排序
		std::sort(set->owners[i]->query_res->begin(), set->owners[i]->query_res->end());
	}
	// 初始化结果交集为第一个vector
	vector<int> result = *set->owners[0]->query_res;
	// 依次计算交集
	for (size_t i = 1; i < d; ++i) {
		// 计算当前结果和下一个 vector 的交集
		result = intersection(result, *set->owners[i]->query_res);
	}
	return result;
}