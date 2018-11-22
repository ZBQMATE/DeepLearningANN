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

using namespace std;

int main() {
	
	long long int *data = new long long int[60000 * 8]();
	ifstream iptf("DATA.txt");
	//ifstream CAA("CAA.txt");
	ofstream ACH("ANCHOR.txt");
	ofstream AWA("AWAY.txt");
	ofstream CLO("CLOSE.txt");
	
	int neighbor_param = 100; // parameter to determine neighbor
	
	
	
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
	/*
	// Food Version A
	for (int i = 0; i < 60000; i = i + 100) {
		cout << i << endl;
		for (int j = i + 100; j < 60000; j = j + 100) {
			for (int k = j + 100; k < 60000; k = k + 100) {
				int tmp_score_j = 0;
				int tmp_score_k = 0;
				for (int m = 0; m < 8; m++) {
					tmp_score_j = tmp_score_j + __popcnt64(data[i * 8 + m] ^ data[j * 8 + m]);
					tmp_score_k = tmp_score_k + __popcnt64(data[i * 8 + m] ^ data[k * 8 + m]);
				}
				if (tmp_score_j < tmp_score_k) {
					// j is close
					CAA << "1" << endl;
				}
				else {
					// k is close
					CAA << "0" << endl;
				}
			}
		}
	}
	*/
	
	// Food Version B
	int *idx = new int[50000]();
	int *idx_a = new int[50000]();
	int *idx_b = new int[50000]();
	for (int iter = 0; iter < 500; iter++){
		for (int i = 0; i < 50000; i++){
			idx[i] = rand() % 50000;
			idx_a[i] = rand() % 50000;
			idx_b[i] = rand() % 50000;
		}
		for (int j = 0; j < 50000; j++) {
			if ((idx[j] == idx_a[j] || idx[j] == idx_b[j]) || idx_a[j] == idx_b[j]) {
				cout << "Same datapoint here!" << endl;
				continue;
			}
			int tmp_score_a = 0;
			int tmp_score_b = 0;
			for (int m = 0; m < 8; m++) {
				tmp_score_a = tmp_score_a + __popcnt64(data[idx[j] * 8 + m] ^ data[idx_a[j] * 8 + m]);
				tmp_score_b = tmp_score_b + __popcnt64(data[idx[j] * 8 + m] ^ data[idx_b[j] * 8 + m]);
			}
			if (tmp_score_a < tmp_score_b) {
				ACH << idx[j] << endl;
				AWA << idx_b[j] << endl;
				CLO << idx_a[j] << endl;
			}
			else {
				ACH << idx[j] << endl;
				AWA << idx_a[j] << endl;
				CLO << idx_b[j] << endl;
			}
		}
	}
	
	
	
	
	return 0;
}