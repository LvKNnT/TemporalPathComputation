#include <bits/stdc++.h>
#include <filesystem>
#include "StreamPresentation.h"
#include "TransformingGraph.h"
#include "GenerateTest.h"

using namespace std;
namespace fs = std::filesystem;

void getGraph(fstream& fin) {
    vector<vector<int> > g;

    string line;
    while(getline(fin, line)) {
        if(!line.empty() && line[0] == '%') continue; // skipping comment line

        stringstream ss(line);
        string u, v;
        int ta, tw;
        if(ss >> u >> v >> tw >> ta) {
            g.push_back({hasher.AddString(u), hasher.AddString(v), ta, tw});
        }
        else {
            cerr << "Invalid line!\n"; // should not come here
            return;
        }
    }

    StreamPresentation::GenerateGraph(g);
    TransformingGraph::GenerateGraph(g);
    TransformingGraph::GenerateReverseGraph(g);
}

void output(fstream& fout, const vector<int>& f) {
    map<string, int> mp;
    for(int i = 0; i < hasher.GetSize(); ++i) {
        mp[hasher.GetString(i)] = f[i];
    }
    for(const pair<string, int>& v : mp) {
        fout << v.first << ": " << v.second << "\n";
    }
    fout << "\n";
}

void output_path(fstream& fout, const vector<int>& parents) {
    int n = hasher.GetSize();
    
    map<string, map<string, bool> > adj_list;

    for(int i=0;i<n;++i) {
        if(parents[i] == -1) continue;

        adj_list[hasher.GetString(parents[i])][hasher.GetString(i)] = true;
    }

    for(const pair<string, map<string, bool> >& adj : adj_list) {
        fout << adj.first << ": ";

        for(const pair<string, bool>& a : adj.second) {
            fout << a.first << " ";
        }
        fout << "\n";
    }
}

// Show the difference between f and ff
void showDifference(fstream& fout, const vector<int>& f, const vector<int>& ff) {
    map<string, int> mp;
    for(int i = 0; i < hasher.GetSize(); ++i) {
        if(f[i] != ff[i]) {
            mp[hasher.GetString(i)] = i;
        }
    }
    if(!mp.empty()) { 
        for(const auto& v : mp) {
            fout << v.first << ": " << f[v.second] << " " << ff[v.second] << "\n";
        }
        fout << "\n";
    }
}

void Foremost(const string& target, const int ta, const int tw, fstream& fin, fstream& fout) {
    fout << "Foremost!\n";

    // Setting input for both solutions
    const int des = hasher.GetId(target);
    if(des == -1) {
        fout << "Input error!\n";
        return;
    }
    
    // Solution provided by streaming graph
    auto begin = std::chrono::high_resolution_clock::now();
    
    vector<int> f = StreamPresentation::ComputingForemostTime(des, ta, tw);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    fout << "1-pass graph: " << elapsed.count() * 1e-9 << " seconds.\n";
    
    // Solution provided by transforming graph
    begin = std::chrono::high_resolution_clock::now();
    
    vector<int> ff = TransformingGraph::ComputingForemostTime(des, ta, tw);
    
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    fout << "Transforming graph: " << elapsed.count() * 1e-9 << " seconds.\n";

    if(f == ff) {
        fout << "Correct!\n";
    }
    else {
        fout << "False!\n";
        // showDifference(fout, f, ff);
    }
}

void ReverseForemost(const string& target, const int ta, const int tw, fstream& fin, fstream& fout) {
    fout << "Reverse-Foremost!\n";

    // Setting input for both solutions
    const int des = hasher.GetId(target);
    if(des == -1) {
        fout << "Input error!\n";
        return;
    }
    
    // Solution provided by streaming graph
    auto begin = std::chrono::high_resolution_clock::now();
    
    vector<int> f = StreamPresentation::ComputingReverseForemostTime(des, ta, tw);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    fout << "1-pass graph: " << elapsed.count() * 1e-9 << " seconds.\n";
    
    // Solution provided by transforming graph
    begin = std::chrono::high_resolution_clock::now();
    
    vector<int> ff = TransformingGraph::ComputingReverseForemostTime(des, ta, tw);
    
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    fout << "Transforming graph: " << elapsed.count() * 1e-9 << " seconds.\n";

    if(f == ff) {
        fout << "Correct!\n";
    }
    else {
        fout << "False!\n";
        showDifference(fout, f, ff);
    }
}

