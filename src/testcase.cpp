#include <bits/stdc++.h>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

//#define INF (int)1e18
//mt19937_64 RNG(chrono::steady_clock::now().time_since_epoch().count());
mt19937_64 RNG(27);
#define int long long
const int MOD = 998244353;

const std::string TESTCASE_PATH = fs::absolute("../testcase/").string();
const std::string fin = TESTCASE_PATH + "testcase.INP";
const std::string fout = TESTCASE_PATH + "testcase.OUT";

void bruh() {
    auto rand = [&](const int& l, const int& r) {
        return uniform_int_distribution<int>(l, r)(RNG);
    };

    int n = 50;
    cout << n << "\n";

    vector<vector<int> > tests(n);
    for(int i=0;i<n;++i) {
        // create vertices
        /*
        for(int j=0;j<2;++j) {
            int m = 1;
            string s(1, '.');
            
            for(int jj = 0; jj < m; ++jj) {
                s[jj] = 'a' + rand(0, 25);
            }
            cout << s << " ";
        }
        
        // create edges
        cout << rand(1, 10) << " " << 1 << "\n";
        */

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
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    srand(time(0));

    auto begin = std::chrono::high_resolution_clock::now();

    if(fopen(fin.c_str(),"r")) {
        freopen(fin.c_str(), "r", stdin);
        freopen(fin.c_str(), "w", stdout);
    }

    bruh();

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cerr << "Time measured: " << elapsed.count() * 1e-9 << " seconds.\n";
}
