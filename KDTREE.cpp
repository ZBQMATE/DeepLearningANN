using namespace std;

void KDTREE_BUILD(int assed, int cat, uint8_t *this_data_idx, int data_dim, int *hct_tree_node, int *hct_bank_tree_node, int *cur_idx, int cur_len, int hct_bf, int hct_leaf_size, int &len_kdtree, int &index_kdtree_bank) {
	
	//KDTREE_BUILD(0, 0, LPP_RESULT, LPP_components, kdtree, kdtree_bank, idx_arr, 50000, branch_factor, leaf_size, len_kdtree, index_kdtree_bank);
	
	if (cur_len < hct_leaf_size) {
		
		cout << "leaf size -- ";
		cout << cur_len << endl;
		
		hct_tree_node[assed * 5 + 0] = 1;
		hct_tree_node[assed * 5 + 1] = cat;
		hct_tree_node[assed * 5 + 2] = index_kdtree_bank;
		hct_tree_node[assed * 5 + 3] = cur_len;
		hct_tree_node[assed * 5 + 4] = index_kdtree_bank + cur_len;
		
		for (int i = 0; i < cur_len; i++) {
			hct_bank_tree_node[index_kdtree_bank + i] = cur_idx[i];
		}
		index_kdtree_bank = index_kdtree_bank + cur_len;
		return;
	}
	
	///\\\\\\\\\/////\\\\\////////\\\\\///////
	
	int *rand_bf = new int[hct_bf]();
	int tmp_flag = 0;
	int i = 0;
	for (i = 0; i < hct_bf; i++) {
		int mk_rand = rand() % cur_len;
		for (int j = 0; j < i; j++) {
			if (rand_bf[j] == mk_rand) {
				tmp_flag = 1;
			}
		}
		if (tmp_flag == 0) {
			rand_bf[i] = mk_rand;
		}
		if (tmp_flag == 1) {
			i = i - 1;
		}
		tmp_flag = 0;
	}
	
	// record the centroids
	int *idx_to_data = new int[hct_bf]();
	for (int j = 0; j < hct_bf; j++) {
		idx_to_data[j] = cur_idx[rand_bf[j]];
	}
	int total_len = (1 + cur_len) * hct_bf;
	int *searched_idx = new int[total_len]();
	int *cnt = new int[hct_bf](); // counter of searched_idx
	int cur_best = 99999999;
	int id_cur_best = 0;
	int dist_a = 0;
	//bitset<512> diff_a;
	for (int i = 0; i < cur_len; i++) {
		for (int j = 0; j < hct_bf; j++) {
			/**
			diff_a = *(this_data_idx + idx_to_data[j]) ^ *(this_data_idx + cur_idx[i]);
			for (int k = 0; k < 512; k++) {
				if (diff_a[k]) {
					dist_a++;
				}
			}
			*/
			
			/**
			for (int v = 0; v < 8; v++) {
				dist_a = dist_a + __popcnt64(this_data_idx[idx_to_data[j] * 8 + v] ^ this_data_idx[cur_idx[i] * 8 + v]);
			}
			*/
			
			for (int v = 0; v < data_dim; v++) {
				dist_a = dist_a + (int)(this_data_idx[v + idx_to_data[j] * data_dim] -  this_data_idx[v + cur_idx[i] * data_dim]) * (this_data_idx[v + idx_to_data[j] * data_dim] - this_data_idx[v + cur_idx[i] * data_dim]);
			}
			
			
			if (cur_best > dist_a) {
				cur_best = dist_a;
				id_cur_best = j;
			}
			dist_a = 0;
		}
		searched_idx[id_cur_best * (cur_len + 1) + cnt[id_cur_best]] = i;
		cnt[id_cur_best] = cnt[id_cur_best] + 1;
		id_cur_best = 0;
		cur_best = 99999999;
	}
	hct_tree_node[assed * 5 + 0] = 0;
	hct_tree_node[assed * 5 + 1] = cat;
	hct_tree_node[assed * 5 + 2] = index_kdtree_bank;
	hct_tree_node[assed * 5 + 3] = hct_bf;
	hct_tree_node[assed * 5 + 4] = index_kdtree_bank + hct_bf;
	for (int i = 0; i < hct_bf; i++) {
		hct_bank_tree_node[index_kdtree_bank + i] = len_kdtree + i;
	}
	index_kdtree_bank = index_kdtree_bank + hct_bf;
	int record_tree_len = len_kdtree;
	len_kdtree = len_kdtree + hct_bf;
	for (int i = 0; i < hct_bf; i++) {
		int *t = new int[cnt[i]]();
		for (int j = 0; j < cnt[i]; j++) {
			t[j] = cur_idx[searched_idx[i * (cur_len + 1) + j]];
		}
		KDTREE_BUILD(record_tree_len + i, idx_to_data[i], this_data_idx, data_dim, hct_tree_node, hct_bank_tree_node, t, cnt[i], hct_bf, hct_leaf_size, len_kdtree, index_kdtree_bank);
		delete[] t;
	}
	return;
}

