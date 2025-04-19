// #include <bits/stdc++.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <random>
#include <functional>

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
    int n, m;
    {
        // Process the first line
        getline(fin, line);
        stringstream ss(line);
        ss >> n >> m;
        hasher.AddFixedId(n);
    }
    for(int i=0;i<m;++i) {
        getline(fin, line);
        if(!line.empty() && line[0] == '%') continue; // skipping comment line
        
        stringstream ss(line);
        string u, v;
        int ta, tw;
        if((ss >> u >> v >> ta >> tw) && tw >= 0) {
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
    TransformingGraph::GenerateGraph(g);
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - sbegin);
    fout << "generating transforming graph: " << elapsed.count() * 1e-9 << " seconds.\n";            
    cerr << "generating transforming graph: " << elapsed.count() * 1e-9 << " seconds.\n";     
    
    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    fout << "Total time: " << elapsed.count() * 1e-9 << " seconds.\n";            
    cerr << "Total time: " << elapsed.count() * 1e-9 << " seconds.\n";     

    fout << "Stream vectice count: " << hasher.GetSize() << "\n";
    cerr << "Stream vectice count: " << hasher.GetSize() << "\n";
    fout << "Stream edges count: " << g.size() << "\n";
    cerr << "Stream edges count: " << g.size() << "\n";

    fout << "Transforming vectice count: " << TransformingGraph::vertices_count << "\n";
    cerr << "Transforming vectice count: " << TransformingGraph::vertices_count << "\n";
    fout << "Transforming edges count: " << TransformingGraph::edges_count << "\n";
    cerr << "Transforming edges count: " << TransformingGraph::edges_count << "\n";
}

void ClearGraph() {
    StreamPresentation::ClearGraph();
    TransformingGraph::ClearGraph();
    hasher.Clear();
}

void output_d(const string& path, const vector<int>& f, bool rev) {
    fstream fout(path, ios::out);

    map<int, int> mp;
    for(int i = 0; i < hasher.GetSize(); ++i) {
        mp[stoi(hasher.GetString(i))] = f[i];
    }
    for(const pair<int, int>& v : mp) {
        if(rev) {
            fout << v.first << ": " << (v.second != INT_MIN ? v.second : -1) << "\n";
        }
        else {
            fout << v.first << ": " << (v.second != INT_MAX ? v.second : -1) << "\n";
        }
    }
    fout << "\n";

    fout.close();
}

void output_distance(const string& path, const string& target, const int& ta, const int& tw) {
    vector<int> f = StreamPresentation::ComputingForemostTime(hasher.GetId(target), ta, tw);
    output_d(path + "/foremost.txt", f, false);

    f = StreamPresentation::ComputingReverseForemostTime(hasher.GetId(target), ta, tw);
    output_d(path + "/reverseforemost.txt", f, false);

    f = StreamPresentation::ComputingFastestPathDuration(hasher.GetId(target), ta, tw);
    output_d(path + "/fastest.txt", f, false);

    f = StreamPresentation::ComputingShortestPathDistance(hasher.GetId(target), ta, tw);
    output_d(path + "/shortest.txt", f, false);
}

// Retrieve the path from the parent list in O(nlogn)
vector<vector<int> > ComputingPath(const int& n, const vector<int>& path) {
    vector<vector<int> > f(n, vector<int> (0));
    vector<int> visited(n, 0);

    function<void(int)> dfs = [&](int node) {
        if (visited[node]) return;
        visited[node] = 1;

        if (path[node] != -1 && node != path[node]) {
            dfs(path[node]);
            f[node] = f[path[node]];
        }
        f[node].push_back(node);
    };

    for (int i = 0; i < n; ++i) {
        if (!visited[i] && path[i] != -1) {
            dfs(i);
        }
    }

    return f;
}

void output_p(const string& path, const vector<vector<int> >& f) {
    fstream fout(path, ios::out);

    map<int, vector<int> > mp;
    for(int i = 0; i < hasher.GetSize(); ++i) {
        mp[stoi(hasher.GetString(i))] = f[i];
    }
    for(const pair<int, vector<int> >& v : mp) {
        fout << v.first << ": ";
        for(int i = 0; i < v.second.size(); ++i) {
            fout << hasher.GetString(v.second[i]) << " ";
        }
        if(v.second.empty()) fout << "-1";
        fout << "\n";
    }
}

