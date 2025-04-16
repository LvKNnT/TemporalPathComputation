#include <bits/stdc++.h>

using namespace std;
namespace fs = filesystem;

/*
Modified disqualified graph to qualified graph.
Mostly change all the negative weight to positive weight.
Change float weight to int weight.
*/

const string input_path = "../../old_data";
const string output_path = "../../Data";

void ModGraph(fstream& fin, fstream& fout) {
    int n, m;
    fin >> n >> m;
    fout << n << " " << m << "\n";

    for(int i=0;i<m;++i) {
        int u, v;
        float ta, tw;
        fin >> u >> v >> ta >> tw;
        if(tw < 0) {
            tw = -tw;
        }

        fout << u << " " << v << " " << int(ta) << " " << int(tw) << "\n";
    }
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    try {
        for (const auto& entry : fs::recursive_directory_iterator(input_path)) {
            if (entry.is_regular_file() && entry.path().filename().string().rfind("out", 0) == 0) {
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

                ModGraph(fin, fout);

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