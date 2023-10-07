#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <iomanip>

#include "../src/utils/perftest.hpp"

using namespace std;

void sort_vec(vector<int>& test){
    sort(test.begin(), test.end());
}

vector<int> generate_random_vector(const int n, const int min, const int max){
    vector<int> vec(n);
    random_device rd;
    for(int i = 0; i < n; i++){
        vec[i] = rd() % (max - min) + min;
    }
    return vec;
}

int main(){
    ios::sync_with_stdio(0); cin.tie(NULL);  cout.tie(NULL);
    vector<int> vec = generate_random_vector(10000, -100000, 100000);
    cout << perftest::benchmark(sort_vec, vec) << "ms\n";
    cout << "------------------------------------------------------------\n";
    cout << perftest::benchmark(sort_vec, generate_random_vector(10000, -100000, 100000)) << "ms\n";

    return 0;
}