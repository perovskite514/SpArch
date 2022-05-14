#include<bits/stdc++.h>
#include "A_fetcher.hpp"
using namespace std;

int main() {

    A_fetcher A_fetcher(10, 10);
  	for(int i = 0; i < 100; i++){
      A_fetcher.clock();
      auto v = A_fetcher.output;
      cout << get<0>(v) << " " << get<1>(v) << " " << get<2>(v) << endl;
      A_fetcher.output_flag = false;
    }
    return 0;
}     