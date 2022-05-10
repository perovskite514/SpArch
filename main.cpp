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



class A_fetcher
{
private: 
    int fetch_size = 0;
    int fetched_num = 0;
    int cnt = 0;
public:
    vector<double> A_cols; // no use
    vector<double> A_offsets; // no use
    vector<double> A_values; // no use
    double A_fetched_values[10000]; // no use
    pair<int, int> A_fetched_inx[10000]; // no use
    vector<vector<tuple<int, int, int, double>>> A_fetch;
    tuple<int, int, int, double, bool> output;
    bool output_flag = false;
    bool fetch_flag = true;
    int row_inx = 0;

    A_fetcher(int n) {
        init(n);
    }
 
    void init(int n) { // generate length n vector A as CSR
        // generate A
        A_fetch.resize(n, vector<tuple<int, int, int, double>>(0));
        vector<vector<pair<int, double>>> A(n);
      	
        for(int i = 0; i < n; i++){
            int num = rand() % 5;
            set<int> S;
            for(int j = 0; j < num; j++){
                double value = rand() % 100;
                int cols = rand() % n;
                if(S.count(cols)) continue;
                S.insert(cols);
                A[i].push_back({cols, value});
            }
        }
      
        // convert A as CSR
        A_offsets.push_back(0);
        for(int i = 0; i < n; i++){
            int num = A[i].size();
            A_offsets.push_back(num);
            sort(A[i].begin(), A[i].end());
            for(int j = 0; j < num; j++){
                auto element = A[i][j];
                A_cols.push_back(element.first);
                A_values.push_back(element.second);
            }
        }
      
        // construct A_fetch[false w] tu^ple(h, true w, val)
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                if(A[j].size() <= i) continue;
                A_fetch[i].pb(make_tuple(i, j, A[j][i].first, A[j][i].second));
                cnt++;
            }
        }
        fetch_size = A_fetch[row_inx].size();
        //rep(i, n){
          //  rep(j, A[i].size()){
            //    cout << A[i][j].fi << " " << A[i][j].se << "  ";
            //}
            //cout << endl;
        //}

        //rep(i, n){
          //  rep(j, A_fetch[i].size()){
            //    cout << get<0>(A_fetch[i][j]) << " " << get<1>(A_fetch[i][j]) << " " << get<2>(A_fetch[i][j]) << "  ";
            //}
            //cout << endl;
        //}
    }

    void clock() {

        cout << cnt << " " << row_inx << " " << fetched_num << " " << fetch_size << endl;
        // fetch one element from A_fetch
        if(cnt == 0) fetch_flag = false;
        if(fetch_flag == false) return;
        if(output_flag == true) return;
        if(fetch_size == fetched_num){
            row_inx++;
            fetch_size = A_fetch[row_inx].size();
            fetched_num = 0;
        }
        bool row_change_flag = (fetch_size == fetched_num + 1);
        output = make_tuple(get<0>(A_fetch[row_inx][fetched_num]), get<1>(A_fetch[row_inx][fetched_num]), get<2>(A_fetch[row_inx][fetched_num]), get<3>(A_fetch[row_inx][fetched_num]), row_change_flag);
        output_flag = true;
        fetched_num++;
        cnt--;
        return;
    }

    // output_flagはFIFO側がデータを読みだしたらfalseに設定される。
    // つまり、A_fetcherはFIFOに読まれる度にデータを提供し続ける。
};



class Builder
{
private:
    int cnt = 0;
public:
    // output variable
    tuple<int, int, int, double, bool> A_element; //<false w, h, true w, val>
    bool output_flag = false;
    bool last_flag;
    //int row_inx = 0;

    // FIFO
    queue<tuple<int, int, int, double, bool>> FIFO;

    //distance table
    vector<vector<int>> A_table; // A_table[true w].pb(cnt)
    vector<int> A_inx; // index of "A_table"
    vector<int> distance_table; // distance_table[true w] = distance 
    Builder(int n) {
        init(n);
    }

