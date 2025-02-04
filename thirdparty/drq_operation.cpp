#include "openssl/bn.h"
#include "utils.h"
#include "drq_data_structure.h"
#include "drq_operation.h"
#include "sstream"
#include "fstream"
#include "iostream"
using namespace std;



// 初始化查询范围
RESULT drq_init_query_range(query_range* ranges, int d) {
	ranges->d = d;
	ranges->range = (BIGNUM***)malloc(sizeof(BIGNUM**) * d);

	for (int i = 0; i < d; ++i) {
		ranges->range[i] = (BIGNUM**)malloc(sizeof(BIGNUM*) * 2);
		ranges->range[i][0] = BN_new();
		ranges->range[i][1] = BN_new();
	}

	return SUCCESS;
}
// 读取查询范围
RESULT drq_read_query_range(query_range* ranges, const char* queryFile) {
	ifstream infile(queryFile);
	if (!infile.is_open()) {
		cerr << "Error opening file: " << queryFile << endl;
		return ERROR;
	}


	string line;
	for (int i = 0; i < ranges->d; ++i) {
		if (!getline(infile, line)) {
			cerr << "Error reading line " << i + 1 << " from file." << endl;
			return ERROR;
		}

		istringstream iss(line);
		string left_str, right_str;

		if (!(iss >> left_str >> right_str)) {
			cerr << "Error parsing line " << i + 1 << " from file." << endl;
			return ERROR;
		}
		if (!BN_dec2bn(&ranges->range[i][0], left_str.c_str()) ||
			!BN_dec2bn(&ranges->range[i][1], right_str.c_str())) {
			cerr << "Error converting strings to BIGNUMs on line " << i + 1 << "." << endl;
			return ERROR;
		}
	}

	infile.close();
	return SUCCESS;
}
// 将不同维度的查询范围发送过去，获取查询的结果
RESULT drq_send_range2owner(query_range* ranges, drq_data_set* set) {
	// 将具体的查询维度分发过去
	int d = ranges->d;
	for (int i = 0; i < d; ++i) {
		// 对应的第i个维度的范围发送给对应的数据
		set->owners[i]->query_range = ranges->range[i];
	}
	return SUCCESS;
}
// 释放query_range
RESULT drq_free_query_range(query_range* ranges) {
	int d = ranges->d;
	for (int i = 0; i < d; ++i) {
		BN_free(ranges->range[i][0]);
		BN_free(ranges->range[i][1]);
		free(ranges->range[i]);
	}
	free(ranges->range);
	return SUCCESS;
}


// 将结果写入到文件中去
RESULT drq_write_res2File(vector<int>& ve, query_range& range, const char* resultFilePath) {
	FILE* file = fopen(resultFilePath, "w");
	if (file == nullptr) {
		perror("Error opening file");
		return ERROR;
	}

	// 写入 range 的范围
	fprintf(file, "Current scope:\n");
	int d = range.d;
	for (int i = 0; i < d; ++i) {
		char* str1 = BN_bn2dec(range.range[i][0]);
		char* str2 = BN_bn2dec(range.range[i][1]);
		fprintf(file, "dim[%d]:%s %s\n", i, str1, str2);
	}

	// 写入 drq_data_set 的成员值
	fprintf(file, "The file containing the scope is:\n");
	size_t len = ve.size();
	for (int i = 0; i < len; ++i) {
		fprintf(file, "%d ", ve[i]);
	}
	// 如果有更多成员，继续写入
	fprintf(file, "\n");
	fclose(file);
	return SUCCESS;
}