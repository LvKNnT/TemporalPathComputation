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

void getGraph(fstream& fin, fstream& fout) {
    fout << "Generating graph...\n";
    cerr << "Generating graph...\n";
    auto begin = std::chrono::high_resolution_clock::now();
    auto sbegin = std::chrono::high_resolution_clock::now();
    
    vector<tuple<int, int, int, int> > g;
    
    string line;
    while(getline(fin, line)) {
        if(!line.empty() && line[0] == '%') continue; // skipping comment line
        
        stringstream ss(line);
        string u, v;
        int ta, tw;
        if((ss >> u >> v >> tw >> ta) && ta >= 0) {
            g.push_back({hasher.AddString(u), hasher.AddString(v), ta, tw});
        }
        else {
            fout << "Invalid line!\n"; // should not come here
            cerr << "Invalid line!\n"; // should not come here
            return;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    fout << "getting graph: " << elapsed.count() * 1e-9 << " seconds.\n";
    cerr << "getting graph: " << elapsed.count() * 1e-9 << " seconds.\n";
    
    sbegin = std::chrono::high_resolution_clock::now();
    StreamPresentation::GenerateGraph(g);
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - sbegin);
    fout << "generating streaming graph: " << elapsed.count() * 1e-9 << " seconds.\n";            
    cerr << "generating streaming graph: " << elapsed.count() * 1e-9 << " seconds.\n";     
    
    sbegin = std::chrono::high_resolution_clock::now();
    // TransformingGraph::GenerateGraph(g);
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - sbegin);
    fout << "generating transforming graph: " << elapsed.count() * 1e-9 << " seconds.\n";            
    cerr << "generating transforming graph: " << elapsed.count() * 1e-9 << " seconds.\n";     
    
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    fout << "Total time: " << elapsed.count() * 1e-9 << " seconds.\n";            
    cerr << "Total time: " << elapsed.count() * 1e-9 << " seconds.\n";     

    fout << "Vectice count: " << hasher.GetSize() << "\n";
    cerr << "Vectice count: " << hasher.GetSize() << "\n";
    fout << "Edges count: " << g.size() << "\n";
    cerr << "Edges count: " << g.size() << "\n";
}

void ClearGraph() {
    StreamPresentation::ClearGraph();
    TransformingGraph::ClearGraph();
}

void output(const string& path, const vector<int>& f) {
    fstream fout(path, ios::out);

    map<int, int> mp;
    for(int i = 0; i < hasher.GetSize(); ++i) {
        mp[stoi(hasher.GetString(i))] = f[i];
    }
    for(const pair<int, int>& v : mp) {
        fout << v.first << ": " << v.second << "\n";
    }
    fout << "\n";

    fout.close();
}