    void init(int n) {
        A_table.resize(n, vector<int>(0));
        A_inx.resize(n, 0);
        distance_table.resize(n, 1e9);
        last_flag = false;
    }

    void clock(bool &input_flag, bool &fetch_flag, tuple<int, int, int, double, bool> input, int row_inx) {

        //cout << input_flag << " " << fetch_flag << endl;
        //cout << get<0>(input) << " " << get<1>(input) << " " << get<2>(input) << " " << get<3>(input) << endl; 
        if(fetch_flag == false && input_flag == false){
            // last
            if(FIFO.size() == 0) last_flag = true;
        }
        if(input_flag == true){
            // FIFO process
            FIFO.push(input);
            input_flag = false;
        
            // update distance table
            if(A_table[get<2>(input)].size() == 0){
                distance_table[get<2>(input)] = cnt;
              	//cout << get<1>(input) << " " << distance_table[get<1>(input)] << endl;
            }
            A_table[get<2>(input)].pb(cnt);
            cnt++;
        }
      
        if(output_flag == false){
            if(FIFO.size() != 0){
                // FIFO process
              	tuple<int, int, int, double, bool> front = FIFO.front();
                A_element = front;
              	FIFO.pop();
                output_flag = true; 

                // update distance table
                A_inx[get<2>(A_element)]++;
                if(A_table[get<2>(A_element)].size() > A_inx[get<2>(A_element)]){
                    distance_table[get<2>(A_element)] = A_table[get<2>(A_element)][A_inx[get<2>(A_element)]];
                  	//cout << get<1>(input) << " " << A_table[get<1>(input)][A_inx[get<1>(input)]] << endl;
                }
                else{
                    distance_table[get<2>(A_element)] = 1e9; // no true col element of FIFO
                }
            }
        }

        return;
    }

    // A_fetcherから要素を1つずつ受け取ってFIFOに入れて、distance_tableを更新してから
    // FIFOからmultiplier arrayに要素を入れる。
};



class B_prefetcher
{
private:
    int hbm_clock = 0;
    int buffer_size = 0;
public:

    vector<tuple<int, int, double>> Buffer; // (cal, row, val)

    // B
    vector<vector<pair<int, double>>> B;
    vector<pair<int, double>> B_output;
    bool output_flag;

    B_prefetcher(int n) {
        init(n);
    }

    void init(int n) { // generate length 2 * n Buffer, length n B as CSR

        output_flag = false;
        // generate B
        B.resize(n, vector<pair<int, double>>(0));
        Buffer.resize(0);
        for(int i = 0; i < 2 * n; i++){
            Buffer.push_back(make_tuple(n, i, 0));
        }
        buffer_size = 2 * n;
      	
        for(int i = 0; i < n; i++){
            int num = rand() % 5 + 1;
            set<int> S;
            for(int j = 0; j < num; j++){
                double value = rand() % 100;
                int cols = rand() % n;
                if(S.count(cols)) continue;
                S.insert(cols);
                B[i].push_back({cols, value});
            }
            sort(B[i].begin(), B[i].end());
        }

    }

