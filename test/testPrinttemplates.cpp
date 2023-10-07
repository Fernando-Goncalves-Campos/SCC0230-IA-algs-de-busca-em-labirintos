#include <iostream>
#include <vector>
#include <map>

#include "../src/utils/printtemplates.hpp"
#include "../src/utils/perftest.hpp"

using namespace std;

void testPrints(){
    ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
    cout << "Hello World" << "\n";
    cout << string("Hello World") << "\n";
    cout << pair<int, char>(100, 'f') << '\n';
    cout << pair<string, int>("100", 100) << '\n';
    cout << pair<int, string>(100, "100") << '\n';
    cout << pair<string, string>("100", "100") << '\n';
    cout << 12 << "\n";
    cout << vector<int>{1, 2, 3} << "\n";
    cout << vector<char>{'1', '2', '3'} << "\n";
    cout << vector<vector<int>>{vector<int>{3, 5, 7}, vector<int>{4, 6, 8}, vector<int>{10, 11, 12}} << '\n';
    cout << vector<vector<string>>{vector<string>{"3", "5", "7"}, vector<string>{"4", "6", "8"}, vector<string>{"10", "11", "12"}} << '\n';
    map<char, int> buffer;
    buffer['a'] = 1;
    buffer['b'] = 2;
    buffer['c'] = 3;
    cout << buffer << '\n';
}

int main(){
    ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
    
    cout << perftest::benchmark(testPrints) << "ms\n";
    return 0;
}