#pragma once

// #include <bits/stdc++.h>
#include <unordered_map>
#include <queue>
#include <string>

using namespace std;

// hashing
class Hasher {
private:
    unordered_map<string, int> string_to_id;
    unordered_map<int, string> id_to_string;
    queue<int> free_ids;
    int current_id = 0;
    int current_size = 0;

    // for allocating ids
    int fixed_ids = 0;

public:
    void AddFixedId(const int& n) {
        fixed_ids = n;
    }

    int GetFixedId() {
        return fixed_ids;
    }

    int AddString(const string& s) {
        if(string_to_id.find(s) != string_to_id.end()) {
            return string_to_id[s];
        }

        int new_id;
        if(!free_ids.empty()) {
            new_id = free_ids.front();
            free_ids.pop();
        }
        else {
            new_id = current_id++;
        }

        string_to_id[s] = new_id;
        id_to_string[new_id] = s;
        ++current_size;

        return new_id;
    }

    bool RemoveString(const string& s) {
        if(string_to_id.find(s) == string_to_id.end()) {
            return false;
        }

        int id = string_to_id[s];
        string_to_id.erase(s);
        id_to_string.erase(id);
        free_ids.push(id);
        --current_size;

        return true;
    }

    string GetString(int id) {
        return id_to_string.find(id) != id_to_string.end() ? id_to_string[id] : "ID not found";
    }

    int GetId(const string& s) {
        return string_to_id.find(s) != string_to_id.end() ? string_to_id[s] : -1;
    }

    int GetSize() {
        return current_size;
    }

    void Clear() {
        string_to_id.clear();
        id_to_string.clear();
        while(!free_ids.empty()) {
            free_ids.pop();
        }
        current_id = 0;
        current_size = 0;
    }
};

Hasher hasher;