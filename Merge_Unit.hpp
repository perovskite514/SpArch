#include<bits/stdc++.h>
using namespace std;

class Merge_Unit
{
private:
    int merge_tree_size = 32;
    int DRAM_size = 1024;

    int inx1 = 0;
    int inx2 = 0;
    int inx3 = 0;
    bool flag1 = true;
    bool flag2 = true;
    bool flag3 = false;

    int DRAM_cycle = 60;
    int DRAM_cycle_cnt = 0;
    queue<pair<int, double>> buffer;
public:
    
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> haffuman_tree_scheduler;

    vector<queue<pair<int, double>>> merge_tree;
    vector<int> merge_tree_inx;
    vector<int> unit_available_flag;

    vector<queue<pair<int, double>>> DRAM;
    vector<int> DRAM_inx;
    vector<int> DRAM_available_flag;

    queue<pair<int, double>> output;
    vector<bool> output_available_flag;
    int unit_inx;
    bool DRAM_flag;
    bool output_flag;
    bool merge_flag;

    Merge_Unit(int n){
        init(n);
    }
 
    void init(int n){
        
        merge_tree.resize(merge_tree_size);
        merge_tree_inx.resize(merge_tree_size);
        unit_available_flag.resize(merge_tree_size, 0); 

        DRAM.resize(DRAM_size);
        DRAM_inx.resize(DRAM_size);
        DRAM_available_flag.resize(DRAM_size, 0);
        
        output_available_flag.resize(4, true);
        DRAM_flag = true;
        output_flag = false;
        unit_inx = 0;
        merge_flag = false;
    }

    vector<pair<int, double>> low_comparator(vector<pair<int, double>> a, vector<pair<int, double>> b, vector<bool> &a_or_b_flag){
      
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
            if(low_level_array[ii][jj] == 1){
                output.push_back(b[ii]);
                a_or_b_flag.push_back(false);
                ii++;
            }
            else{
                output.push_back(a[jj]);
                a_or_b_flag.push_back(true);
                jj++;
            }
            inx++;
        }
      
