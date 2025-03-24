#pragma once     

#include <bits/stdc++.h>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

const std::string TESTCASE_PATH = fs::absolute("../testcase/").string();
const std::string FIN = TESTCASE_PATH + "testcase.INP";
const std::string FOUT = TESTCASE_PATH + "testcase.OUT";

void GenerateTest(int t) {
    mt19937_64 RNG(t);

    auto rand = [&](const int& l, const int& r) {
        return uniform_int_distribution<int>(l, r)(RNG);
    };

    int n = 10000;
    cout << n << "\n";

    vector<vector<int> > tests(n);
    for(int i=0;i<n;++i) {
        tests[i].push_back(rand(0, 10));
        tests[i].push_back(rand(0, 10));
        while(tests[i][1] == tests[i][0]) tests[i][1] = rand(0, 25);

        tests[i].push_back(rand(1, 100));
        tests[i].push_back(rand(1, 2));
    }
    sort(tests.begin(), tests.end());
    
    for(const auto& test : tests) {

        for(int i=0;i<2;++i) cout << test[i] << " ";
        for(int i=2;i<4;++i) cout << test[i] << " ";
        cout << "\n";
    }
    cerr << t << "\n";
}