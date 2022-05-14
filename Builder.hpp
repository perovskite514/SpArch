#include<bits/stdc++.h>
using namespace std;

class Builder
{
private:
    int cnt;
public:
    // output variable
    tuple<int, int, int, double, bool> A_element; //<false w, h, true w, val>
    bool output_flag = false;
    bool last_flag;

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
        cnt = 0;
    }

    void clock(bool &input_flag, bool &fetch_flag, tuple<int, int, int, double, bool> input, int row_inx) {

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
            }
            A_table[get<2>(input)].push_back(cnt);
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
