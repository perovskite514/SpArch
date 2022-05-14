#include<bits/stdc++.h>
#include "A_fetcher.hpp"
#include "Builder.hpp"
#include "B_prefetcher.hpp"
#include "Outer_product.hpp"
#include "Merge_Unit.hpp"
using namespace std;

int main() {

    int a[4] = {10, 100, 1000, 10000};
    int buffer_hit;
    for(int i = 0; i <= 10; i++){
        buffer_hit = 0;
        int n, m, l;
        m = 0;
        n = 100;
        l = i * 100;
        if(l == 0) l = 1;
        A_fetcher A_fetcher(n, m);
        Builder Builder(n);
        B_prefetcher B_prefetcher(n, m, l);
        Outer_product Outer_product(n);
        Merge_Unit Merge_Unit(n);

    for(int i = 0; i < 1000000; i++){
        
        vector<bool> flag;
        vector<queue<pair<int, double>>> output;
        flag.push_back(Outer_product.output_flag);
        output.push_back(Outer_product.output);
        vector<queue<int>> output_inx;
        output_inx.push_back(Outer_product.output_inx);
        
            Merge_Unit.clock(i, Outer_product.last_flag, flag, output, output_inx);
            if(Merge_Unit.output_flag){
                auto p = Merge_Unit.output;
                int len = p.size();
                for(int i = 0; i < len - 1; i++){
                    auto element = p.front();
                    p.pop();
                }
                cout << "clock  " << i << " " << buffer_hit << endl;
                break;
            }
            Outer_product.output = output[0];
            Outer_product.output_inx = output_inx[0];

            Outer_product.clock(Builder.last_flag, Builder.output_flag, Builder.A_element, B_prefetcher.output_flag, B_prefetcher.B_output);

            B_prefetcher.clock(buffer_hit, Outer_product.inx, Outer_product.inx_flag, Builder.distance_table);

            Builder.clock(A_fetcher.output_flag, A_fetcher.fetch_flag, A_fetcher.output, A_fetcher.row_inx);
            A_fetcher.clock();
        }
    }
    return 0;
}