//KDTREE_TRAVERSE(LPP_RESULT, branch_factor, z, hct_tree_node, hct_bank_tree_node, PQ, PQ_score, R, PQ_len, R_len, data_len);
void KDTREE_TRAVERSE(int nnn, uint8_t *LPP_RESULT, int data_dim, int hct_bf, int zzz, int *hct_tree_node, int *hct_bank_tree_node, int *hct_PQ, int *hct_PQ_score, int *hct_R, int &hct_len_pq, int &hct_len_r, int data_len) {
//void KDTREE_TRAVERSE(long long int *this_data_idx, long long int *query_idx, int nnn, int hct_bf, int zzz, int *hct_tree_node, int *hct_bank_tree_node, int *hct_PQ, int *hct_PQ_score, int *hct_R, int *hct_len_pq, int &hct_len_r, int *flag_lin, int ith_tree, int data_len) {
	
	//bitset<512> diff_this;
	int cur_best = 512;
	int id_cur_best = 0;
	int tmp_cnt = 0;
	// if bottom layer
	if (hct_tree_node[nnn * 5 + 0] == 1) {
		for (int j = hct_tree_node[nnn * 5 + 2]; j < hct_tree_node[nnn * 5 + 4]; j++) {
			//if (flag_lin[hct_bank_tree_node[j]] == 0) {
			hct_R[hct_len_r] = hct_bank_tree_node[j];
			hct_len_r = hct_len_r + 1;
			//}
		}
	}
	if (hct_tree_node[nnn * 5 + 0] == 0) {
		// C is the cat set
		int *C = new int[hct_bf]();
		for (int i = 0; i < hct_bf; i++) {
			int id_next = hct_bank_tree_node[hct_tree_node[nnn * 5 + 2] + i];
			C[i] = hct_tree_node[id_next * 5 + 1];
		}
		
		int *score_C = new int[hct_bf]();
		for (int i = 0; i < hct_bf; i++) {
			/**
			diff_this = *(this_data_idx + C[i]) ^ *(query_idx + zzz);
			for (int j = 0; j < 512; j++) {
				if (diff_this[j]) {
					tmp_cnt++;
				}
			}
			
			for (int v = 0; v < 8; v++) {
				tmp_cnt = tmp_cnt + __popcnt64(this_data_idx[C[i] * 8 + v] ^ query_idx[zzz * 8 + v]);
			}
			*/
			
			for (int v = 0; v < data_dim; v++) {
				tmp_cnt = tmp_cnt + (int)(LPP_RESULT[v + C[i] * data_dim] - LPP_RESULT[v + (data_len + zzz) * data_dim]) * (LPP_RESULT[v + C[i] * data_dim] - LPP_RESULT[v + (data_len + zzz) * data_dim]);
				
			}
			
			score_C[i] = tmp_cnt;
			tmp_cnt = 0;
		}
		
		int score_rcd = 99999999;
		int id_rcd = 0;
		for (int i = 0; i < hct_bf; i++) {
			if (score_C[i] < score_rcd) {
				score_rcd = score_C[i];
				id_rcd = i;
			}
		}
		// cq is the id in hct_tree_node
		int cq = hct_bank_tree_node[hct_tree_node[nnn * 5 + 2] + id_rcd];
		int *pq_rcd = new int[hct_bf - 1]();
		int *pq_score_rcd = new int[hct_bf - 1]();
		int counter_tmp = 0;
		for (int i = 0; i < hct_bf; i++) {
			if (i != id_rcd) {
				pq_rcd[counter_tmp] = hct_bank_tree_node[hct_tree_node[nnn * 5 + 2] + i];
				pq_score_rcd[counter_tmp] = score_C[i];
				counter_tmp++;
			}
		}
		for (int i = 0; i < hct_bf - 1; i++) {
			hct_PQ[hct_len_pq + i] = pq_rcd[i];
			hct_PQ_score[hct_len_pq + i] = pq_score_rcd[i];
		}
		hct_len_pq = hct_len_pq + hct_bf - 1;
		//KDTREE_TRAVERSE(this_data_idx, query_idx, cq, hct_bf, zzz, hct_tree_node, hct_bank_tree_node, hct_PQ, hct_PQ_score, hct_R, hct_len_pq, hct_len_r, flag_lin, ith_tree, data_len);
		//KDTREE_TRAVERSE(int nnn, MatrixXf *LPP_RESULT, int data_dim, int hct_bf, int zzz, int *hct_tree_node, int *hct_bank_tree_node, int *hct_PQ, double *hct_PQ_score, int *hct_R, int *hct_len_pq, int &hct_len_r, int data_len)
		KDTREE_TRAVERSE(cq, LPP_RESULT, data_dim, hct_bf, zzz, hct_tree_node, hct_bank_tree_node, hct_PQ, hct_PQ_score, hct_R, hct_len_pq, hct_len_r, data_len);
	}
	return;
}

