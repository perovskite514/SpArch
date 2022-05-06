#include<bits/stdc++.h>
using namespace std;
#define rep(i,n) for(ll i=0;i<n;i++)
#define repl(i,l,r) for(ll i=(l);i<(r);i++)
#define per(i,n) for(ll i=n-1;i>=0;i--)
#define perl(i,r,l) for(ll i=r-1;i>=l;i--)
#define fi first
#define se second
#define pb push_back
#define ins insert
#define all(x) (x).begin(),(x).end()
using ll=long long;
using vl=vector<ll>;
using vvl=vector<vector<ll>>;
const ll dy[8]={1,0,-1,0,1,1,-1,-1};
const ll dx[8]={0,-1,0,1,1,-1,1,-1};
using Graph = vector<vector<int>>;



class Merge_Unit
{
private:
public:
    
    vector<queue<pair<int, double>>> merge_tree;
    vector<int> merge_tree_inx;
    vector<bool> unit_available_flag;
    vector<queue<pair<int, double>>> DRAM;
    vector<int> DRAM_inx;
    vector<bool> DRAM_available_flag;
    vector<bool> output_available_flag;
    queue<int> DRAM_priority_inx;
    bool DRAM_flag;

    Merge_Unit(int n){
        init(n);
    }
 
    void init(int n){
        
        merge_tree.resize(8);
        merge_tree_inx.resize(8);
        unit_available_flag.resize(8, true);
        output_available_flag.resize(4, true);
        DRAM_flag = false;
        DRAM.resize(1000);
        DRAM_inx.resize(1000);
        DRAM_available_flag.resize(1000, true);

    }

