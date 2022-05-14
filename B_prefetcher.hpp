#include<bits/stdc++.h>
using namespace std;

class B_prefetcher
{
private:
    int hbm_clock;
    int buffer_size;
    int density;
public:

    vector<tuple<int, int, double>> Buffer; // (cal, row, val)

    // B
    vector<vector<pair<int, double>>> B;
    vector<vector<pair<int, double>>> B2;
    vector<pair<int, double>> B_output;
    bool output_flag;

    B_prefetcher(int n, int m, int l) {
        init(n, m, l);
    }

    void init(int n, int m, int l) { // generate length 2 * n Buffer, 

        hbm_clock = 0;
        density = m;
        buffer_size = l * n;
        output_flag = false;
        // generate B
        B.resize(n, vector<pair<int, double>>(0));
        B2.resize(n, vector<pair<int, double>>(0));
        Buffer.resize(buffer_size);
        for(int i = 0; i < buffer_size; i++){
            Buffer.push_back(make_tuple(n, i, 0));
        }
      	
        int p = n;
        for(int i = 0; i < n; i++){
            int num = rand() % (p + 1);
            set<int> S;
            for(int j = 0; j < num; j++){
                double value = rand() % 100;
                int row = rand() % n;
                if(S.count(row)) continue;
                S.insert(row);
                //B[i].push_back({row, value});
            }
            //sort(B[i].begin(), B[i].end());
        }

        set<int> S2;
        for(int i = 0; i < p; i++){
            int num = rand() % (n * n);
            int cols = num / n;
            int row = num % n;
            double value = rand() % 100;
            if(S2.count(num)) continue;
            S2.insert(num);
            B[cols].push_back({row, value});
            sort(B[cols].begin(), B[cols].end());
        }
    }

    void clock(int &buffer_hit, int inx, bool &inx_flag, vector<int> distance_table) {

        if(inx_flag == false) return;

        int calcnt = 0;
        for(int i = 0; i < buffer_size; i++){
            if(get<0>(Buffer[i]) == inx) calcnt++;
        }
        if(calcnt == B[inx].size()){
            // exists B's cal in buffer
            B_output = B[inx];
            buffer_hit++;
            output_flag = true;
            inx_flag = false;
            return;
        }

        hbm_clock++;
        if(hbm_clock == 60){
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
