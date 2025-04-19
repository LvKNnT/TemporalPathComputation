#include <bits/stdc++.h>

int call(std::string a) {
	return system(a.c_str());
}

int main(void) {
	std::string baseFolder = "solution";
	std::string exeName = "model_lvk.exe";
	std::vector<std::string> tasks = {
		"foremost",
		"reverseforemost",
		"fastest",
		"shortest"
	};
	for (auto task : tasks) {
		call("mkdir task\\" + task);
		for (int i = 1; i <= 16; i += 1) {
			std::string id = std::to_string(i);
			if (id.size() < 2) {
				id.insert(id.begin(), '0');
			}
			std::string folder = "task\\" + task + "\\TEST" + id;
			std::cerr << folder << '\n';
			call("mkdir " + folder);
			std::string input = folder + "\\" + task + ".INP";
			std::string output = folder + "\\" + task + ".OUT";
			call("copy input\\" + id + " " + input);
			call("\"" + baseFolder + "\\" + task + "\\" + exeName + "\"" + " < " + input + " > " + output);
		}
	}
}