    void clock(int inx, bool &inx_flag, vector<int> distance_table) {

        if(inx_flag == false) return;

        int calcnt = 0;
        for(int i = 0; i < buffer_size; i++){
            if(get<0>(Buffer[i]) == inx) calcnt++;
        }
        if(calcnt == B[inx].size()){
            // exists B's cal in buffer
            B_output = B[inx];
            output_flag = true;
            inx_flag = false;
            return;
        }

        hbm_clock++;
        if(hbm_clock == 10){
            hbm_clock = 0;

            // load from hbm
            set<int> S;
            for(int i = 0; i < buffer_size; i++){
                if(get<0>(Buffer[i]) == inx){
                    S.insert(get<1>(Buffer[i]));
                }
            }

            vector<pair<int, double>> change_inx;
            for(int i = 0; i < B[inx].size(); i++){
                if(!S.count(B[inx][i].first)){
                    change_inx.push_back(B[inx][i]);
                }
            }

            for(int i = 0; i < change_inx.size(); i++){
                int distance = 0;
                int cinx = 0; // change_inx;
                int crow = 0; // change_row_inx;
                for(int j = 0; j < Buffer.size(); j++){
                    if(get<0>(Buffer[j]) == inx) continue;
                    if(distance_table[get<0>(Buffer[j])] > distance){
                        distance = distance_table[get<0>(Buffer[j])];
                        cinx = j;
                        crow = get<1>(Buffer[j]);
                    }
                    else if(distance_table[get<0>(Buffer[j])] == distance){
                        if(get<1>(Buffer[j]) < crow){
                            crow = get<1>(Buffer[j]);
                            cinx = j;
                        }
                    }
                }
                Buffer[cinx] = make_tuple(inx, change_inx[i].first, change_inx[i].second);
            }

            B_output = B[inx];
            output_flag = true;
            inx_flag = false;
        }

        return;
    }

    // B_prefetcherとOuter_productとの間はOuter_product側のflagで管理
};



class Outer_product
{
private:
    int result_size = 0;
public:

    vector<vector<pair<int, double>>> result;
    vector<pair<int, double>> output_result;  
    //pair<int, double> output;
    queue<pair<int, double>> output;
    queue<int> output_inx;
    bool output_flag;
    bool inx_flag;
    bool last_flag;
    int inx;
    int row;

    Outer_product(int n){
        init(n);
    }

    void init(int n){ 

        result.resize(n, vector<pair<int, double>>(0));
        output_result.resize(0);
        inx_flag = false;
        output_flag = false;
        last_flag = false;
        inx = 0;
        row = n;
        result_size = n;
        return;

    }

    void clock(bool &last_flag2, bool &input_flag, tuple<int, int, int, double, bool> A_element, bool &B_flag, vector<pair<int, double>> B_input){

        //cout << row << " row " << inx << " inx" << endl;
        //cout << get<0>(A_element) << " " << get<1>(A_element) << " " << get<2>(A_element) << " " << get<3>(A_element) << " " << get<4>(A_element) << endl;
        // output関連の処理
        //if(output_result.size() != 0){
          //  if(output_flag == false){
            //    output = output_result[output_inx];
              //  output_inx++;
            //}
        //}

        if(last_flag2 == true){
            last_flag = true;
        }
        if(last_flag2 == true && row != 1e9){
            // last
            for(int i = 0; i < result_size; i++){
                for(int j = 0; j < result[i].size(); j++){
                    //output_result.push_back({result_size * i + result[i][j].first, result[i][j].second});
                }
            }
            if(row != result_size){
                //output_result.push_back({-1, -1}); // 初回のみ弾く
                //output_inx.push(-1);
            }
            //row = 1e9;
            //result.resize(result_size, vector<pair<int, double>>(0));
        }

        if(input_flag == false) return;

        //ここの操作は一列読み切ったら結果をvalidにしてmergeに送る、を独立で行う。
        if(row != get<0>(A_element)){
            // move result to output_result
            for(int i = 0; i < result_size; i++){
                for(int j = 0; j < result[i].size(); j++){
                    output_result.push_back({result_size * i + result[i][j].first, result[i][j].second});
                }
            }
            if(row != result_size){
                output_result.push_back({-1, -1}); // 初回のみ弾く
                output_inx.push(-1);
                //cout << "-1push" << endl;
            }
            row = get<0>(A_element);
            result.resize(result_size, vector<pair<int, double>>(0));
        }

        // inputがtrueなのでinxがvalid
        inx_flag = true;
        inx = get<2>(A_element);
        if(B_flag == false) return;

        // AもBも揃っている。
        int cal = get<1>(A_element);
        int val = get<3>(A_element);
        cout << "OUTER" << endl;
        for(int i = 0; i < B_input.size(); i++){
            output.push({result_size * cal + B_input[i].first, val * B_input[i].second}); //こっちを使う
            cout << result_size * cal + B_input[i].first << " " << val * B_input[i].second << endl;
            result[cal].push_back({B_input[i].first, val * B_input[i].second});
        }
        output_inx.push(B_input.size()); //これも使う

        if(get<4>(A_element)){
            //output.push({-1, -1}); // 初回のみ弾く
            output_inx.push(-1);
            cout << "-1push" << endl;
        }

        // AもBも使ったので次の奴を要求する
        input_flag = false;
        inx_flag = false;
        B_flag = false;

        return;
    }
};



