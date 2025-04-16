#include "Hasher.h"

namespace fs = std::filesystem;

/*
Clarification:
input file .csv should in separated folder.
*/

const string raw_path = "../raw_real_data/";
const string out_path = "../../Data/";

// input from .csv file
void GenGraph(fstream& fin, fstream& fout) {
    vector<vector<string> > g;

    string line;
    getline(fin, line); // skip first line
    while(getline(fin, line)) {
        stringstream ss(line);
        vector<string> tokens;
        string token;

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        if (tokens.size() != 4) {
            cerr << "Invalid line: " << line << "\n"; // should not come here
            continue;
        }
        hasher.AddString(tokens[0]);
        timer.AddTime(tokens[1]);
        hasher.AddString(tokens[2]);
        timer.AddTime(tokens[3]);

        g.emplace_back(tokens);
    }

    // Preprocess the graph
    timer.Process();
    stable_sort(g.begin(), g.end(), [](const vector<string>& a, const vector<string>& b) {
        return timer.GetId(a[1]) < timer.GetId(b[1]);
    });

    fout << hasher.GetSize() << " " << g.size() << "\n";
    for(const vector<string>& e : g) {
        fout << hasher.GetId(e[0]) << " " << hasher.GetId(e[2]) << " " << timer.GetId(e[1]) << " " << timer.GetId(e[3]) - timer.GetId(e[1]) << "\n";
    }
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    try {
        for (const auto& entry : fs::recursive_directory_iterator(raw_path)) {
            if (entry.is_regular_file() && entry.path().extension() == ".csv") {
                fs::path relative = fs::relative(entry.path(), raw_path);
                fs::path out_subdir = fs::path(out_path) / relative.parent_path();

                std::string out_filename = "out." + entry.path().stem().string();
                fs::path out_file_path = out_subdir / out_filename;

                fs::create_directories(out_subdir);

                fstream fin(entry.path(), ios::in);
                if (!fin.is_open()) {
                    cerr << "Failed to open file: " << entry.path() << "\n";
                    continue;
                }
                fstream fout(out_file_path, ios::out);

                GenGraph(fin, fout);

                fin.close();
                fout.close();
            }
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "Filesystem error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}