void Fastest(const string& target, const int ta, const int tw, fstream& fin, fstream& fout) {
    fout << "Fastest!\n";

    // Setting input for both solutions
    const int des = hasher.GetId(target);
    if(des == -1) {
        fout << "Input error!\n";
        return;
    }
    
    // Solution provided by streaming graph
    auto begin = std::chrono::high_resolution_clock::now();
    
    vector<int> f = StreamPresentation::ComputingFastestPathDuration(des, ta, tw);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    fout << "1-pass graph: " << elapsed.count() * 1e-9 << " seconds.\n";
    
    // Solution provided by transforming graph
    begin = std::chrono::high_resolution_clock::now();
    
    vector<int> ff = TransformingGraph::ComputingFastestPathDuration(des, ta, tw);
    
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    fout << "Transforming graph: " << elapsed.count() * 1e-9 << " seconds.\n";

    if(f == ff) {
        fout << "Correct!\n";
    }
    else {
        fout << "False!\n";
        // showDifference(fout, f, ff);
    }
}

void Shortest(const string& target, const int ta, const int tw, fstream& fin, fstream& fout) {
    fout << "Shortest!\n";

    // Setting input for both solutions
    const int des = hasher.GetId(target);
    if(des == -1) {
        fout << "Input error!\n";
        return;
    }
    
    // Solution provided by streaming graph
    auto begin = std::chrono::high_resolution_clock::now();
    
    vector<int> f = StreamPresentation::ComputingShortestPathDistance(des, ta, tw);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    fout << "1-pass graph: " << elapsed.count() * 1e-9 << " seconds.\n";
    
    // Solution provided by transforming graph
    begin = std::chrono::high_resolution_clock::now();
    
    vector<int> ff = TransformingGraph::ComputingShortestPathDistance(des, ta, tw);
    
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    fout << "Transforming graph: " << elapsed.count() * 1e-9 << " seconds.\n";

    if(f == ff) {
        fout << "Correct!\n";
    }
    else {
        fout << "False!\n";
        showDifference(fout, f, ff);
    }
}

signed main() {
    ios_base::sync_with_stdio(false);

    const std::string DATA_PATH = fs::absolute("../raw/").string();
    const std::string OUT_PATH = fs::absolute("../testcase/testcase.out").string();

    // Access to output
    fstream fout(OUT_PATH, ios::out);
    if(!fout.is_open()) {
        cerr << "Failed to open output file!\n";
        return -1;
    }

    try {
        for (const auto& entry : fs::recursive_directory_iterator(DATA_PATH)) {
            if (entry.is_regular_file() && entry.path().filename().string().rfind("out", 0) == 0) {
                // Counting time getting graph and compute for each input
                auto begin = std::chrono::high_resolution_clock::now();
                
                cerr << "Processing file: " << entry.path() << "\n";
                fout << "Current file: " << entry.path() << "\n";
                
                fstream fin(entry.path(), ios::in);
                if (fin.is_open()) {
                    getGraph(fin);

                    const string des = "1";
                    const int ta = 0, tw = INT_MAX;

                    /*
                    */
                    Foremost("1", ta, tw, fin, fout);
                    fout << "\n";
                    
                    ReverseForemost("1", ta, tw, fin, fout);
                    fout << "\n";
                    
                    Fastest("1", ta, tw, fin, fout);
                    fout << "\n";
                    
                    Shortest("1", ta, tw, fin, fout);
                    fout << "\n";
                    /*
                    */

                    fin.close();
                } else {
                    std::cerr << "Failed to open file: " << entry.path() << "\n";
                }

                auto end = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
                cerr << "Time measured: " << elapsed.count() * 1e-9 << " seconds.\n";            
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }

    return 0;
}
