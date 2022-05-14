#include<bits/stdc++.h>
using namespace std;

class A_fetcher
{
private: 
    int fetch_size;
    int fetched_num;
    int cnt;
    int cnt2;
    int density;
public:
    vector<vector<tuple<int, int, int, double>>> A_fetch;
    vector<vector<tuple<int, int, int, double>>> A_fetch2;
    tuple<int, int, int, double, bool> output;
    bool output_flag = false;
    bool fetch_flag = true;
    int row_inx;

    A_fetcher(int n, int m) {
        init(n, m);
    }
 
    void init(int n, int m) { // generate length n vector A

        // generate A
        fetch_size = 0;
        fetched_num = 0;
        cnt = 0;
        cnt2 = 0;
        row_inx = 0;
        density = m; // no use
        A_fetch.resize(n, vector<tuple<int, int, int, double>>(0));
        vector<vector<pair<int, double>>> A(n);
        vector<vector<pair<int, double>>> A2(n);
        int p = n;

        for(int i = 0; i < n; i++){
            int num = rand() % (p + 1);
            set<int> S;
            for(int j = 0; j < num; j++){
                double value = rand() % 100;
                int row = rand() % n;
                if(S.count(row)) continue;
                S.insert(row);
                A[i].push_back({row, value});
            }
            sort(A[i].begin(), A[i].end());
        }

        set<int> S2;
        for(int i = 0; i < p; i++){
            int num = rand() % (n * n);
            int cols = num / n;
            int row = num % n;
            double value = rand() % 100;
            if(S2.count(num)) continue;
            S2.insert(num);
            A2[cols].push_back({row, value});
            sort(A2[cols].begin(), A2[cols].end());
        }

        // construct A_fetch[false w] tuple(h, true w, val)
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                if(A[j].size() <= i) continue;
                //A_fetch[i].push_back(make_tuple(i, j, A[j][i].first, A[j][i].second));
                //cnt++;
            }
        }

        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                if(A2[j].size() <= i) continue;
                A_fetch[i].push_back(make_tuple(i, j, A2[j][i].first, A2[j][i].second));
                cnt++;
            }
        }
        fetch_size = A_fetch[row_inx].size();
    }

    void clock() {

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