    vector<pair<int, double>> low_comparator(vector<pair<int, double>> a, vector<pair<int, double>> b){
      
      	vector<pair<int, double>> output;
        int low_level_array[5][5] = {};
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                if(a[j].first > b[i].first){
                    low_level_array[i][j] = 1;
                }
                else{
                    low_level_array[i][j] = 0;
                }
            }
        }
        for(int i = 0; i < 4; i++){
            low_level_array[i][4] = 1;
        }

        int ii = 0;
        int jj = 0;
        int inx = 0;
    
        while(1){
            if(inx >= 8) break;
          	//cout << ii << " " << jj << endl;
            if(low_level_array[ii][jj] == 1){
                output.push_back(b[ii]);
                ii++;
            }
            else{
                output.push_back(a[jj]);
                jj++;
            }
            inx++;
        }
      
      	for(int j = 0; j < output.size(); j++){
        	//cout << output[j].first << " " << output[j].second << endl;
        }
        return output;
    }

    vector<pair<int, double>> comparator_array(bool &a_flag, bool &b_flag, queue<pair<int, double>> que1, queue<pair<int, double>> que2){

        vector<pair<int, double>> a;
        vector<pair<int, double>> b;
        int a_len = 12, b_len = 12;
        
        vector<pair<int, double>> a_debug;
        vector<pair<int, double>> b_debug;

        for(int i = 0; i < 12; i++){
            if(que1.size() != 0){
                if(que1.front().first == -2){
                    a_flag = false;
                    a_len = min(a_len, i);
                    a.push_back({1e9 + i, 0});
                }
                else{
                    a.push_back(que1.front());
                    a_debug.push_back(que1.front());
                    que1.pop();
                }
            }
            else{
                a_flag = false;
                a_len = min(a_len, i);
                a.push_back({1e9 + i, 0});
            }
            if(que2.size() != 0){
                if(que2.front().first == -2){
                    b_flag = false;
                    b_len = min(b_len, i);
                    b.push_back({2e9 + i, 0});
                }
                else{
                    b.push_back(que2.front());
                    b_debug.push_back(que2.front());
                    que2.pop();
                }
            }
            else{
                b_flag = false;
                b_len = min(b_len, i);
                b.push_back({2e9 + i, 0});
            }
        }

        int a_chunk_flag[3] = {};
        int b_chunk_flag[3] = {};
        if(a_len <= 4){
            a_chunk_flag[1] = 1;
            a_chunk_flag[2] = 1;
        }
        else if(a_len <= 8){
            a_chunk_flag[2] = 1;
        }
        if(b_len <= 4){
            b_chunk_flag[1] = 1;
            b_chunk_flag[2] = 1;
        }
        else if(b_len <= 8){
            b_chunk_flag[2] = 1;
        }

        pair<int, double> a_chunk_bouder[3] = {};
        pair<int, double> b_chunk_bouder[3] = {};
        for(int i = 0; i < 3; i++){
            a_chunk_bouder[i] = a[i * 4 + 3];
            b_chunk_bouder[i] = b[i * 4 + 3]; 
        }

        // top_level
        int top_level_array[4][4] = {};
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                if(a_chunk_bouder[j].first > b_chunk_bouder[i].first){
                    top_level_array[i][j] = 1;
                }
                else{
                    top_level_array[i][j] = 0;
                }
            }
        }
        for(int i = 0; i < 4; i++){
            top_level_array[i][3] = 1;
        }

        int top_level_output[5][5] = {};
        int ii = 0;
        int jj = 0;
        int inx = 0;

        while(1){
            if(ii >= 3 || jj >= 3 || inx >= 5) break;
            top_level_output[inx][0] = jj;
            top_level_output[inx][1] = ii;
            top_level_output[inx][2] = top_level_array[ii][jj];
            inx++;
            if(top_level_array[ii][jj] == 0){
                jj++;
            }
            else{
                ii++;
            }
        }
      
        // low_level
        vector<pair<int, double>> output;
        for(int i = 0; i < inx; i++){
            vector<pair<int, double>> a_low(4);
            vector<pair<int, double>> b_low(4);
            vector<pair<int, double>> low_output;
            int a_num = top_level_output[i][0];
            int b_num = top_level_output[i][1];
            for(int j = 0; j < 4; j++){
                a_low[j] = a[a_num * 4 + j];
                b_low[j] = b[b_num * 4 + j];
            }
            low_output = low_comparator(a_low, b_low);
          	for(int j = 0; j < low_output.size(); j++){
              //cout << low_output[j].first << " " << low_output[j].second << endl;
            }
            pair<int, double> last;
            int last_inx = 7;
            if(top_level_output[i][2] == 1){
                last = b_low[3];
            }
            else{
                last = a_low[3];
            }
            for(int j = 0; j < 8; j++){
                if(low_output[j] == last){
                    last_inx = j;
                    break;
                }
            }
          	if(i == inx - 1) last_inx = 7;
          	int pre_pos = -1;
          	int len = output.size();
          	if(len != 0){
              pre_pos = output[len - 1].first;
            }
          	//cout << pre_pos << endl;
            for(int j = 0; j <= last_inx; j++){
                if(j > last_inx) break;
                int pos = low_output[j].first;
                if(pos >= 1e9) break;
                double val = low_output[j].second;
                while(1){
                    j++;
                    if(j > last_inx) break;
                    if(pos != low_output[j].first) break;
                    val += low_output[j].second;
                }
                j--;
                if(pos > pre_pos) output.push_back({pos, val});
              	//if(pos > pre_pos) cout << pos << " " << val << " 1" << endl;
            }
        }
        
      	vector<pair<int, double>> output_comp;
      	int output_len = output.size();
      	for(int i = 0; i < output_len; i++){
            if(i > output_len) break;
            int pos = output[i].first;
            double val = output[i].second;
            while(1){
                i++;
                if(i > output_len) break;
                if(pos != output[i].first) break;
                val += output[i].second;
            }
            i--;
            output_comp.push_back({pos, val});
        }
      
        // debug code
        vector<pair<int, double>> low_output_debug;
        vector<pair<int, double>> output_debug;
      	int a_debug_len = a_debug.size();
      	int b_debug_len = b_debug.size();
        for(int i = 0; i < a_debug_len; i++) low_output_debug.push_back(a_debug[i]);
        for(int i = 0; i < b_debug_len; i++) low_output_debug.push_back(b_debug[i]);
        sort(low_output_debug.begin(), low_output_debug.end());
        int len = low_output_debug.size();
        for(int i = 0; i < len; i++){
            if(i > len) break;
                int pos = low_output_debug[i].first;
                double val = low_output_debug[i].second;
                while(1){
                    i++;
                    if(i > len) break;
                    if(pos != low_output_debug[i].first) break;
                    val += low_output_debug[i].second;
                }
                i--;
                output_debug.push_back({pos, val});
        }
      	
      	int output_comp_len = output_comp.size();
      	int len2 = output_debug.size();
        if(output_len != len2){
          cout << "size_error" << endl;
          exit(1);
        }
        for(int i = 0; i < output_comp_len; i++){
          	cout << output_comp[i].first << " " << output_comp[i].second << endl;
            if(output_comp[i] != output_debug[i]){
              cout << "error" << endl;
              exit(1);
            }
        }
      	for(int i = 0; i < len2; i++){
          	cout << output_debug[i].first << " " << output_debug[i].second << endl;
            //if(output_comp[i] != output_debug[i]) exit(1);
        }

        return output_comp;
    }

    void clock(bool &last_flag, vector<bool> outer_product_flag, vector<queue<pair<int, double>>> input_queue, vector<queue<int>> input_queue_inx){

        // 空いているleaf_nodeにinputをマッチングする
        for(int i = 4; i < 8; i++){
            if(unit_available_flag[i] == true){
                for(int j = 0; j < 1; j++){
                    if(output_available_flag[j] == true && input_queue[j].size() != 0){
                        merge_tree_inx[i] = j;
                        unit_available_flag[i] = false;
                        outer_product_flag[j] = false;
                        break;
                    }
                }
            }
        }

        // merge_tree
        for(int i = 0; i < 8; i++){
            if(i < 4){
                // 上位nodeからmerge
                bool a_flag = true;
                bool b_flag = true;
                vector<pair<int, double>> merge_result = comparator_array(a_flag, b_flag, merge_tree[2 * i], merge_tree[2 * i + 1]);
                int len = merge_result.size();
                for(int j = 0; j < len; j++){
                    merge_tree[i].push(merge_result[j]);
                }
                if(a_flag == false && b_flag == false){
                    // mergeが終わった。
                    merge_tree[i].push({-2, 0});
                    if(merge_tree[2 * i].size() != 0) merge_tree[2 * i].pop();
                    if(merge_tree[2 * i + 1].size() != 0) merge_tree[2 * i + 1].pop();
                }
            }
            else{
                // leaf_node
                if(unit_available_flag[i] == false){
                    int inx = merge_tree_inx[i];
                    if(input_queue_inx[inx].size() == 0) continue;
                    int len = input_queue_inx[inx].front();
                    input_queue_inx[inx].pop();
                    // inxがマッチングしているouterのinxでlenだけ取り出す
                    for(int j = 0; j < len; j++){
                        if(input_queue[inx].size() == 0){
                            cout << "queue_size_error" << endl;
                            exit(1);
                        }
                        pair<int, double> element = input_queue[inx].front();
                        input_queue[inx].pop();
                        merge_tree[i].push(element);
                    }
                    if(input_queue_inx[inx].size() != 0){
                        // 最後
                        if(input_queue_inx[inx].front() == -1){
                            merge_tree_inx[i] = -1;
                            unit_available_flag[i] = true;
                            outer_product_flag[inx] = true;
                            input_queue_inx[inx].pop();
                            merge_tree[i].push({-2, 0});
                        }
                    }
                }
            }
        }

        // root_node
        if(last_flag == true && DRAM_flag == true){
            // merge_tree内のmerge_resultが出力
        }

        // 空いているroot_nodeにrootのoutputをマッチングする
        bool match_flag = false;
        if(unit_available_flag[1] == true){
            for(int i = 4; i < 8; i++){
                if(unit_available_flag[i] == true){
                    if(merge_tree[1].size() != 0){
                        merge_tree_inx[i] = 1e9;
                        merge_tree_inx[1] = i;
                        unit_available_flag[1] = false;
                        unit_available_flag[i] = false;
                        match_flag = true;
                        break;
                    }
                }
            }
            if(match_flag == false){
                // DRAMに入れる
                
            }
        }

        if(unit_available_flag[1] == false){
            int inx = merge_tree_inx[1];
            for(int j = 0; j < 12; j++){
                if(merge_tree[1].size() == 0) break;
                pair<int, double> element = merge_tree[1].front();
                if(element.first == -2){
                    unit_available_flag[1] = true;
                    unit_available_flag[inx] = true;
                    merge_tree_inx[1] = -1;
                    merge_tree_inx[inx] = -1;
                    merge_tree[1].pop();
                    break;
                }
                merge_tree[inx].push(element);
            }
        }

        // DRAM
        return;
    }

};

int main(){

    queue<pair<int, double>> que1;
    queue<pair<int, double>> que2;
    vector<pair<int, double>> a = {{1, 0.1}, {3, 0.5}, {4, 0.2}, {13, 1.2}, {19, 5.1}, {22, -3.1}, {35, 1.2}, 
                                   {37, 9.2}, {42, 1.1}, {47, 9.9}, {48, 0.3}, {58, -10.8}};
    vector<pair<int, double>> b = {{3, 0.6}, {5, 1.3}, {10, 2.2}, {12, 1.1}, {15, 4.4}, {29, 3.9}, {35, -1.1}, 
                                   {40, 9.2}, {44, 7.1}, {52, -1.0}, {55, 0.2}, {61, 9.9}};          
  	int a_len = a.size();
  	int b_len = b.size();
    for(int i = 0; i < a_len; i++){
        que1.push(a[i]);
    }
    for(int i = 0; i < b_len; i++){
        que2.push(b[i]);
    }

    Merge_Unit Merge_Unit(10);
    vector<pair<int, double>> ans;
    ans = Merge_Unit.comparator_array(que1, que2);
  	int ans_len = ans.size();
    for(int i = 0; i < ans_len; i++){
        cout << ans[i].first << " " << ans[i].second << endl;
    }
}