#include <bits/stdc++.h>

using namespace std;
namespace fs = std::filesystem;

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    const string data_path = "../task/";
    const string foremost_path = "../solution/foremost/trans_lvk.exe";
    const string reverseforemost_path = "../solution/reverseforemost/trans_lvk.exe";
    const string fastest_path = "../solution/fastest/trans_lvk.exe";
    const string shortest_path = "../solution/shortest/trans_lvk.exe";

    if(!fs::exists(foremost_path) || !fs::exists(reverseforemost_path) || !fs::exists(fastest_path) || !fs::exists(shortest_path)) {
        cerr << "One or more model files are missing!\n";
        return 1;
    }

    float foremost_time = 0.0f;
    float reverseforemost_time = 0.0f;
    float fastest_time = 0.0f;
    float shortest_time = 0.0f;

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
                    
                    auto begin = std::chrono::high_resolution_clock::now();
                    system(command.c_str());
                    auto end = std::chrono::high_resolution_clock::now();
                    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
                    foremost_time += elapsed.count() * 1e-9;

                    cerr << "Foremost processing to:" << output_path << "\n";
                } else if(type == "reverseforemost") {
                    string command = "cmd /c \"\"" + reverseforemost_path + "\" < \"" + input_path.generic_string() + "\" > \"" + output_path.generic_string() + "\"";
                    
                    auto begin = std::chrono::high_resolution_clock::now();
                    system(command.c_str());
                    auto end = std::chrono::high_resolution_clock::now();
                    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
                    reverseforemost_time += elapsed.count() * 1e-9;

                    cerr << "ReverseForemost processing to:" << output_path << "\n";
                } else if(type == "fastest") {
                    string command = "cmd /c \"\"" + fastest_path + "\" < \"" + input_path.generic_string() + "\" > \"" + output_path.generic_string() + "\"";
                    
                    auto begin = std::chrono::high_resolution_clock::now();
                    system(command.c_str());
                    auto end = std::chrono::high_resolution_clock::now();
                    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
                    fastest_time += elapsed.count() * 1e-9;

                    cerr << "Fastest processing to:" << output_path << "\n";
                } else if(type == "shortest") {
                    string command = "cmd /c \"\"" + shortest_path + "\" < \"" + input_path.generic_string() + "\" > \"" + output_path.generic_string() + "\"";
                    
                    auto begin = std::chrono::high_resolution_clock::now();
                    system(command.c_str());
                    auto end = std::chrono::high_resolution_clock::now();
                    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
                    shortest_time += elapsed.count() * 1e-9;

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

    cerr << "Foremost time: " << foremost_time / 16 << " seconds.\n";
    cerr << "ReverseForemost time: " << reverseforemost_time / 16 << " seconds.\n";
    cerr << "Fastest time: " << fastest_time / 16 << " seconds.\n";
    cerr << "Shortest time: " << shortest_time / 16 << " seconds.\n";
}