void output_path(const string& path, const string& target, const int& ta, const int& tw) {
    vector<int> f = StreamPresentation::GetForemostTimePath(hasher.GetId(target), ta, tw);
    vector<vector<int> > p = ComputingPath(hasher.GetSize(), f); 
    output_p(path + "/foremost.txt", p);

    f = StreamPresentation::GetReverseForemostPath(hasher.GetId(target), ta, tw);
    p = ComputingPath(hasher.GetSize(), f);
    output_p(path + "/reverseforemost.txt", p);

    f = StreamPresentation::GetFastestPath(hasher.GetId(target), ta, tw);
    p = ComputingPath(hasher.GetSize(), f);
    output_p(path + "/fastest.txt", p);

    f = StreamPresentation::GetShortestPath(hasher.GetId(target), ta, tw);
    p = ComputingPath(hasher.GetSize(), f);
    output_p(path + "/shortest.txt", p);
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

bool checkForemost(const string& target, const int ta, const int tw, fstream& fin, fstream& fout) {
    fout << "Foremost!\n";

    // Setting input for both solutions
    const int des = hasher.GetId(target);
    if(des == -1) {
        fout << "Input error!\n";
        return false;
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
        return true;
    }

    fout << "False!\n";
    showDifference(fout, f, ff);
    return false;
}

bool checkReverseForemost(const string& target, const int ta, const int tw, fstream& fin, fstream& fout) {
    fout << "Reverse-Foremost!\n";

    // Setting input for both solutions
    const int des = hasher.GetId(target);
    if(des == -1) {
        fout << "Input error!\n";
        return false;
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
        return true;
    }
    
    fout << "False!\n";
    showDifference(fout, f, ff);
    return false;
}

bool checkFastest(const string& target, const int ta, const int tw, fstream& fin, fstream& fout) {
    fout << "Fastest!\n";

    // Setting input for both solutions
    const int des = hasher.GetId(target);
    if(des == -1) {
        fout << "Input error!\n";
        return false;
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
        return true;
    }
    
    fout << "False!\n";
    showDifference(fout, f, ff);
    return false;
}

bool checkShortest(const string& target, const int ta, const int tw, fstream& fin, fstream& fout) {
    fout << "Shortest!\n";

    // Setting input for both solutions
    const int des = hasher.GetId(target);
    if(des == -1) {
        fout << "Input error!\n";
        return false;
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
        return true;
    }
    
    fout << "False!\n";
    showDifference(fout, f, ff);
    return false;
}

void testRandomNodesStreamGraph(int n, const int64_t& RNGcode, fstream& fin, fstream& fout) {
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
    cerr << "Test Random Stream Graph time measured: " << elapsed.count() * 1e-9 << " seconds.\n";            
    fout << "Test Random Stream Graph time measured: " << elapsed.count() * 1e-9 << " seconds.\n\n";       
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
    cerr << "Test Random Nodes time measured: " << elapsed.count() * 1e-9 << " seconds.\n";            
    fout << "Test Random Nodes time measured: " << elapsed.count() * 1e-9 << " seconds.\n\n";                
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

bool checkAccurate(const string& target, const int& ta, const int& tw, fstream& fin, fstream& fout) {
    // Counting time testing for each input
    auto begin = std::chrono::high_resolution_clock::now();

    bool yes = true;
    
    yes = yes && checkForemost(target, ta, tw, fin, fout);
    yes = yes && checkReverseForemost(target, ta, tw, fin, fout);
    yes = yes && checkFastest(target, ta, tw, fin, fout);
    yes = yes && checkShortest(target, ta, tw, fin, fout);

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cerr << "Test Accurate time measured: " << elapsed.count() * 1e-9 << " seconds.\n";            
    fout << "Test Accurate time measured: " << elapsed.count() * 1e-9 << " seconds.\n\n";  

    return yes;
}

// Test generator for themis
void testGenerator(fstream& ftest, fstream& fsolution, const int k) {
    // get node k of the graph
    const int node = (hasher.GetSize() - 1) * k;
    const int ta = 0, tw = INT_MAX;

    ftest << hasher.GetFixedId() << " " << StreamPresentation::g.size() << "\n";
    ftest << hasher.GetString(node) << " " << ta << " " << tw << "\n";

    for(const auto& [u, v, ta, tw] : StreamPresentation::g) {
        ftest << hasher.GetString(u) << " " << hasher.GetString(v) << " " << ta << " " << tw << "\n";
    }

    const int INF = 2e9;
    vector<int> f = StreamPresentation::ComputingReverseForemostTime(node, ta, tw);
    vector<int> ans(hasher.GetFixedId(), INT_MIN);
    for(int i = 0; i < hasher.GetSize(); ++i) {
        ans[stoi(hasher.GetString(i)) - 1] = f[i];
    }
    for(const int& v : ans) {
        fsolution  << (v >= -INF ? v : -1) << " ";
    }
}

signed main() {
    const std::string DISTANCE_PATH = "../solution/distance/";
    const std::string PATH_PATH = "../solution/path/";
    // const std::string DATA_PATH = "../raw/";
    // const std::string DATA_PATH = "../raw/ca-cit-HepPh/";
    // const std::string DATA_PATH = "../raw/youtube-u-growth/";
    // const std::string DATA_PATH = "../raw/dblp_coauthor/";
    const std::string DATA_PATH = "../Themis/Data/";
    const std::string TEST_PATH = "../Themis/Tasks/reverseforemost/";
    const std::string OUT_PATH = "../res/res.out";

    // Access to output
    fstream fout(OUT_PATH, ios::out);
    if(!fout.is_open()) {
        cerr << "Failed to open output file!\n";
        return 1;
    }

    const int randomNode = 100;
    char16_t* randomNodePtr = new char16_t;
    const int64_t RNGcode = (int64_t) randomNodePtr;
    delete randomNodePtr;
    
    /*
    const int RNGcode = 2703;
    */
    fout << "Random nodes: " << randomNode << "\n";
    cerr << "Random nodes: " << randomNode << "\n";
    fout << "RNG code: " << RNGcode << "\n";
    cerr << "RNG code: " << RNGcode << "\n";

    try {
        for (const auto& entry : fs::recursive_directory_iterator(DATA_PATH)) {
            if (entry.is_regular_file() && entry.path().filename().string().rfind("out", 0) == 0) {
                cerr << "Processing file: " << entry.path().filename() << "\n";
                fout << "Current file: " << entry.path().filename() << "\n";

                fstream fin(entry.path(), ios::in);
                if(!fin.is_open()) {
                    cerr << "Failed to open file!\n";
                    continue;
                }  
                
                getGraph(fin, fout);
                if(StreamPresentation::g.size() == 0) {
                    cerr << "Input error!\n";
                    // fout << "Input error!\n";
                    
                    ClearGraph();
                    fin.close();
                    continue;
                }
                    
                /*
                if(checkAccurate("1", 0, INT_MAX, fin, fout)) {
                    testRandomNodes(randomNode, RNGcode, fin, fout);
                }   
                        
                // Test for random nodes
                testRandomNodes(randomNode, RNGcode, fin, fout);
                
                testRandomNodesStreamGraph(randomNode, RNGcode, fin, fout);
                
                // Check for accuracy
                checkAccurate("1", 0, INT_MAX, fin, fout);
                */
               
                /*
                // Output distance
                output_distance(DISTANCE_PATH + entry.path().parent_path().filename().string(), "1", 0, INT_MAX);
                // Output path 
                output_path(PATH_PATH + entry.path().parent_path().filename().string(), "1", 0, INT_MAX);
                */
                
                // Test generator for themis
                
                const string testName = entry.path().parent_path().filename().string();
                
                // Makeing test in subfolder like themis format
                for(int i=0;i<2;++i) {
                    // Create subfolder
                    fs::path subfolder = fs::path(TEST_PATH) / (testName + "_" + to_string(i+1));
                    fs::create_directories(subfolder);
                    
                    // Create test file
                    fs::path testFile = subfolder / ("reverseforemost.INP");
                    fs::path solutionFile = subfolder / ("reverseforemost.OUT");
                    
                    fstream testFileStream(testFile, ios::out);
                    if(!testFileStream.is_open()) {
                        cerr << "Failed to open test file!\n";
                        continue;
                    }
                    fstream solutionFileStream(solutionFile, ios::out);
                    if(!solutionFileStream.is_open()) {
                        cerr << "Failed to open solution file!\n";
                        continue;
                    }
                    
                    testGenerator(testFileStream, solutionFileStream, i);
                    
                    testFileStream.close();
                    solutionFileStream.close();
                }
                
                ClearGraph();
                
                fin.close();
            }
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "Filesystem error: " << e.what() << std::endl;
        return 1;
    }

    fout.close();

    // createLog(OUT_PATH);

    return 0;
}