//void POP_MIN(int *hct_PQ, int *hct_PQ_score, int *hct_len_pq, int num_hct, int data_len, int &poped_id, int &poped_tree) {
void POP_MIN(int *hct_PQ, int *hct_PQ_score, int &hct_len_pq, int &poped_id) {
	int tmp_best = 99999999;
	int tmp_id = 0;
	//int at_layer = 0;
	
	//for (int i = 0; i < num_hct; i++) {
	for (int j = 0; j < hct_len_pq; j++) {
		if (hct_PQ_score[j] <= tmp_best) {
			tmp_best = hct_PQ_score[j];
			tmp_id = j;
			
			//at_layer = i;
		}
		//cout << hct_PQ_score[j] << endl;
	}
	//}
	
	poped_id = hct_PQ[tmp_id];
	//poped_tree = at_layer;
	
	hct_PQ[tmp_id] = hct_PQ[hct_len_pq - 1];
	hct_PQ_score[tmp_id] = hct_PQ_score[hct_len_pq - 1];
	hct_PQ[hct_len_pq - 1] = 0;
	hct_PQ_score[hct_len_pq - 1] = 0;
	hct_len_pq = hct_len_pq - 1;
	return;
}

//void COMBINED_SEARCH(long long int *data, long long int *query, int data_len, int z, int num_hct, int *hct_tree_node, int *hct_bank_tree_node, int *flag_lin, int *hct_PQ, int *hct_PQ_score, int *hct_R, int *hct_len_pq, int &hct_len_r, int *hct_root_id, int &len_kdtree, int &index_kdtree_bank, int *R, int &len_r) {

