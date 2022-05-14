#include<bits/stdc++.h>
using namespace std;

class Outer_product
{
private:
    int result_size = 0;
public:

    vector<vector<pair<int, double>>> result;
    vector<pair<int, double>> output_result;  
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

        if(last_flag2 == true){
            last_flag = true;
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
        set<int> S;
        int cnt = 0;
        for(int i = 0; i < B_input.size(); i++){
            if(S.count(B_input[i].first)){
                cnt++;
                continue;
            }
            S.insert(B_input[i].first);
            output.push({result_size * cal + B_input[i].first, val * B_input[i].second}); //こっちを使う
            result[cal].push_back({B_input[i].first, val * B_input[i].second});
        }
        output_inx.push(B_input.size() - cnt); //これも使う
        if(get<4>(A_element)){
            output.push({-1, -1}); // 初回のみ弾く
            output_inx.push(-1);
        }

        // AもBも使ったので次の奴を要求する
        input_flag = false;
        inx_flag = false;
        B_flag = false;

        return;
    }
};