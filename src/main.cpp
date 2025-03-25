// #include <bits/stdc++.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <random>

#include <filesystem>
#include "StreamPresentation.h"
#include "TransformingGraph.h"

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

void checkForemost(const string& target, const int ta, const int tw, fstream& fin, fstream& fout) {
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
        showDifference(fout, f, ff);
    }
}

void checkReverseForemost(const string& target, const int ta, const int tw, fstream& fin, fstream& fout) {
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

void checkFastest(const string& target, const int ta, const int tw, fstream& fin, fstream& fout) {
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
        showDifference(fout, f, ff);
    }
}

void checkShortest(const string& target, const int ta, const int tw, fstream& fin, fstream& fout) {
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
    const std::string DATA_PATH = "../raw/";
    const std::string OUT_PATH = "../res/res.out";

    // Access to output
    fstream fout(OUT_PATH, ios::out);
    if(!fout.is_open()) {
        cerr << "Failed to open output file!\n";
        return 1;
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
                    
                    mt19937_64 RNG(27);
                    auto rand = [&](const int& l, const int& r) -> int {
                        return uniform_int_distribution<int>(l, r)(RNG);
                    };

                    auto end = std::chrono::high_resolution_clock::now();
                    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
                    fout << "Time generating graph: " << elapsed.count() * 1e-9 << " seconds.\n";            
                    cerr << "Time generating graph: " << elapsed.count() * 1e-9 << " seconds.\n";            
                    
                    // Check for random 100 inputs
                    fout << "Average time for 100 random inputs:\n";

                    fout << "Streaming graph:\n";
                    double foremostTime = 0.0, reverseForemostTime = 0.0, fastestTime = 0.0, shortestTime = 0.0;
                    
                    for(int i=0;i<10;++i) {
                        const int target = rand(0, hasher.GetSize() - 1);
                        const int ta = 0, tw = INT_MAX;
                        
                        auto startRun = std::chrono::high_resolution_clock::now();
                        StreamPresentation::ComputingForemostTime(target, ta, tw);
                        auto endRun = std::chrono::high_resolution_clock::now();
                        foremostTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;

                        startRun = std::chrono::high_resolution_clock::now();
                        StreamPresentation::ComputingReverseForemostTime(target, ta, tw);\
                        endRun = std::chrono::high_resolution_clock::now();
                        reverseForemostTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;

                        startRun = std::chrono::high_resolution_clock::now();
                        StreamPresentation::ComputingFastestPathDuration(target, ta, tw);
                        endRun = std::chrono::high_resolution_clock::now();
                        fastestTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;

                        startRun = std::chrono::high_resolution_clock::now();
                        StreamPresentation::ComputingShortestPathDistance(target, ta, tw);
                        endRun = std::chrono::high_resolution_clock::now();
                        shortestTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
                    }

                    fout << "Foremost: " << foremostTime / 100.0f << " seconds.\n";
                    fout << "Reverse-Foremost: " << reverseForemostTime / 100.0f << " seconds.\n";
                    fout << "Fastest: " << fastestTime / 100.0f << " seconds.\n";
                    fout << "Shortest: " << shortestTime / 100.0f << " seconds.\n";

                    fout << "Transforming graph:\n";
                    foremostTime = 0.0, reverseForemostTime = 0.0, fastestTime = 0.0, shortestTime = 0.0;

                    for(int i=0;i<10;++i) {
                        const int target = rand(0, hasher.GetSize() - 1);
                        const int ta = 0, tw = INT_MAX;
                        
                        auto startRun = std::chrono::high_resolution_clock::now();
                        TransformingGraph::ComputingForemostTime(target, ta, tw);
                        auto endRun = std::chrono::high_resolution_clock::now();
                        foremostTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;

                        startRun = std::chrono::high_resolution_clock::now();
                        TransformingGraph::ComputingReverseForemostTime(target, ta, tw);
                        endRun = std::chrono::high_resolution_clock::now();
                        reverseForemostTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;

                        startRun = std::chrono::high_resolution_clock::now();
                        TransformingGraph::ComputingFastestPathDuration(target, ta, tw);
                        endRun = std::chrono::high_resolution_clock::now();
                        fastestTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;

                        startRun = std::chrono::high_resolution_clock::now();
                        TransformingGraph::ComputingShortestPathDistance(target, ta, tw);
                        endRun = std::chrono::high_resolution_clock::now();
                        shortestTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
                    }

                    fout << "Foremost: " << foremostTime / 100.0f << " seconds.\n";
                    fout << "Reverse-Foremost: " << reverseForemostTime / 100.0f << " seconds.\n";
                    fout << "Fastest: " << fastestTime / 100.0f << " seconds.\n";
                    fout << "Shortest: " << shortestTime / 100.0f << " seconds.\n";

                    fin.close();
                } else {
                    std::cerr << "Failed to open file: " << entry.path() << "\n";
                }

                auto end = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
                cerr << "Total time measured: " << elapsed.count() * 1e-9 << " seconds.\n";            
                fout << "Total time measured: " << elapsed.count() * 1e-9 << " seconds.\n\n";            
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return 1;
    }

    fout.close();
    
    if(fs::file_size(OUT_PATH) == 0) {
        cerr << "The res.out file is empty!\n";
        return 0;
    }

    string logDir = "../log/";
    
    // Ensure the log directory exists
    if(!fs::exists(logDir)) {
        fs::create_directory(logDir);
    }

    // Find the next available log file
    int logIndex = 0;
    string logPath;
    do {
        logPath = logDir + "log" + to_string(logIndex) + ".txt";
        logIndex++;
    }
    while(fs::exists(logPath)); // ensure the file name is unique

    try {
        fs::copy_file(OUT_PATH, logPath, fs::copy_options::overwrite_existing);
        cerr << "Log file is saved at: " << logPath << "\n";
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
