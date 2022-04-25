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
    bool fetch_flag = true; 
    int fetch_size = 0;
    int fetched_num = 0;
    int cnt = 0;
public:
    vector<double> A_cols; // no use
    vector<double> A_offsets; // no use
    vector<double> A_values; // no use
    double A_fetched_values[10000]; // no use
    pair<int, int> A_fetched_inx[10000]; // no use
    vector<vector<tuple<int, int, double>>> A_fetch;
    tuple<int, int, double> output;
    bool output_flag = false;
    int row_inx = 0;

    A_fetcher(int n) {
        init(n);
    }
 
    void init(int n) { // generate length n vector A as CSR
        // generate A
        A_fetch.resize(n, vector<tuple<int, int, double>>(0));
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
      
        // construct A_fetch[false w] tuple(h, true w, val)
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                if(A[j].size() <= i) continue;
                A_fetch[i].pb(make_tuple(j, A[j][i].first, A[j][i].second));
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

        // fetch one element from A_fetch
        if(cnt == 0) fetch_flag = false;
        if(fetch_flag == false) return;
        if(output_flag == true) return;
        if(fetch_size == fetched_num){
            row_inx++;
            fetch_size = A_fetch[row_inx].size();
            fetched_num = 0;
        }
        output = A_fetch[row_inx][fetched_num];
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
    bool fetch_flag = true; 
    int fetch_size = 0;
    int fetched_num = 0;
    int cnt = 0;
public:
    // output variable
    tuple<int, int, int, double> A_element; //<false w, h, true w, val>
    bool output_flag = false;
    //int row_inx = 0;

    // FIFO
    queue<tuple<int, int, double>> FIFO;

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
    }

    void clock(bool &input_flag, tuple<int, int, double> input, int row_inx) {

        if(input_flag == true){
            // FIFO process
            FIFO.push(input);
            input_flag = false;
          	
            // update distance table
            if(A_table[get<1>(input)].size() == 0){
                distance_table[get<1>(input)] = cnt;
              	//cout << get<1>(input) << " " << distance_table[get<1>(input)] << endl;
            }
            A_table[get<1>(input)].pb(cnt);
            cnt++;
        }
      
        if(output_flag == false){
            if(FIFO.size() != 0){
                // FIFO process
              	tuple<int, int, double> front = FIFO.front();
                A_element = make_tuple(row_inx, get<0>(front), get<1>(front), get<2>(front));
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

int main() {

    A_fetcher A_fetcher(10);
    Builder Builder(10);
  	rep(i, 100){
      A_fetcher.clock();
      Builder.clock(A_fetcher.output_flag, A_fetcher.output, A_fetcher.row_inx);
      auto v = Builder.A_element;
      //cout << get<0>(v) << " " << get<1>(v) << " " << get<2>(v) << " " << get<3>(v) << endl;
      
      //rep(i, 10){
        //cout << Builder.distance_table[i] << " ";
      //}
      //cout << endl;
      //Builder.output_flag = false;
    }
  	
  	rep(i, 10){
      rep(j, Builder.A_table[i].size()){
        cout << Builder.A_table[i][j] << " ";
      }
      cout << endl;
    } 
  	rep(i, 10){
        cout << Builder.distance_table[i] << " ";
    }
  	cout << endl;
  
  	rep(i, 100){
      A_fetcher.clock();
      Builder.clock(A_fetcher.output_flag, A_fetcher.output, A_fetcher.row_inx);
      auto v = Builder.A_element;
      cout << Builder.FIFO.size() << " " << get<0>(v) << " " << get<1>(v) << " " << get<2>(v) << " " << get<3>(v) << endl;
      
      rep(i, 10){
        cout << Builder.distance_table[i] << " ";
      }
      cout << endl;
      Builder.output_flag = false;
    }
    
    return 0;
}     