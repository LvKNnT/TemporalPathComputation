#include <bits/stdc++.h>

using namespace std;
namespace fs = std::filesystem;

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    const string data_path = "../task/";
    const string foremost_path = "../solution/foremost/model_lvk.exe";
    const string reverseforemost_path = "../solution/reverseforemost/model_lvk.exe";
    const string fastest_path = "../solution/fastest/model_lvk.exe";
    const string shortest_path = "../solution/shortest/model_lvk.exe";

    if(!fs::exists(foremost_path) || !fs::exists(reverseforemost_path) || !fs::exists(fastest_path) || !fs::exists(shortest_path)) {
        cerr << "One or more model files are missing!\n";
        return 1;
    }

    try {
        for (const auto& entry : fs::recursive_directory_iterator(data_path)) {
            if (entry.is_regular_file() && entry.path().extension() == ".INP") {
                cerr << "Processing file: " << entry.path().filename() << "\n";

                // use abs_path to get the absolute path of the input file
                fs::path input_path = entry.path();
                fs::path output_path = entry.path();
                output_path.replace_extension(".OUT");

                // get file name without extension
                const string type = entry.path().stem().string();

                if(type == "foremost") {
                    string command = "cmd /c \"\"" + foremost_path + "\" < \"" + input_path.generic_string() + "\" > \"" + output_path.generic_string() + "\"";
                    system(command.c_str());

                    cerr << "Foremost processing to:" << output_path << "\n";
                } else if(type == "reverseforemost") {
                    string command = "cmd /c \"\"" + reverseforemost_path + "\" < \"" + input_path.generic_string() + "\" > \"" + output_path.generic_string() + "\"";
                    system(command.c_str());

                    cerr << "ReverseForemost processing to:" << output_path << "\n";
                } else if(type == "fastest") {
                    string command = "cmd /c \"\"" + fastest_path + "\" < \"" + input_path.generic_string() + "\" > \"" + output_path.generic_string() + "\"";
                    system(command.c_str());

                    cerr << "Fastest processing to:" << output_path << "\n";
                } else if(type == "shortest") {
                    string command = "cmd /c \"\"" + shortest_path + "\" < \"" + input_path.generic_string() + "\" > \"" + output_path.generic_string() + "\"";
                    system(command.c_str());

                    cerr << "Shortest processing to:" << output_path << "\n";
                } else {
                    cerr << "Unknown processing type!\n" << input_path.generic_string() << "\n";
                }

            }
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "Filesystem error: " << e.what() << std::endl;
        return 1;
    }
}