class Merge_Unit
{
private:
public:
    
    vector<queue<pair<int, double>>> merge_tree;
    vector<int> merge_tree_inx;
    vector<int> merge_ok_flag;
    vector<int> unit_available_flag;
    vector<queue<pair<int, double>>> DRAM;
    vector<bool> DRAM_available_flag;
    vector<bool> output_available_flag;
    vector<int> outer_to_unit_inx;
    queue<int> DRAM_inx;
    int unit_inx;
    bool DRAM_flag;
    bool output_flag;
    queue<pair<int, double>> output;

    Merge_Unit(int n){
        init(n);
    }
 
    void init(int n){
        
        merge_tree.resize(8);
        merge_tree_inx.resize(8);
        merge_ok_flag.resize(8);
        unit_available_flag.resize(8, 0); // 0何もなし, 1はマッチング完了, 2は
        output_available_flag.resize(4, true);
        outer_to_unit_inx.resize(4, 0);
        DRAM_flag = true;
        DRAM.resize(1000);
        DRAM_available_flag.resize(1000, false);
        output_flag = false;
        unit_inx = 0;
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
      
      	for(int j = 0; j < output.size(); j++){
        	//cout << output[j].first << " " << output[j].second << endl;
        }
        return output;
    }

    vector<pair<int, double>> comparator_array(bool &a_flag, bool &b_flag, queue<pair<int, double>> que1, queue<pair<int, double>> que2, vector<bool> &a_or_b_flag){

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

        int a_chunk_flag[3] = {};
        int b_chunk_flag[3] = {};
        if(a_len <= 4){
            a_chunk_flag[1] = 1;
            a_chunk_flag[2] = 1;
        }
        else if(a_len <= 8){
            a_chunk_flag[2] = 1;
        };
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
            vector<bool> a_or_b;
            low_output = low_comparator(a_low, b_low, a_or_b);
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
          	//if(i == inx - 1) last_inx = 7;
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
                if(pos > pre_pos) a_or_b_flag.push_back(a_or_b[j]);
                if(pos > pre_pos) cout << a_or_b[j];
                double val = low_output[j].second;
                while(1){
                    j++;
                    if(j > last_inx) break;
                    if(pos != low_output[j].first) break;
                    val += low_output[j].second;
                    if(pos > pre_pos) a_or_b_flag.push_back(a_or_b[j]);
                    if(pos > pre_pos) cout << a_or_b[j];
                }
                j--;
                if(pos > pre_pos) output.push_back({pos, val});
              	if(pos > pre_pos) cout << " " << pos << " " << val << " 1" << endl;
            }