void COMBINED_SEARCH(long long int *data, uint8_t *LPP_RESULT, int data_dim, int data_len, int query_len, int num_hct, int *hct_tree_node, int *hct_bank_tree_node, int &len_kdtree, int &index_kdtree_bank, int branch_factor, int leaf_size, int *combined_search_result) {
	
	//COMBINED_SEARCH(data, LPP_RESULT, 50000, 10000, num_hct, kdtree, kdtree_bank, len_kdtree, index_kdtree_bank, int branch_factor, int leaf_size);
	
	
	
	cout << "using hct..." << endl;
	
	int *PQ = new int[2000000]();
	int *PQ_score = new int[2000000]();
	int *R = new int[2000000]();
	int *R_score = new int[2000000]();
	int PQ_len = 0;
	int R_len = 0;
	
	for (int z = 0; z < query_len; z++) {
		
		int linear_job = 5000;
		
		for (int i = 0; i < PQ_len; i++) {
			PQ[i] = 0;
			PQ_score[i] = 0;
		}
		for (int i = 0; i < R_len; i++) {
			R[i] = 0;
			R_score[i] = 0;
		}
		
		PQ_len = 0;
		R_len = 0;
		
		//cout << "dog" << endl;
		//KDTREE_TRAVERSE(data, query, hct_root_id[i], hct_branch_factor, z, hct_tree_node, hct_bank_tree_node, hct_PQ, hct_PQ_score, hct_R, hct_len_pq, hct_len_r, flag_lin, i, data_len);
		KDTREE_TRAVERSE(0, LPP_RESULT, data_dim, branch_factor, z, hct_tree_node, hct_bank_tree_node, PQ, PQ_score, R, PQ_len, R_len, data_len);
		//cout << "cat" << endl;
		int poped_id = 0;
		while (R_len < linear_job) {
			//cout << R_len << endl;
			POP_MIN(PQ, PQ_score, PQ_len, poped_id);
			//cout << poped_id << endl;
			//KDTREE_TRAVERSE(int nnn, float *LPP_RESULT, int data_dim, int hct_bf, int zzz, int *hct_tree_node, int *hct_bank_tree_node, int *hct_PQ, double *hct_PQ_score, int *hct_R, int &hct_len_pq, int &hct_len_r, int data_len)
			KDTREE_TRAVERSE(poped_id, LPP_RESULT, data_dim, branch_factor, z, hct_tree_node, hct_bank_tree_node, PQ, PQ_score, R, PQ_len, R_len, data_len);
		}
		//cout << "pig" << endl;
		// do linear search
		
		
		for (int i = 0; i < R_len; i++) {
			int cur_score = 0;
			for (int j = 0; j < 8; j++) {
				cur_score = cur_score + __popcnt64(data[R[i] * 8 + j] ^ data[(data_len + z) * 8 + j]);
			}
			R_score[i] = cur_score;
			//cout << cur_score << endl;
		}
		
		int bst_id = 0;
		int bst_score = 512;
		for (int i = 0; i < R_len; i++) {
			if (R_score[i] < bst_score) {
				bst_score = R_score[i];
				bst_id = R[i];
			}
		}
		
		combined_search_result[z] = bst_id;
		
		//delete [] PQ;
		//delete [] PQ_score;
		//delete [] R;
		//delete [] R_score;
		//cout << bst_id << endl;
	}
	
	/**
	if (z == 0) {
		// build the tree
		int *idx_arr = new int[data_len]();
		for (int i = 0; i < data_len; i++) {
			idx_arr[i] = i;
		}
		int tmp_root = 0;
		for (int i = 0; i < num_hct; i++) {
			cout << "build hct..." << endl;
			
			hct_root_id[i] = tmp_root;
			len_kdtree = len_kdtree + 1;
			KDTREE_BUILD(tmp_root, 0, data, data_dim, hct_tree_node, hct_bank_tree_node, idx_arr, data_len, hct_branch_factor, leaf_size, len_kdtree, index_kdtree_bank);
			tmp_root = len_kdtree;
		}
	}
	
	
	// clear
	for (int i = 0; i < num_hct; i++) {
		for (int j = 0; j < hct_len_pq[i]; j++) {
			hct_PQ[i * data_len + j] = 0;
		}
		for (int j = 0; j < hct_len_r; j++) {
			hct_R[j] = 0;
		}
		hct_len_pq[i] = 0;
		hct_len_r = 0;
	}
	
	
	// calculate the number for linear searching
	int total_outlier = 0;
	for (int i = 0; i < data_len; i++) {
		if (flag_lin[i] == 0) {
			total_outlier++;
		}
	}
	int max_exam = int(linear_job * total_outlier);
	*/
	
	/*
	for (int i = 0; i < num_hct; i++) {
		KDTREE_TRAVERSE(data, query, hct_root_id[i], hct_branch_factor, z, hct_tree_node, hct_bank_tree_node, hct_PQ, hct_PQ_score, hct_R, hct_len_pq, hct_len_r, flag_lin, i, data_len);
	}
	
	int poped_id = 0;
	int poped_tree = 0;
	while (hct_len_r < max_exam) {
		hct_popmin(hct_PQ, hct_PQ_score, hct_len_pq, num_hct, data_len, poped_id, poped_tree);
		KDTREE_TRAVERSE(data, query, poped_id, hct_branch_factor, z, hct_tree_node, hct_bank_tree_node, hct_PQ, hct_PQ_score, hct_R, hct_len_pq, hct_len_r, flag_lin, poped_tree, data_len);
	}
	for (int i = 0; i < hct_len_r; i++) {
		checkin(hct_R[i], 0, len_r, R);
	}
	*/
	return;
}
