

/**
This is an implementation of LPP
2018 03 09

CL /I .\eigen\Eigen LPP.cpp

*/

#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>

#include <intrin.h>
#include <bitset>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <Eigen>

#include "KDTREE.cpp"


using namespace std;
using namespace Eigen;
using Eigen::MatrixXf;



int main() {
	
	long long int *data = new long long int[60000 * 8]();
	ifstream iptf("DATA.txt");
	ofstream rp("COMBINED_SEARCH_RESULT.txt");
	//ofstream ft("lpp_float.txt");
	//ofstream eg("zaredge.txt")
	
	int LPP_NUM_SAMPLE = 2000;
	int LPP_components = 20; // number of dimention the LLP function project to
	int LPP_neighbor_param = 150; // parameter to determine neighbor
	
	
	
	string aa;
	string bb;
	string cc;
	string dd;
	string ee;
	string ff;
	string gg;
	string hh;
	
	long long int aai;
	long long int bbi;
	long long int cci;
	long long int ddi;
	long long int eei;
	long long int ffi;
	long long int ggi;
	long long int hhi;
	
	long long int psss = 0;
	
	int base_count = 0;
	
	for (int yyy = 0; yyy < 60000; yyy++) {
		for (int ttt = 0; ttt < 8; ttt++) {
			getline(iptf, aa);
			getline(iptf, bb);
			getline(iptf, cc);
			getline(iptf, dd);
			getline(iptf, ee);
			getline(iptf, ff);
			getline(iptf, gg);
			getline(iptf, hh);
			
			aai = atoi(aa.c_str());
			bbi = atoi(bb.c_str());
			cci = atoi(cc.c_str());
			ddi = atoi(dd.c_str());
			eei = atoi(ee.c_str());
			ffi = atoi(ff.c_str());
			ggi = atoi(gg.c_str());
			hhi = atoi(hh.c_str());
			
			psss = aai << 56 | bbi << 48 | cci << 40 | ddi << 32 | eei << 24 | ffi << 16 | ggi << 8 | hhi;
			
			data[base_count] = psss;
			base_count++;
		}
	}
	
	cout << "MAKE RANDOM DATA BY EXCHANGE RANDOM DATA POINTS WITH THE LAST" << endl;
	
	for (int yyy = 0; yyy < 10000; yyy++) {
		for (int ttt = 0; ttt < 8; ttt++) {
			psss = data[yyy * 8 * 5 + ttt];
			data[yyy * 8 * 5 + ttt] = data[50000 * 8 + yyy * 8 + ttt];
			data[50000 * 8 + yyy * 8 + ttt] = psss;
		}
	}
	
	/**
	DEFINE MEASUREMENTS
	PROJECT DATASET AND QUERYSET SEPERATELY
	USE PROJECTED QUERYSET TO SEARCH NN FROM PROJECTED DATASET
	CHECK THE NN ACCURACY OF THAT NN ACQUIRED BY THAT SHORT CODE
	
	DATA USAGE
	THE FIRST 50000 DATA POINTS ARE DATASET, THE LAST 10000 ARE QUERYSET
	*/
	cout << "DO A HAND JOB" << endl;
	
	clock_t linear_start = clock();
	
	int *ground_truth = new int[10000]();
	
	for (int iii = 0; iii < 10000; iii++) {
		int tmp_best_id = 0;
		int tmp_best_score = 512;
		for (int jjj = 0; jjj < 50000; jjj++) {
			int tmp_cur_score = 0;
			for (int kkk = 0; kkk < 8; kkk++) {
				tmp_cur_score = tmp_cur_score + __popcnt64(data[50000 * 8 + iii * 8 + kkk] ^ data[jjj * 8 + kkk]);
			}
			if (tmp_cur_score < tmp_best_score) {
				tmp_best_id = jjj;
				tmp_best_score = tmp_cur_score;
			}
		}
		ground_truth[iii] = tmp_best_id;
		//cout << tmp_best_id << endl;
	}
	
	double linear_time = (double) (clock() - linear_start) / CLOCKS_PER_SEC;
	
	cout << "construct the adjency graph W and Laplacian matrix" << endl;
	
	cout << "extract training samples from data" << endl;
	long long int *LLP_sample = new long long int[LPP_NUM_SAMPLE * 8]();
	for (int iii = 0; iii < LPP_NUM_SAMPLE; iii++) {
		for (int jjj = 0; jjj < 8; jjj++) {
			LLP_sample[iii * 8 + jjj] = data[(iii * (50000 / LPP_NUM_SAMPLE) + 1) * 8 + jjj];
		}
	}
	
	MatrixXf LPP_sample_matrix_form(512, LPP_NUM_SAMPLE);
	for (int iii = 0; iii < LPP_NUM_SAMPLE; iii++) {
		for (int jjj = 0; jjj < 8; jjj++) {
			long long int tmp_probe = 1;
			long long int tmp_result = 0;
			
			for (int kkk = 0; kkk < 64; kkk++) {
				tmp_result = tmp_probe ^ LLP_sample[iii * 8 + jjj];
				if (tmp_result > LLP_sample[iii * 8 + jjj]) {
					LPP_sample_matrix_form(jjj * 64 + 63 - kkk, iii) = 0;
				}
				if (tmp_result < LLP_sample[iii * 8 + jjj]) {
					LPP_sample_matrix_form(jjj * 64 + 63 - kkk, iii) = 1;
				}
				
				tmp_probe = tmp_probe * 2;
			}
		}
	}
	
	MatrixXf DATA_matrix_form(512, 60000);
	for (int iii = 0; iii < 60000; iii++) {
		for (int jjj = 0; jjj < 8; jjj++) {
			long long int tmp_probe = 1;
			long long int tmp_result = 0;
			for (int kkk = 0; kkk < 64; kkk++) {
				tmp_result = tmp_probe ^ data[iii * 8 + jjj];
				if (tmp_result > data[iii * 8 + jjj]) {
					DATA_matrix_form(jjj * 64 + 63 - kkk, iii) = 0;
				}
				if (tmp_result < data[iii * 8 + jjj]) {
					DATA_matrix_form(jjj * 64 + 63 - kkk, iii) = 1;
				}
				tmp_probe = tmp_probe * 2;
			}
		}
	}
	
	cout << "build the adjacency graph W, a matrix of 0 and 1, set unit to 1 if the distance is smaller than LPP_neighbor_param" << endl;
	
	MatrixXf W(LPP_NUM_SAMPLE, LPP_NUM_SAMPLE);
	for (int iii = 0; iii < LPP_NUM_SAMPLE; iii++) {
		for (int jjj = iii; jjj < LPP_NUM_SAMPLE; jjj++) {
			int tmp_dist = 0;
			for (int kkk = 0; kkk < 8; kkk++) {
				tmp_dist = tmp_dist + __popcnt64(LLP_sample[iii * 8 + kkk] ^ LLP_sample[jjj * 8 + kkk]);
			}
			if (tmp_dist < LPP_neighbor_param) {
				W(iii, jjj) = 1;
				W(jjj, iii) = 1;
			}
			if (tmp_dist >= LPP_neighbor_param) {
				W(iii, jjj) = 0;
				W(jjj, iii) = 0;
			}
		}
	}
	
	MatrixXf D(LPP_NUM_SAMPLE, LPP_NUM_SAMPLE);
	for (int iii = 0; iii < LPP_NUM_SAMPLE; iii++) {
		int tmp_sum = 0;
		for (int jjj = 0; jjj < LPP_NUM_SAMPLE; jjj++) {
			tmp_sum = tmp_sum + W(jjj, iii);
		}
		D(iii, iii) = tmp_sum;
	}
	
	MatrixXf L(LPP_NUM_SAMPLE, LPP_NUM_SAMPLE);
	for (int iii = 0; iii < LPP_NUM_SAMPLE; iii++) {
		for (int jjj = 0; jjj < LPP_NUM_SAMPLE; jjj++) {
			L(iii, jjj) = D(iii, jjj) - W(iii, jjj);
		}
	}
	
	cout << "dealing the eigen map problem" << endl;
	
	cout << "compute T1 = XLX" << endl;
	MatrixXf XLX(512, 512);
	XLX = LPP_sample_matrix_form * L * LPP_sample_matrix_form.transpose();
	
	cout << "compute T2 = XDX" << endl;
	MatrixXf XDX(512, 512);
	XDX = LPP_sample_matrix_form * D * LPP_sample_matrix_form.transpose();
	
	cout << "compute the target matrix" << endl;
	MatrixXf TargetMat(512, 512);
	TargetMat = XDX.inverse() * XLX;
	
	cout << "compute the eigenvalues" << endl;
	
	SelfAdjointEigenSolver<MatrixXf> es(TargetMat);
	
	MatrixXf VC;
	MatrixXf VA;
	VC = es.eigenvalues();
	VA = es.eigenvectors();
	cout << VC << endl;
	
	MatrixXf a;
	a = VA.leftCols(LPP_components);
	
	cout << "do linear projection..." << endl;
	
	MatrixXf LPP_RESULT(LPP_components, 60000);
	LPP_RESULT = a.transpose() * DATA_matrix_form;
	
	/************  BUILD A KD TREE USING LPP RESULT  **************/
	
	float *LPP_RESULT_F = new float[LPP_components * 60000]();
	for (int i = 0; i < LPP_components; i++) {
		for (int j = 0; j < 60000; j++) {
			LPP_RESULT_F[j * LPP_components + i] = LPP_RESULT(i, j);
		}
	}
	/*
	for (int i = 0; i < 60000; i++) {
		for (int j = 0; j < LPP_components; j++) {
			ft << LPP_RESULT_F[i * LPP_components + j] << "  ";
		}
		ft << endl;
	}
	*/
	uint8_t *LPP_RESULT_I = new uint8_t[LPP_components * 60000]();
	float max_in_mtx = 0;
	float min_in_mtx = 0;
	for (int i = 0; i < LPP_components * 60000; i++) {
		if (max_in_mtx < LPP_RESULT_F[i]) {
			max_in_mtx = LPP_RESULT_F[i];
		}
		if (min_in_mtx > LPP_RESULT_F[i]) {
			min_in_mtx = LPP_RESULT_F[i];
		}
	}
	float gap_max_min = (max_in_mtx - min_in_mtx) / 256;
	for (int i = 0; i < LPP_components * 60000; i++) {
		LPP_RESULT_I[i] = (int) (LPP_RESULT_F[i] - min_in_mtx) / gap_max_min;
	}
	
	
	int branch_factor = 16;
	int leaf_size = 100;
	int len_kdtree = 1;
	int index_kdtree_bank = 0;
	int num_hct = 1;
	int *kdtree = new int[2000000]();
	int *kdtree_bank = new int[2000000]();
	int *idx_arr = new int[50000]();
	for (int iii = 0; iii < 50000; iii++) {
		idx_arr[iii] = iii;
	}
	int *combined_search_result = new int[10000]();
	
	KDTREE_BUILD(0, 0, LPP_RESULT_I, LPP_components, kdtree, kdtree_bank, idx_arr, 50000, branch_factor, leaf_size, len_kdtree, index_kdtree_bank);
	
	//cout << len_kdtree << endl;
	
	clock_t combined_start = clock();
	
	COMBINED_SEARCH(data, LPP_RESULT_I, LPP_components, 50000, 10000, num_hct, kdtree, kdtree_bank, len_kdtree, index_kdtree_bank, branch_factor, leaf_size, combined_search_result);
	//COMBINED_SEARCH(long long int *data, long long int *query, int data_len, int z, int num_hct, int *hct_tree_node, int *hct_bank_tree_node, int *flag_lin, int *hct_PQ, int *hct_PQ_score, int *hct_R, int *hct_len_pq, int &hct_len_r, int *hct_root_id, int &len_kdtree, int &index_kdtree_bank, int *R, int &len_r)
	
	double combined_time = (double) (clock() - combined_start) / CLOCKS_PER_SEC;
	
	// compare results
	int combined_search_score = 0;
	for (int i = 0; i < 10000; i++) {
		if (ground_truth[i] == combined_search_result[i]) {
			combined_search_score++;
		}
	}
	
	rp << "combined search score" << endl;
	rp << combined_search_score << endl;
	
	rp << "linear search time   ";
	rp << linear_time << endl;
	
	rp << "combined search time   ";
	rp << combined_time;
	
	
	
	/**
	cout << "SEARCH THE LPP RESULT..." << endl;
	int *LPP_search_result = new int[10000]();
	for (int iii = 0; iii < 10000; iii++) {
		cout << iii << endl;
		int tmp_best_id = 0;
		int tmp_best_score = 10000000;
		for (int jjj = 0; jjj < 50000; jjj++) {
			int tmp_cur_score = 0;
			for (int kkk = 0; kkk < LPP_components; kkk++) {
				tmp_cur_score = tmp_cur_score + (LPP_RESULT(kkk, jjj) - LPP_RESULT(kkk, 50000 + iii)) * (LPP_RESULT(kkk, jjj) - LPP_RESULT(kkk, 50000 + iii));
			}
			if (tmp_cur_score < tmp_best_score) {
				tmp_best_id = jjj;
				tmp_best_score = tmp_cur_score;
			}
		}
		LPP_search_result[iii] = tmp_best_id;
	}
	
	int score = 0;
	for (int iii = 0; iii < 10000; iii++) {
		if (ground_truth[iii] == LPP_search_result[iii]) {
			score++;
		}
	}
	rp << "REAL SEARCH SCORE.." << endl;
	rp << score << endl;
	
	cout << "SEARCH THE BINARY LPP RESULT..." << endl;
	
	int *LPP_binary_search_result = new int[10000]();
	
	float LPP_mean = 0;
	for (int iii = 0; iii < 60000; iii++) {
		for (int jjj = 0; jjj < LPP_components; jjj++) {
			LPP_mean = LPP_mean + LPP_RESULT(jjj, iii);
		}
	}
	LPP_mean = LPP_mean / (60000 * LPP_components);
	
	for (int iii = 0; iii < 60000; iii++) {
		for (int jjj = 0; jjj < LPP_components; jjj++) {
			if (LPP_RESULT(jjj, iii) >= LPP_mean) {
				LPP_RESULT(jjj, iii) = 1;
			}
			if (LPP_RESULT(jjj, iii) < LPP_mean) {
				LPP_RESULT(jjj, iii) = 0;
			}
		}
	}
	
	for (int iii = 0; iii < 10000; iii++) {
		cout << iii << endl;
		int tmp_best_id = 0;
		int tmp_best_score = 0;
		for (int jjj = 0; jjj < 50000; jjj++) {
			int tmp_cur_score = 0;
			for (int kkk = 0; kkk < LPP_components; kkk++) {
				if (LPP_RESULT(kkk, jjj) == LPP_RESULT(kkk, 50000 + iii)) {
					tmp_cur_score++;
				}
			}
			if (tmp_cur_score > tmp_best_score) {
				tmp_best_id = jjj;
				tmp_best_score = tmp_cur_score;
			}
		}
		LPP_binary_search_result[iii] = tmp_best_id;
	}
	
	int bi_score = 0;
	for (int iii = 0; iii < 10000; iii++) {
		if (LPP_binary_search_result[iii] == ground_truth[iii]) {
			bi_score++;
		}
	}
	
	rp << "BINARY SEARCH SCORE..." << endl;
	rp << bi_score << endl;
	*/
	
	return 0;
}