        return output;
    }

    vector<pair<int, double>> comparator_array(int clock, bool &a_flag, bool &b_flag, queue<pair<int, double>> que1, queue<pair<int, double>> que2, vector<bool> &a_or_b_flag){

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
                    a.push_back({1e9 + i, 1e9});
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
                a.push_back({1e9 + i, 1e9});
            }
            if(que2.size() != 0){
                if(que2.front().first == -2){
                    b_flag = false;
                    b_len = min(b_len, i);
                    b.push_back({2e9 + i, 1e9});
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
                b.push_back({2e9 + i, 1e9});
            }
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
            vector<bool> a_or_b;
            low_output = low_comparator(a_low, b_low, a_or_b);
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
          	int pre_pos = -1;
          	int len = output.size();
          	if(len != 0){
              pre_pos = output[len - 1].first;
            }
            for(int j = 0; j <= last_inx; j++){
                if(j > last_inx) break;
                int pos = low_output[j].first;
                if(pos >= 1e9) break;
                if(pos > pre_pos) a_or_b_flag.push_back(a_or_b[j]);
                double val = low_output[j].second;
                if(pos > pre_pos) output.push_back({pos, val});
            }
        }

        int a_or_b_len = a_or_b_flag.size();
        
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

        return output_comp;
    }

    void clock(int clock, bool &last_flag, vector<bool> outer_product_flag, vector<queue<pair<int, double>>> &input_queue, vector<queue<int>> &input_queue_inx){

        for(int i = 0; i < merge_tree_size; i++){
            int len = merge_tree[i].size();
            int pos = -1;
            for(int j = 0; j < len; j++){
                auto element = merge_tree[i].front();
                merge_tree[i].pop();
                if(element.first != pos) merge_tree[i].push(element);
                pos = element.first;
            }
        }
        
        if(output_flag) return;
        
        // 空いているleaf_nodeにinputをマッチングする
        for(int j = 0; j < 1; j++){
            if(output_available_flag[j] == true && input_queue[j].size() != 0){
                bool matching_flag = false;
                for(int i = 0; i < merge_tree_size; i++){
                    if(unit_available_flag[i] == 0 && merge_tree[i].size() == 0){
                        merge_tree_inx[i] = j;
                        unit_available_flag[i] = 1;
                        //cout << "Matching " << j << " " << i << endl;
                        output_available_flag[j] = false;
                        matching_flag = true;
                        break;
                    }
                }
                if(matching_flag == false){
                    for(int i = 0; i < DRAM_size; i++){
                        if(DRAM_available_flag[i] == 0 && DRAM[i].size() == 0){
                            DRAM_inx[i] = j;
                            DRAM_available_flag[i] = 1;
                            //cout << "DRAM_Matching " << j << " " << i << " " << clock << endl;
                            output_available_flag[j] = false;
                            break;
                        }
                    }   
                }
            }    
        }

        if(merge_flag == true || haffuman_tree_scheduler.size() >= 2){

            if(merge_flag == false){
                auto merge1 = haffuman_tree_scheduler.top();
                haffuman_tree_scheduler.pop();
                auto merge2 = haffuman_tree_scheduler.top();
                haffuman_tree_scheduler.pop();
                merge_flag = true;
                inx1 = merge1.second;
                inx2 = merge2.second;
                inx3 = 0;
                flag1 = true;
                flag2 = true;
                flag3 = false;
                if(inx1 >= merge_tree_size){
                    inx1 -= merge_tree_size;
                    flag1 = false;
                }
                if(inx2 >= merge_tree_size){
                    inx2 -= merge_tree_size;
                    flag2 = false;
                }
                if(flag1){
                    flag3 = true;
                    inx3 = inx1;
                }
                else if(flag2){
                    flag3 = true;
                    inx3 = inx2;
                }
                else{
                    for(int i = 0; i < merge_tree_size; i++){
                        if(unit_available_flag[i] == 0 && merge_tree[i].size() == 0){
                            inx3 = i;
                            flag3 = true;
                            unit_available_flag[i] = 1;
                            break;
                        }
                    }
                    if(flag3 == false){
                        // (DRAM, DRAM) --> DRAM
                        for(int i = 0; i < DRAM_size; i++){
                            if(DRAM_available_flag[i] == 0 && DRAM[i].size() == 0){
                                DRAM_available_flag[i] = 1;
                                inx3 = i;
                                break;
                            }
                        }
                    }
                }
            }
            
            if((flag1 | flag2 | flag3) == false){
                DRAM_cycle_cnt++;
                cout << DRAM_cycle_cnt << endl;
                if(DRAM_cycle_cnt < DRAM_cycle_cnt){
                    goto skip;
                }
            }

            bool a_flag = true;
            bool b_flag = true;
            vector<bool> a_or_b_flag;
            vector<pair<int, double>> merge_result;
            if(flag1 == true && flag2 == true){
                merge_result = comparator_array(clock, a_flag, b_flag, merge_tree[inx1], merge_tree[inx2], a_or_b_flag);
            }
            else if(flag1 == true && flag2 == false){
                merge_result = comparator_array(clock, a_flag, b_flag, merge_tree[inx1], DRAM[inx2], a_or_b_flag);
            }
            else if(flag1 == false && flag2 == true){
                merge_result = comparator_array(clock, a_flag, b_flag, DRAM[inx1], merge_tree[inx2], a_or_b_flag);
            }
            else{
                merge_result = comparator_array(clock, a_flag, b_flag, DRAM[inx1], DRAM[inx2], a_or_b_flag);
            }

            int len = merge_result.size();
            for(int j = 0; j < len; j++){
                buffer.push(merge_result[j]);
            }
            
            for(int j = 0; j < a_or_b_flag.size(); j++){
                if(a_or_b_flag[j]){
                    if(flag1 == true) merge_tree[inx1].pop();
                    else DRAM[inx1].pop();
                }
                else{
                    if(flag2 == true) merge_tree[inx2].pop();
                    else DRAM[inx2].pop();
                }
            }

            if(a_flag == false && b_flag == false){
                // merge end
                int weight = 0;
                int len = buffer.size();
                weight = len;
                if(flag3 == true){
                    for(int j = 0; j < len; j++){
                        merge_tree[inx3].push(buffer.front());
                        buffer.pop();
                    }
                    merge_tree[inx3].push({-2, 0});
                }
                else{
                    for(int j = 0; j < len; j++){
                        DRAM[inx3].push(buffer.front());
                        buffer.pop();
                    }
                    DRAM[inx3].push({-2, 0});
                    inx3 += merge_tree_size;
                }
                DRAM_cycle_cnt = 0;
                if(flag1 == true) merge_tree[inx1].pop();
                else DRAM[inx1].pop();
                if(flag2 == true) merge_tree[inx2].pop();
                else DRAM[inx2].pop();
                haffuman_tree_scheduler.push({weight, inx3});
                merge_flag = false;
            }
        }

        skip:
        // push
        for(int i = 0; i < merge_tree_size; i++){
            // merge_tree
            if(unit_available_flag[i] == 1){
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
                    // last
                    if(input_queue_inx[inx].front() == -1){
                        merge_tree_inx[i] = -1;
                        unit_available_flag[i] = 0;
                        output_available_flag[inx] = true;
                        input_queue_inx[inx].pop();
                        merge_tree[i].push({-2, 0});
                        haffuman_tree_scheduler.push({merge_tree[i].size(), i}); 
                    }
                }
            }
        }

        for(int i = 0; i < DRAM_size; i++){
            // DRAM
            if(DRAM_available_flag[i] == 1){
                int inx = DRAM_inx[i];
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
                    DRAM[i].push(element);
                }

                if(input_queue_inx[inx].size() != 0){
                    // last
                    if(input_queue_inx[inx].front() == -1){
                        DRAM_inx[i] = -1;
                        DRAM_available_flag[i] = 0;
                        output_available_flag[inx] = true;
                        input_queue_inx[inx].pop();
                        DRAM[i].push({-2, 0});
                        haffuman_tree_scheduler.push({DRAM[i].size(), i + merge_tree_size});
                    }
                }
            }
        }

        if(last_flag == true && merge_flag == false && haffuman_tree_scheduler.size() == 1){
            output_flag = true;
            auto output_inx = haffuman_tree_scheduler.top().second;
            haffuman_tree_scheduler.pop();
            if(output_inx >= merge_tree_size){
                output_inx -= merge_tree_size;
                output = merge_tree[output_inx];
            }
            else{
                output = merge_tree[output_inx];
            }  
        }

        return;
    }
};