void output_distance(const string& path, const string& target, const int& ta, const int& tw) {
    vector<int> f = StreamPresentation::ComputingForemostTime(hasher.GetId(target), ta, tw);
    output(path + "/foremost.txt", f);

    f = StreamPresentation::ComputingReverseForemostTime(hasher.GetId(target), ta, tw);
    output(path + "/reverseforemost.txt", f);

    f = StreamPresentation::ComputingFastestPathDuration(hasher.GetId(target), ta, tw);
    output(path + "/fastest.txt", f);

    f = StreamPresentation::ComputingShortestPathDistance(hasher.GetId(target), ta, tw);
    output(path + "/shortest.txt", f);
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

void testRandomNodesStreamGraph(int n, int RNGcode, fstream& fin, fstream& fout) {
    // Counting time testing for each input
    auto begin = std::chrono::high_resolution_clock::now();

    // RNG
    mt19937_64 RNG(RNGcode);
    auto rand = [&](const int& l, const int& r) -> int {
        return uniform_int_distribution<int>(l, r)(RNG);
    };
    
    // Check for random n inputs
    fout << "Average time for " << n << " random inputs:\n";

    double foremostTime = 0.0, reverseForemostTime = 0.0, fastestTime = 0.0, shortestTime = 0.0;
    
    for(int i=0;i<n;++i) {
        const int target = rand(0, hasher.GetSize() - 1);
        const int ta = 0, tw = INT_MAX;
        
        // Streaming Graph
        auto startRun = std::chrono::high_resolution_clock::now();
        StreamPresentation::ComputingForemostTime(target, ta, tw);
        auto endRun = std::chrono::high_resolution_clock::now();
        foremostTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
        
        startRun = std::chrono::high_resolution_clock::now();
        StreamPresentation::ComputingReverseForemostTime(target, ta, tw);
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
    
    fout << "Streaming graph:\n";
    fout << "Foremost: " << foremostTime / n << " seconds.\n";
    fout << "Reverse-Foremost: " << reverseForemostTime / n << " seconds.\n";
    fout << "Fastest: " << fastestTime / n << " seconds.\n";
    fout << "Shortest: " << shortestTime / n << " seconds.\n";

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cerr << "Test time measured: " << elapsed.count() * 1e-9 << " seconds.\n";            
    fout << "Test time measured: " << elapsed.count() * 1e-9 << " seconds.\n\n";       
}

void testRandomNodes(int n, int RNGcode, fstream& fin, fstream& fout) {
    // Counting time testing for each input
    auto begin = std::chrono::high_resolution_clock::now();

    // RNG
    mt19937_64 RNG(RNGcode);
    auto rand = [&](const int& l, const int& r) -> int {
        return uniform_int_distribution<int>(l, r)(RNG);
    };
    
    // Check for random 100 inputs
    fout << "Average time for " << n << " random inputs:\n";

    double foremostTime = 0.0, reverseForemostTime = 0.0, fastestTime = 0.0, shortestTime = 0.0;
    double tforemostTime = 0.0, treverseForemostTime = 0.0, tfastestTime = 0.0, tshortestTime = 0.0;
    
    for(int i=0;i<n;++i) {
        const int target = rand(0, hasher.GetSize() - 1);
        const int ta = 0, tw = INT_MAX;
        
        // Streaming Graph
        auto startRun = std::chrono::high_resolution_clock::now();
        StreamPresentation::ComputingForemostTime(target, ta, tw);
        auto endRun = std::chrono::high_resolution_clock::now();
        foremostTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
        
        startRun = std::chrono::high_resolution_clock::now();
        StreamPresentation::ComputingReverseForemostTime(target, ta, tw);
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
        
        // Transforming Graph
        startRun = std::chrono::high_resolution_clock::now();
        TransformingGraph::ComputingForemostTime(target, ta, tw);
        endRun = std::chrono::high_resolution_clock::now();
        tforemostTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
        
        startRun = std::chrono::high_resolution_clock::now();
        TransformingGraph::ComputingReverseForemostTime(target, ta, tw);
        endRun = std::chrono::high_resolution_clock::now();
        treverseForemostTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
        
        startRun = std::chrono::high_resolution_clock::now();
        TransformingGraph::ComputingFastestPathDuration(target, ta, tw);
        endRun = std::chrono::high_resolution_clock::now();
        tfastestTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
        
        startRun = std::chrono::high_resolution_clock::now();
        TransformingGraph::ComputingShortestPathDistance(target, ta, tw);
        endRun = std::chrono::high_resolution_clock::now();
        tshortestTime += std::chrono::duration_cast<std::chrono::nanoseconds>(endRun - startRun).count() * 1e-9;
    }
    
    fout << "Streaming graph:\n";
    fout << "Foremost: " << foremostTime / n << " seconds.\n";
    fout << "Reverse-Foremost: " << reverseForemostTime / n << " seconds.\n";
    fout << "Fastest: " << fastestTime / n << " seconds.\n";
    fout << "Shortest: " << shortestTime / n << " seconds.\n";

    fout << "Transforming graph:\n";
    fout << "Foremost: " << tforemostTime / n << " seconds.\n";
    fout << "Reverse-Foremost: " << treverseForemostTime / n << " seconds.\n";
    fout << "Fastest: " << tfastestTime / n << " seconds.\n";
    fout << "Shortest: " << tshortestTime / n << " seconds.\n";

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cerr << "Test time measured: " << elapsed.count() * 1e-9 << " seconds.\n";            
    fout << "Test time measured: " << elapsed.count() * 1e-9 << " seconds.\n\n";                
}

void createLog(const string& OUT_PATH) {
    if(fs::file_size(OUT_PATH) == 0) {
        cerr << "The res.out file is empty!\n";
        return;
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
        cerr << "Filesystem error: " << e.what() << std::endl;
    }
}

void checkAccurate(const string& target, const int& ta, const int& tw, fstream& fin, fstream& fout) {
    // Counting time testing for each input
    auto begin = std::chrono::high_resolution_clock::now();

    
    checkForemost(target, ta, tw, fin, fout);
    checkReverseForemost(target, ta, tw, fin, fout);
    checkFastest(target, ta, tw, fin, fout);
    checkShortest(target, ta, tw, fin, fout);

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cerr << "Test time measured: " << elapsed.count() * 1e-9 << " seconds.\n";            
    fout << "Test time measured: " << elapsed.count() * 1e-9 << " seconds.\n\n";  
}

signed main() {
    const std::string DISTANCE_PATH = "../distance/";
    // const std::string DATA_PATH = "../raw/";
    // const std::string DATA_PATH = "../raw/youtube-u-growth/";
    // const std::string DATA_PATH = "../raw/dblp_coauthor/";
    const std::string DATA_PATH = "../Data/";
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
                cerr << "Processing file: " << entry.path().filename() << "\n";
                fout << "Current file: " << entry.path().filename() << "\n";

                fstream fin(entry.path(), ios::in);
                if(!fin.is_open()) {
                    cerr << "Failed to open file!\n";
                    return 1;
                }  
                
                getGraph(fin, fout);

                const int randomNode = 100;
                const int RNGcode = 2703;
                fout << "Random nodes: " << randomNode << "\n";
                fout << "RNG code: " << RNGcode << "\n";
                
                /*
                // Test for random nodes
                testRandomNodes(randomNode, RNGcode, fin, fout);
                */

                testRandomNodesStreamGraph(randomNode, RNGcode, fin, fout);
                
                /*
                // Check for accuracy
                checkAccurate("1", 0, INT_MAX, fin, fout);
                
                // Output distance
                output_distance(DISTANCE_PATH + entry.path().parent_path().filename().string(), "1", 0, INT_MAX);
                */

                ClearGraph();
                
                fin.close();
            }
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "Filesystem error: " << e.what() << std::endl;
        return 1;
    }

    fout.close();

    createLog(OUT_PATH);

    return 0;
}