            if(i != inx - 1) continue;
            for(int j = last_inx + 1; j < 8; j++){
                if(low_output[j].second != 1e9){
                    if(top_level_output[i][2] == 1){
                        //que1_rev.push(low_output[j]);
                        //cout << low_output[j].fi << " " << low_output[j].se << "low" << endl;
                    }
                    else{
                        //que2_rev.push(low_output[j]);
                        //cout << low_output[j].fi << " " << low_output[j].se << "low2" << endl;
                    }
                }
            }
        }

        int a_or_b_len = a_or_b_flag.size();
        rep(i, a_or_b_len){
            cout << "( " << i << " " << a_or_b_flag[i] << ") ";
        }
        cout << endl;
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
          //cout << "size_error" << endl;
          //exit(1);
        }
        for(int i = 0; i < output_comp_len; i++){
          	//cout << output_comp[i].first << " " << output_comp[i].second << endl;
            if(output_comp[i] != output_debug[i]){
              //cout << "error" << endl;
              //exit(1);
            }
        }
      	for(int i = 0; i < len2; i++){
          	//cout << output_debug[i].first << " " << output_debug[i].second << endl;
            //if(output_comp[i] != output_debug[i]) exit(1);
        }

        return output_comp;
    }

    void clock(bool &last_flag, vector<bool> outer_product_flag, vector<queue<pair<int, double>>> &input_queue, vector<queue<int>> &input_queue_inx){

        if(output_flag) return;
        //cout << last_flag << " " << DRAM_flag << endl;
        rep(i, 8){
            cout << "(" << i << " , " << merge_tree[i].size() << "), ";  
        }
        cout << endl;
        // 空いているleaf_nodeにinputをマッチングする
        for(int j = 0; j < 1; j++){
            if(output_available_flag[j] == true && input_queue[j].size() != 0){
                for(int i = 0; i < 4; i++){
                    //cout << i << " " << outer_to_unit_inx[j] << endl;
                    if(unit_available_flag[(i + outer_to_unit_inx[j]) % 4 + 4] == 0){
                        merge_tree_inx[(i + outer_to_unit_inx[j]) % 4 + 4] = j;
                        unit_available_flag[(i + outer_to_unit_inx[j]) % 4 + 4] = 1;
                        outer_product_flag[j] = false;
                        cout << "Matching " << (i + outer_to_unit_inx[j]) % 4 + 4;
                        outer_to_unit_inx[j]++;
                        outer_to_unit_inx[j] %= 4;
                        output_available_flag[j] = false;
                        cout << " " << (i + outer_to_unit_inx[j]) % 4 + 4 << endl;
                        break;
                    }
                }
            }
        }

        //for(int i = 4; i < 8; i++){
          //  if(unit_available_flag[i] == true){
             //   for(int j = 0; j < 1; j++){
                //    if(output_available_flag[j] == true && input_queue[j].size() != 0){
                    //    merge_tree_inx[i] = j;
                    //    unit_available_flag[i] = false;
                    //    outer_product_flag[j] = false;
                    //    break;
                    //}
                //}
            //}
        //}

        // merge_tree
        for(int i = 1; i < 8; i++){
            if(i < 4){
                // 上位nodeからmerge
                bool a_flag = true;
                bool b_flag = true;
                if(merge_ok_flag[2 * i] == 0 || merge_ok_flag[2 * i + 1] == 0) continue;
                //if(merge_tree[2 * i].size() == 0 && merge_tree[2 * i + 1].size() == 0) continue;
                //if(unit_available_flag[2 * i] == false || unit_available_flag[2 * i + 1] == false) continue;
                auto debug1 = merge_tree[2 * i];
                auto debug2 = merge_tree[2 * i + 1];
                cout << "debug1 " << 2 * i << endl;
                ll len1 = debug1.size();
                ll len2 = debug2.size();
                rep(i, len1){
                    cout << debug1.front().fi << " " << debug1.front().se << endl;
                    debug1.pop();
                }
                cout << endl;

                cout << "debug2" << endl;
                rep(i, len2){
                    cout << debug2.front().fi << " " << debug2.front().se << endl;
                    debug2.pop();
                }
                cout << endl;
                vector<bool> a_or_b_flag;
                vector<pair<int, double>> merge_result = comparator_array(a_flag, b_flag, merge_tree[2 * i], merge_tree[2 * i + 1], a_or_b_flag);
                auto debug3 = merge_result;
                cout << "debug3" << endl;
                ll len3 = debug3.size();
                rep(i, len3){
                    cout << debug3[i].fi << " " << debug3[i].se << endl;
                }
                cout << endl;

                int len = merge_result.size();
                for(int j = 0; j < len; j++){
                    merge_tree[i].push(merge_result[j]);
                    //cout << i << " " << merge_result[j].first << " " << merge_result[j].second << " node" << endl;
                }
                for(int j = 0; j < a_or_b_flag.size(); j++){
                    if(a_or_b_flag[j]){
                        merge_tree[2 * i].pop();
                    }
                    else{
                        merge_tree[2 * i + 1].pop();
                    }
                }

                debug1 = merge_tree[2 * i];
                debug2 = merge_tree[2 * i + 1];
                len1 = debug1.size();
                len2 = debug2.size();
                cout << "debug1" << endl;
                rep(i, len1){
                    cout << debug1.front().fi << " " << debug1.front().se << endl;
                    debug1.pop();
                }
                cout << endl;
                cout << "debug2" << endl;
                rep(i, len2){
                    cout << debug2.front().fi << " " << debug2.front().se << endl;
                    debug2.pop();
                }
                cout << endl;

                if(a_flag == false && b_flag == false){
                    // mergeが終わった。
                    merge_tree[i].push({-2, 0});
                    cout << i << " -2push" << endl;
                    if(merge_tree[2 * i].size() != 0) merge_tree[2 * i].pop();
                    if(merge_tree[2 * i + 1].size() != 0) merge_tree[2 * i + 1].pop();
                    merge_ok_flag[i]++;
                    merge_ok_flag[2 * i]--;
                    merge_ok_flag[2 * i + 1]--;
                }
            }
            else{
                // leaf_node
                cout << i << endl;
                if(unit_available_flag[i] == 1){
                    int inx = merge_tree_inx[i];
                    cout << inx << endl;
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
                        cout << i << " " << element.first << " " << element.second << " node" << endl;
                    }
                    if(input_queue_inx[inx].size() != 0){
                        // 最後
                        if(input_queue_inx[inx].front() == -1){
                            merge_tree_inx[i] = -1;
                            unit_available_flag[i] = 0;
                            output_available_flag[inx] = true;
                            outer_product_flag[inx] = true;
                            input_queue_inx[inx].pop();
                            merge_tree[i].push({-2, 0});
                            merge_ok_flag[i]++;
                            cout << i << " -2push" << endl; 
                        }
                    }
                }
            }
        }

        bool DRAM_empty_flag = true; 
        for(int i = 0; i < 1000; i++){
            if(DRAM[i].size() != 0) DRAM_empty_flag = false;
        }

        // root_node
        if(last_flag == true && DRAM_empty_flag == true){
            // merge_tree内のmerge_resultが出力
            int num = 0;
            for(int i = 0; i < 8; i++){
                if(merge_tree[i].size() > 0) num++; 
            }
            if(num == 1){
                output_flag = true;
                for(int i = 0; i < 8; i++){
                    if(merge_tree[i].size() > 0) output = merge_tree[i];
                }
            }
        }

        if(output_flag == true) return;
        // 空いているleaf_nodeにrootのoutputをマッチングする
        bool match_flag = false;
        if(unit_available_flag[1] == 0){
            for(int i = 4; i < 8; i++){
                if(unit_available_flag[i] == 0){
                    if(merge_tree[1].size() != 0){
                        merge_tree_inx[i] = 1e9;
                        merge_tree_inx[1] = i;
                        unit_available_flag[1] = 2;
                        unit_available_flag[i] = 2;
                        match_flag = true;
                        break;
                    }
                }
            }
            if(match_flag == false){
                // DRAMに入れる
                for(int i = 0; i < 1000; i++){
                    if(DRAM_available_flag[i] == true){
                        if(merge_tree[1].size() != 0){
                            merge_tree_inx[1] = 1000 + i; // 1000は適当な定数
                            unit_available_flag[1] = 2;
                            DRAM_available_flag[i] = false;
                            match_flag = true;
                            break;
                        }
                    }
                }
            }
        }

        if(unit_available_flag[1] == 2){
            int inx = merge_tree_inx[1];
            if(inx >= 1000){
                inx -= 1000; //さっき足した1000を引く
                for(int j = 0; j < 24; j++){
                    if(merge_tree[1].size() == 0) break;
                    pair<int, double> element = merge_tree[1].front();
                    if(element.first == -2){
                        unit_available_flag[1] = 0;
                        merge_tree_inx[1] = -1;
                        merge_tree[1].pop();
                        DRAM_inx.push(inx);
                        DRAM[inx].push({-2, 0});
                        break;
                    }
                    DRAM[inx].push(element);
                    merge_tree[1].pop();
                }
            }
            for(int j = 0; j < 24; j++){
                if(merge_tree[1].size() == 0) break;
                pair<int, double> element = merge_tree[1].front();
                if(element.first == -2){
                    unit_available_flag[1] = 0;
                    unit_available_flag[inx] = 0;
                    merge_tree_inx[1] = -1;
                    merge_tree_inx[inx] = -1;
                    merge_tree[1].pop();
                    merge_tree[inx].push({-2, 0});
                    break;
                }
                merge_tree[inx].push(element);
                merge_tree[1].pop();
            }
        }

        // DRAM

        // 空いているleaf_nodeにDRAMをマッチングする
        if(DRAM_inx.size() != 0 && DRAM_flag == true){
            int inx = DRAM_inx.front();
            for(int i = 4; i < 8; i++){
                if(unit_available_flag[i] == 0){
                    merge_tree_inx[i] = inx;
                    unit_available_flag[i] = 2;
                    DRAM_flag = false;
                    unit_inx = i;
                }
            }
        }
        
        if(DRAM_flag == false && unit_available_flag[unit_inx] == false){
            int inx = merge_tree_inx[unit_inx];
            for(int j = 0; j < 24; j++){
                if(DRAM[inx].size() == 0) break;
                pair<int, double> element = DRAM[inx].front();
                if(element.first == -2){
                    unit_available_flag[unit_inx] = 0;
                    DRAM_available_flag[inx] = true;
                    merge_tree_inx[unit_inx] = -1;
                    merge_tree[unit_inx].push({-2, 0});
                    DRAM[inx].pop();
                    DRAM_inx.pop();
                    DRAM_flag = true;
                    break;
                }
                merge_tree[unit_inx].push(element);
                DRAM[inx].pop();
            }
        }

        return;
    }
};

