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
	ofstream eg("zaredge.txt");
	
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
	
	int NNN = 50000;
	
	for (int i = 0; i < NNN; i++) {
		cout << i << endl;
		for (int j = 0; j < NNN; j++) {
			int tmp_dist = 0;
			for (int k = 0; k < 8; k++) {
				tmp_dist = tmp_dist + __popcnt64(data[i * 8 + k] ^ data[j * 8 + k]);
			}
			if (tmp_dist < neighbor_param) {
				eg << i << " " << j << endl;
			}
		}
	}
	
	
	
	
	return 0;
}