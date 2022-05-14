#include<bits/stdc++.h>
#include"A_fetcher.hpp"
#include"Builder.hpp"
using namespace std;


int main() {

    A_fetcher A_fetcher(10, 10);
    Builder Builder(10);
  	for(int i = 0; i < 100; i++){
        A_fetcher.clock();
        Builder.clock(A_fetcher.output_flag, A_fetcher.fetch_flag, A_fetcher.output, A_fetcher.row_inx);
        auto v = Builder.A_element;
        cout << get<0>(v) << " " << get<1>(v) << " " << get<2>(v) << " " << get<3>(v) << endl;
      
        for(int j = 0; j < 10; j++){
            cout << Builder.distance_table[j] << " ";
        }
        cout << endl;
        Builder.output_flag = false;
    }
  	
  	for(int i = 0; i < 10; i++){
        for(int j = 0; j < Builder.A_table[i].size(); j++){
            cout << Builder.A_table[i][j] << " ";
        }
        cout << endl;
    } 
  	for(int i = 0; i < 10; i++){
        cout << Builder.distance_table[i] << " ";
    }
  	cout << endl;
  
  	for(int i = 0; i < 100; i++){
        A_fetcher.clock();
        Builder.clock(A_fetcher.output_flag, A_fetcher.fetch_flag, A_fetcher.output, A_fetcher.row_inx);
        auto v = Builder.A_element;
        cout << Builder.FIFO.size() << " " << get<0>(v) << " " << get<1>(v) << " " << get<2>(v) << " " << get<3>(v) << endl;
      
        for(int j = 0; j < 10; j++){
            cout << Builder.distance_table[j] << " ";
        }
        cout << endl;
        Builder.output_flag = false;
    }
    
    return 0;
}     