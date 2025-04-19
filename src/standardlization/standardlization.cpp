#include <bits/stdc++.h>

using namespace std;
namespace fs = filesystem;

/*
since the paper data has wrong format, we need to modify it.

Modified disqualified graph to qualified graph.
Mostly change all the negative weight to positive weight.
Change float weight to int weight.
*/

const string input_path = "../../Data";
const string output_path = "../../Themis/Data";

bool ModGraph(fstream& fin, fstream& fout) {
    int n, m;
    fin >> n >> m;
    fout << n << " " << m << "\n";

    // first constrant
    if(n > 1000000 || m > 1000000) {
        cerr << "n|m > 1000000\n";
        return false;
    }

    vector<tuple<int, int, int, int> > g;

    for(int i=0;i<m;++i) {
        int u, v;
        float ta, tw;
        fin >> u >> v >> ta >> tw;
        if(tw < 0) {
            tw = -tw;
        }

        if(ta > INT_MAX - tw) {
            tw = INT_MAX - ta;
        }

        g.push_back({u, v, static_cast<int>(ta), static_cast<int>(tw)});
    }

    stable_sort(g.begin(), g.end(), [](const auto& a, const auto& b) {
        return get<2>(a) < get<2>(b);
    });

    for(const auto& [u, v, ta, tw] : g) {
        fout << u << " " << v << " " << ta << " " << tw << "\n";
    }

    return true;
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    try {
        for (const auto& entry : fs::recursive_directory_iterator(input_path)) {
            if (entry.is_regular_file() && entry.path().filename().string().rfind("out", 0) == 0) {
                cerr << "Processing file: " << entry.path().filename().string() << "\n";

                fs::path relative = fs::relative(entry.path(), input_path);
                fs::path out_subdir = fs::path(output_path) / relative.parent_path();

                std::string out_filename = entry.path().filename().string();
                fs::path out_file_path = out_subdir / out_filename;

                fs::create_directories(out_subdir);

                fstream fin(entry.path(), ios::in);
                if (!fin.is_open()) {
                    cerr << "Failed to open file: " << entry.path() << "\n";
                    continue;
                }
                fstream fout(out_file_path, ios::out);

                if(ModGraph(fin, fout) == false) {
                    cerr << "ModGraph failed!\n";
                    fout.close();

                    fs::remove_all(out_subdir); // remove the subdir

                    continue;
                }

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