int main() {

    A_fetcher A_fetcher(10);
    Builder Builder(10);
    B_prefetcher B_prefetcher(10);
    Outer_product Outer_product(10);
    Merge_Unit Merge_Unit(10);

    rep(i, 10){
        auto v = B_prefetcher.B[i];
        ll len = v.size();
        rep(j, len){
            //cout << v[j].first << " " << v[j].second << " " << i << endl;
        }
        //cout << endl;
    }
    //cout << endl;
    rep(i, 300){
      //A_fetcher.clock();
      //Builder.clock(A_fetcher.output_flag, A_fetcher.fetch_flag, A_fetcher.output, A_fetcher.row_inx);
      auto v = Builder.A_element;
    }
    auto v = Builder.A_element;
    //cout << Builder.FIFO.size() << " " << get<0>(v) << " " << get<1>(v) << " " << get<2>(v) << " " << get<3>(v) << endl;
      ll len = Builder.FIFO.size();
      rep(i, len){
          auto v = Builder.FIFO.front();
          Builder.FIFO.pop();
          //cout << get<0>(v) << " " << get<1>(v) << " " << get<2>(v) << endl;
      }
      rep(i, 10){
        //cout << Builder.distance_table[i] << " ";
      }
      //cout << endl;
      //Builder.output_flag = false;

    
    rep(i, 200){

      vector<bool> flag;
      vector<queue<pair<int, double>>> output;
      flag.push_back(Outer_product.output_flag);
      output.push_back(Outer_product.output);
      vector<queue<int>> output_inx;
      output_inx.push_back(Outer_product.output_inx);
      
      Merge_Unit.clock(Outer_product.last_flag, flag, output, output_inx);

      if(Merge_Unit.output_flag){
        cout << "clock " << i << endl;
        auto p = Merge_Unit.output;
        int len = p.size();
        for(int i = 0; i < len - 1; i++){
          auto element = p.front();
          cout << element.first << " " << element.second << endl;
          p.pop();
        }
        break;
      }
      Outer_product.output = output[0];
      Outer_product.output_inx = output_inx[0];

      Outer_product.clock(Builder.last_flag, Builder.output_flag, Builder.A_element, B_prefetcher.output_flag, B_prefetcher.B_output);

      B_prefetcher.clock(Outer_product.inx, Outer_product.inx_flag, Builder.distance_table);

      Builder.clock(A_fetcher.output_flag, A_fetcher.fetch_flag, A_fetcher.output, A_fetcher.row_inx);
      A_fetcher.clock();
    }
    string input;
    while(1){
      cout << "(sim) ";
      getline(cin, input);
      if(input == "end") break;
      vector<bool> flag;
      vector<queue<pair<int, double>>> output;
      flag.push_back(Outer_product.output_flag);
      output.push_back(Outer_product.output);
      vector<queue<int>> output_inx;
      output_inx.push_back(Outer_product.output_inx);
      output_inx.push_back(Outer_product.output_inx);

      Merge_Unit.clock(Outer_product.last_flag, flag, output, output_inx);
      Outer_product.clock(Builder.last_flag, Builder.output_flag, Builder.A_element, B_prefetcher.output_flag, B_prefetcher.B_output);
      B_prefetcher.clock(Outer_product.inx, Outer_product.inx_flag, Builder.distance_table);
      Builder.clock(A_fetcher.output_flag, A_fetcher.fetch_flag, A_fetcher.output, A_fetcher.row_inx);
      A_fetcher.clock();

      cout << "Builder <false w, h, true w, val>" << endl;
      auto A_element = Builder.A_element;
      cout << get<0>(A_element) << " " << get<1>(A_element) << " " << get<2>(A_element) << " " << get<3>(A_element);
      if(Builder.output_flag) cout << " Output_ok";
      cout << endl;
      cout << endl;

      cout << "B_prefetcher" << endl;
      auto B_cal = B_prefetcher.B_output;
      rep(i, B_cal.size()){
          cout << B_cal[i].first << " " << B_cal[i].second << endl;
      }
      if(B_prefetcher.output_flag) cout << " Output_ok" << endl;
      cout << endl;

      cout << "Outer_product ";
      auto outer_output = Outer_product.output;
      auto output_inx2 = Outer_product.output_inx;
      cout << "inx " << Outer_product.inx << " row" << Outer_product.row << endl;
      if(Outer_product.output_flag) cout << "Output_on";
      cout << endl;
      if(Outer_product.inx_flag) cout << "Inx_on";
      cout << endl; 
      if(Outer_product.last_flag) cout << "Last_on";

      if(input == "outer"){
          ll len = outer_output.size();
          rep(i, len){
              cout << outer_output.front().first << " " << outer_output.front().second << endl;
              outer_output.pop(); 
          }
      }

      cout << endl;
      cout << endl;

      if(Merge_Unit.output_flag){
          cout << "End" << endl;
          ll len = Merge_Unit.merge_tree[1].size();
          rep(i, len){
              cout << Merge_Unit.merge_tree[1].front().first << " " << Merge_Unit.merge_tree[1].front().second << endl;
              Merge_Unit.merge_tree[1].pop();
          }
          break;
      }
    }

      //rep(i, 10){
        //cout << Builder.distance_table[i] << " ";
      //}
      //cout << endl;
      //Builder.output_flag = false;
  	
  	rep(i, 10){
      rep(j, Builder.A_table[i].size()){
        //cout << Builder.A_table[i][j] << " ";
      }
      //cout << endl;
    } 
  	rep(i, 10){
        //cout << Builder.distance_table[i] << " ";
    }
  	//cout << endl;
  
  	

    return 0;
}